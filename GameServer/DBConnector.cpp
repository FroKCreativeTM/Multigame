#include "pch.h"
#include "DBConnector.h"

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
	driver = get_driver_instance();
	con = driver->connect(Server.c_str(), User.c_str(), Password.c_str());

	if (con != nullptr)
		cout << "connection complete" << endl;
	else
		return false;

	con->setSchema(Database.c_str());
	stmt = con->createStatement();

	return true;
}

void DBConnector::Close()
{
	if (res)
	{
		delete res;
	}
	res = nullptr;

	if (stmt)
	{
		delete stmt;
	}
	stmt = nullptr;

	if (con)
	{
		delete con;
	}
	con = nullptr;
}
