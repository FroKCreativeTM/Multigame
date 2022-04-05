#include "pch.h"
#include "DBConnector.h"

namespace FrokEngine
{
	DBConnector::DBConnector()
	{
	}

	DBConnector::~DBConnector()
	{
		Close();
	}

	bool DBConnector::Connect(const string& Server, const string& User, const string& Password,
		const string& Database)
	{
		_driver = get_driver_instance();
		_con = _driver->connect(Server.c_str(), User.c_str(), Password.c_str());

		if (_con != nullptr)
			cout << "connection complete" << endl;
		else
			return false;

		_con->setSchema(Database.c_str());
		_stmt = _con->createStatement();

		return true;
	}

	void DBConnector::Close()
	{
		if (_res)
		{
			delete _res;
		}
		_res = nullptr;

		if (_stmt)
		{
			delete _stmt;
		}
		_stmt = nullptr;

		if (_con)
		{
			delete _con;
		}
		_con = nullptr;
	}

	sql::ResultSet* DBConnector::GetResultSet()
	{
		if (!_res)
			return nullptr;
		return _res;
	}

	sql::Statement* DBConnector::GetStatement()
	{
		if (!_stmt) return nullptr;
		return _stmt;
	}

}