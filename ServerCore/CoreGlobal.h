#pragma once

namespace FrokEngine
{
	extern class ThreadManager* GThreadManager;
	extern class Memory* GMemory;
	extern class SendBufferManager* GSendBufferManager;
	extern class GlobalQueue* GGlobalQueue;
	extern class JobTimer* GJobTimer;
	extern class DBConnector* GDBConnector;

	extern class DeadLockProfiler* GDeadLockProfiler;
}

