#pragma once
class DBConnector
{
public:
	DBConnector();
	~DBConnector();

	// DB 에 연결
	bool Connect(
		const string& Server,
		const string& User,
		const string& Password,
		const string& Database
	);
	// DB 연결 종료
	void Close();



private :
	class sql::Driver* driver;
	class sql::Connection* con;
	class sql::Statement* stmt;
	class sql::ResultSet* res;
};

