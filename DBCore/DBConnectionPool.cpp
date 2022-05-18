#include "pch.h"
#include "DBConnectionPool.h"

namespace FrokEngine
{
	DBConnectionPool::DBConnectionPool()
	{
	}

	DBConnectionPool::~DBConnectionPool()
	{
		// 모두 정리하고 끝낸다.
		Clear();
	}

	bool DBConnectionPool::Connect(int32 connectionCount, const WCHAR* connectionString)
	{
		// 먼저 락을 걸고 시작합니다.
		WRITE_LOCK;

		// 어떤 사유로 할당하는가를 정해야한다.
		if (::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_environment) != SQL_SUCCESS) 
		{
			return false;
		}

		// SQL의 환경을 설정합니다.
		if (::SQLSetEnvAttr(_environment, SQL_ATTR_ODBC_VERSION, reinterpret_cast<SQLPOINTER>(SQL_OV_ODBC3), 0) != SQL_SUCCESS)
		{
			return false;
		}

		// 연결된 카운트만큼 스레드를 만들어서 사용한다.
		for (int32 i = 0; i < connectionCount; i++)
		{
			// 일단 메모리를 할당한다.
			DBConnection* connection = xnew<DBConnection>();

			// DB에 환경에 맞게 연결하자.
			if (connection->Connect(_environment, connectionString) == false)
				return false;

			_connections.push_back(connection);
		}

		return true;
	}

	void DBConnectionPool::Clear()
	{
		WRITE_LOCK;

		if (_environment != SQL_NULL_HANDLE)
		{
			// 환경에 맞게 핸들을 놓는다.
			::SQLFreeHandle(SQL_HANDLE_ENV, _environment);
			_environment = SQL_NULL_HANDLE;
		}

		for (auto connection : _connections)
		{
			xdelete(connection);
		}

		_connections.clear();
	}

	DBConnection* DBConnectionPool::Pop()
	{
		WRITE_LOCK;

		if (_connections.empty())
		{
			return nullptr;
		}

		DBConnection* connection = _connections.back();
		_connections.pop_back();
		return connection;
	}

	void DBConnectionPool::Push(DBConnection* connection)
	{
		WRITE_LOCK;
		_connections.push_back(connection);
	}

}