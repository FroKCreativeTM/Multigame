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

		// ���۷��� ī������ �ؼ� �������͸� �ǳ� �ʿ䰡 ���� ������ 
		// ����Ʈ ����Ʈ ��� X
		DBConnection*				Pop();
		void						Push(DBConnection* connection);

	private:
		USE_LOCK;
		SQLHENV _environment = SQL_NULL_HANDLE;
		Vector<DBConnection*> _connections;
	};
}