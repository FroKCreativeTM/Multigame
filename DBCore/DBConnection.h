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
		bool Fetch();	// SELECTó�� ������� �޾ƿ��� ����
		int32 GetRowCount();
		void Unbind();

	public : 
		// ���ڵ��� ������ �Ѱ��� ��찡 ���� �����ٵ�
		// �׷� �κе��� ó���ϴ� �Լ��̴�.
		// �����Ҷ� ����
		bool BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len,
			SQLPOINTER ptr, SQLLEN* index);
		// �����͸� �ܾ�� ��
		// �� �����͸� �޾ƿ� �� ����
		bool BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
		// �� �� �����ΰ�.
		void HandleError(SQLRETURN ret);

	private:
		// ������ ������ ����ϴ� �׷� ����
		SQLHDBC _connection = SQL_NULL_HANDLE;
		// ������ ���Ǵ� ����
		SQLHSTMT _statement = SQL_NULL_HANDLE;
	};
}