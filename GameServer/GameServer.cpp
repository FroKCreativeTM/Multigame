#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Protocol.pb.h"
#include "Job.h"
#include "RoomManager.h"
#include "GameRoom.h"
#include "Player.h"

#include "ConfigManager.h"
#include "DataManager.h"

#include <DBBind.h>

enum
{
	WORKER_TICK = 64
};

void DoWorkerJob(ServerServiceRef& service)
{
	while (true)
	{
		LEndTickCount = ::GetTickCount64() + WORKER_TICK;

		GRoom->DoTimer(50, &GameRoom::Update);

		// 네트워크 입출력 처리 -> 인게임 로직까지 (패킷 핸들러에 의해)
		service->GetIocpCore()->Dispatch(10);

		// 예약된 일감 처리
		ThreadManager::DistributeReservedJobs();

		// 글로벌 큐
		ThreadManager::DoGlobalQueueWork();
	}
}

uint64 lastTick = 0;

unique_ptr<DBConnectionPool> GDBConnectionPool = make_unique<DBConnectionPool>();

int main()
{
	// 먼저 Config나 Data등등을 들고 오자.
	ConfigManager::GetInst()->LoadConfig();
	DataManager::GetInst()->LoadData();

	// DB부터 켜자
	// 1번째 인자 : 스레드 수
	// 2번째 인자 : 연결을 위한 DSN 
	ASSERT_CRASH(GDBConnectionPool->Connect(1, ConfigManager::GetInst()->GetServerConfig().dbConnectionString.c_str()));

	// Drop Table If Exists
	{
		auto query = L"DROP TABLE  if exists `Gold`;";

		DBConnection* dbConn = GDBConnectionPool->Pop();
		ASSERT_CRASH(dbConn->Execute(query));
		GDBConnectionPool->Push(dbConn);
	}

	// Create Table
	{
		auto query = L"CREATE TABLE `Gold` (					\
						`id` INT NOT NULL AUTO_INCREMENT,		\
						`gold` INT NULL,						\
						`name` VARCHAR(200) NULL,				\
						`createData` DATETIME NULL,				\
						PRIMARY KEY(`id`));";

		DBConnection* dbConn = GDBConnectionPool->Pop();
		ASSERT_CRASH(dbConn->Execute(query));
		GDBConnectionPool->Push(dbConn);
	}

	// Add Data
	for (int32 i = 0; i < 3; i++)
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();

		/*
		// 기존에 바인딩된 인자들을 날린다.
		dbConn->Unbind();

		// 넘길 인자들
		int32 gold = 100;
		SQLLEN len = 0;

		WCHAR name[100] = L"Frok";
		SQLLEN nameLen = 0;

		TIMESTAMP_STRUCT ts = {};
		ts.year = 2022;
		ts.month = 5;
		ts.day = 19;
		SQLLEN tsLen = 0;
		
		// 인자들을 인덱스에 바인딩한다.
		ASSERT_CRASH(dbConn->BindParam(1, &gold, &len));
		ASSERT_CRASH(dbConn->BindParam(2, name, &nameLen));
		ASSERT_CRASH(dbConn->BindParam(3, &ts, &tsLen));

		// SQL을 실행한다.
		ASSERT_CRASH(dbConn->Execute(L"INSERT INTO `Gold`(`gold`, `name`, `createData`) VALUES(?, ?, ?)"));
		*/

		// Input 3개 Output 0개
		DBBind<3, 0> dbBind(*dbConn, L"INSERT INTO `Gold`(`gold`, `name`, `createData`) VALUES(?, ?, ?)");

		int32 gold = 100;
		dbBind.BindParam(0, gold);
		WCHAR name[100] = L"테스트";
		dbBind.BindParam(1, name);
		TIMESTAMP_STRUCT ts = { 2022, 5, 19 };
		dbBind.BindParam(2, ts);

		ASSERT_CRASH(dbBind.Execute());

		GDBConnectionPool->Push(dbConn);
	}

	// Data read
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();

		/*
		// 기존에 바인딩된 인자들을 날린다.
		dbConn->Unbind();

		int32 gold = 100;
		SQLLEN len = 0;

		// 인자들을 인덱스에 바인딩한다.
		ASSERT_CRASH(dbConn->BindParam(1, &gold, &len));

		int32 outId = 0;
		SQLLEN outIdLen = 0;
		ASSERT_CRASH(dbConn->BindCol(1, &outId, &outIdLen));

		int32 outGold = 0;
		SQLLEN outGoldLen = 0;
		ASSERT_CRASH(dbConn->BindCol(2, &outGold, &outGoldLen));

		WCHAR outName[100];
		SQLLEN outNameLen = 0;
		ASSERT_CRASH(dbConn->BindCol(3, outName, len32(outName), &outNameLen));

		TIMESTAMP_STRUCT outTs = {};
		SQLLEN outTsLen = 0;
		ASSERT_CRASH(dbConn->BindCol(4, &outTs, &outTsLen));

		// SQL을 실행한다.
		ASSERT_CRASH(dbConn->Execute(L"SELECT id, gold, name, createData FROM `Gold` WHERE `gold` = (?)"));
		*/

		// Input 1개 Output 4개
		DBBind<1, 4> dbBind(*dbConn, L"SELECT id, gold, name, createData FROM `Gold` WHERE `gold` = (?)");

		int32 gold = 100;
		dbBind.BindParam(0, gold);

		int32 outId = 0;
		int32 outGold = 0;
		WCHAR outName[100];
		TIMESTAMP_STRUCT outTs;
		dbBind.BindCol(0, outId);
		dbBind.BindCol(1, outGold);
		dbBind.BindCol(2, outName);
		dbBind.BindCol(3, outTs);

		ASSERT_CRASH(dbBind.Execute());

		// 데이터가 있는 동안 반복적으로 가져온다.
		wcout.imbue(locale("kor"));
		while (dbConn->Fetch())
		{
			wcout << "Id : " << outId << " Gold : " << outGold << " name : " << outName << endl;
			cout << "Date : " << outTs.year << "/" << outTs.month << "/" << outTs.day << endl;
		}

		GDBConnectionPool->Push(dbConn);
	}

	ClientPacketHandler::Init();

	GRoom = RoomManager::GetInst()->Add(1);

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([&service]()
			{
				DoWorkerJob(service);
			});
	}

	// Main Thread
	DoWorkerJob(service);

	GThreadManager->Join();
}