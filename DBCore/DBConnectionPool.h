#pragma once

#include "DBConnection.h"

namespace FrokEngine 
{
	class DBConnectionPool
	{
	public:
		DBConnectionPool();
		~DBConnectionPool();

		bool						Connect(int32 connectionCount, const WCHAR* connectionString);
		void						Clear();

		// 레퍼런싱 카운팅을 해서 생포인터를 건낼 필요가 없기 때문에 
		// 스마트 포인트 사용 X
		DBConnection*				Pop();
		void						Push(DBConnection* connection);

	private:
		USE_LOCK;
		SQLHENV _environment = SQL_NULL_HANDLE;
		Vector<DBConnection*> _connections;
	};
}