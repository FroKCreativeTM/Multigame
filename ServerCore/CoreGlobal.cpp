#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "Memory.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"
#include "SendBuffer.h"
#include "GlobalQueue.h"
#include "JobTimer.h"
#include "DBConnector.h"

namespace FrokEngine
{
	ThreadManager* GThreadManager = nullptr;
	Memory* GMemory = nullptr;
	SendBufferManager* GSendBufferManager = nullptr;
	GlobalQueue* GGlobalQueue = nullptr;
	JobTimer* GJobTimer = nullptr;
	DBConnector* GDBConnector = nullptr;

	DeadLockProfiler* GDeadLockProfiler = nullptr;

	class CoreGlobal
	{
	public:
		CoreGlobal()
		{
			GThreadManager = new ThreadManager();
			GMemory = new Memory();
			GSendBufferManager = new SendBufferManager();
			GGlobalQueue = new GlobalQueue();
			GJobTimer = new JobTimer();
			GDBConnector = new DBConnector();

			GDBConnector->Connect("tcp://127.0.0.1:3306", "root", "keAarwrm76*", "gamedb");

			GDeadLockProfiler = new DeadLockProfiler();
			SocketUtils::Init();
		}

		~CoreGlobal()
		{
			delete GThreadManager;
			delete GMemory;
			delete GSendBufferManager;
			delete GGlobalQueue;
			delete GJobTimer;
			delete GDeadLockProfiler;
			delete GDBConnector;
			SocketUtils::Clear();
		}
	} GCoreGlobal;
}