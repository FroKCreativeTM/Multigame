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
						PRIMARY KEY(`id`));";

		DBConnection* dbConn = GDBConnectionPool->Pop();
		ASSERT_CRASH(dbConn->Execute(query));
		GDBConnectionPool->Push(dbConn);
	}

	// Add Data
	for (int32 i = 0; i < 3; i++)
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();

		// 기존에 바인딩된 인자들을 날린다.
		dbConn->Unbind();

		// 넘길 인자들
		int32 gold = 100;
		SQLLEN len = 0;
		
		// 인자들을 인덱스에 바인딩한다.
		dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len);

		// SQL을 실행한다.
		ASSERT_CRASH(dbConn->Execute(L"INSERT INTO `Gold`(`gold`) VALUES(?)"));
		GDBConnectionPool->Push(dbConn);
	}

	// Data read
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();

		// 기존에 바인딩된 인자들을 날린다.
		dbConn->Unbind();

		int32 gold = 100;
		SQLLEN len = 0;

		// 인자들을 인덱스에 바인딩한다.
		ASSERT_CRASH(dbConn->BindParam(1, SQL_C_LONG, SQL_INTEGER, sizeof(gold), &gold, &len));

		int32 outId = 0;
		SQLLEN outIdLen = 0;
		ASSERT_CRASH(dbConn->BindCol(1, SQL_C_LONG, sizeof(outId), &outId, &outIdLen));

		int32 outGold = 0;
		SQLLEN outGoldLen = 0;
		ASSERT_CRASH(dbConn->BindCol(2, SQL_C_LONG, sizeof(outGold), &outGold, &outGoldLen));

		// SQL을 실행한다.
		ASSERT_CRASH(dbConn->Execute(L"SELECT id, gold FROM `Gold` WHERE `gold` = (?)"));

		// 데이터가 있는 동안 반복적으로 가져온다.
		while (dbConn->Fetch())
		{
			cout << "Id : " << outId << " Gold : " << outGold << endl;
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