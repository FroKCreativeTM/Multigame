#include "pch.h"
#include "DBConnection.h"

namespace FrokEngine
{
	bool DBConnection::Connect(SQLHDBC henv, const WCHAR* connectionString)
	{
		// 진짜 Connection을 만들자.
		if (::SQLAllocHandle(SQL_HANDLE_DBC, henv, &_connection) != SQL_SUCCESS)
			return false;

		WCHAR stringBuffer[MAX_PATH] = { 0 };
		::wcscpy_s(stringBuffer, connectionString);

		WCHAR resultString[MAX_PATH] = { 0 };
		SQLSMALLINT resultStringLen = 0;

		SQLRETURN ret = ::SQLDriverConnectW(_connection, nullptr, reinterpret_cast<SQLWCHAR*>(stringBuffer), _countof(stringBuffer),
			OUT reinterpret_cast<SQLWCHAR*>(resultString), _countof(resultString), 
			OUT & resultStringLen, SQL_DRIVER_NOPROMPT);

		if (::SQLAllocHandle(SQL_HANDLE_STMT, _connection, &_statement) != SQL_SUCCESS)
			return false;

		return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
	}

	void DBConnection::Clear()
	{
		if (_connection != SQL_NULL_HANDLE)
		{
			::SQLFreeHandle(SQL_HANDLE_DBC, _connection);
			_connection = SQL_NULL_HANDLE;
		}

		if (_statement != SQL_NULL_HANDLE)
		{
			::SQLFreeHandle(SQL_HANDLE_STMT, _statement);
			_statement = SQL_NULL_HANDLE;
		}
	}

	bool DBConnection::Execute(const WCHAR* query)
	{
		SQLRETURN ret = ::SQLExecDirectW(_statement, (SQLWCHAR*)query, SQL_NTSL);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			return true;

		HandleError(ret);
		return false;
	}

	bool DBConnection::Fetch()
	{
		// 받을 게 있다면
		SQLRETURN ret = SQLFetch(_statement);

		switch (ret)
		{
		case SQL_SUCCESS : 
		case SQL_SUCCESS_WITH_INFO : 
			return true;
		// 받을 데이터가 없다면
		case SQL_NO_DATA : 
			return false;
		case SQL_ERROR : 
			HandleError(ret);
			return false;
		default : 
			return true;
		}
	}

	int32 DBConnection::GetRowCount()
	{
		SQLLEN count = 0;
		SQLRETURN ret = ::SQLRowCount(_statement, OUT & count);

		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			return static_cast<int32>(count);

		return -1;
	}

	void DBConnection::Unbind()
	{
		::SQLFreeStmt(_statement, SQL_UNBIND);
		::SQLFreeStmt(_statement, SQL_RESET_PARAMS);
		::SQLFreeStmt(_statement, SQL_CLOSE);
	}

	bool DBConnection::BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, 
		SQLULEN len, SQLPOINTER ptr, SQLLEN* index)
	{
		SQLRETURN ret = ::SQLBindParameter(_statement, paramIndex, SQL_PARAM_INPUT,
			cType, sqlType, len, 0, ptr, 0, index);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			HandleError(ret);
			return false;
		}
		return true;
	}

	bool DBConnection::BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index)
	{
		SQLRETURN ret = ::SQLBindCol(_statement, columnIndex, cType, value, len, index);

		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			HandleError(ret);
			return false;
		}
		return true;
	}

	void DBConnection::HandleError(SQLRETURN ret)
	{
		if (ret == SQL_SUCCESS)
			return;

		// 에러가 여러개일 수 있다.
		SQLSMALLINT index = 1;
		SQLWCHAR sqlState[MAX_PATH] = { 0 };
		SQLINTEGER nativeErr = 0;
		SQLWCHAR errMsg[MAX_PATH] = { 0 };
		SQLSMALLINT msgLen = 0;
		SQLRETURN errRet = 0;

		while (true)
		{
			errRet = ::SQLGetDiagRecW(SQL_HANDLE_STMT, _statement, index, sqlState, OUT& nativeErr,
				errMsg, _countof(errMsg), OUT & msgLen);

			if (errRet == SQL_NO_DATA)
				break;

			if (errRet != SQL_SUCCESS && errRet != SQL_SUCCESS_WITH_INFO)
				break;

			// TODO : 로그를 파일에
			wcout.imbue(locale("kor"));
			wcout << errMsg << endl;

			index++;
		}
	}
}