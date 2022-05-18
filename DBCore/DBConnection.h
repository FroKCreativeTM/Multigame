#pragma once

// FOR ODBC
#include <sql.h>
#include <sqlext.h>

namespace FrokEngine
{
	class DBConnection
	{
	public:
		bool Connect(SQLHDBC henv, const WCHAR* connectionString);
		void Clear();

		bool Execute(const WCHAR* query);
		bool Fetch();	// SELECT처럼 결과물을 받아오는 물건
		int32 GetRowCount();
		void Unbind();

	public : 
		// 인자들을 쿼리에 넘겨줄 경우가 많이 생길텐데
		// 그런 부분들을 처리하는 함수이다.
		// 실행할때 인자
		bool BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len,
			SQLPOINTER ptr, SQLLEN* index);
		// 데이터를 긁어올 때
		// 즉 데이터를 받아올 때 인자
		bool BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
		// 몇 번 에러인가.
		void HandleError(SQLRETURN ret);

	private:
		// 실제로 연결을 담당하는 그런 변수
		SQLHDBC _connection = SQL_NULL_HANDLE;
		// 쿼리에 사용되는 물건
		SQLHSTMT _statement = SQL_NULL_HANDLE;
	};
}