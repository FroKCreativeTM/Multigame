#pragma once
namespace FrokEngine
{
	class DBConnector
	{
	public:
		DBConnector();
		~DBConnector();

		// DB �� ����
		bool Connect(
			const string& Server,
			const string& User,
			const string& Password,
			const string& Database
		);
		// DB ���� ����
		void Close();

		class sql::ResultSet* GetResultSet();
		class sql::Statement* GetStatement();

	private:
		class sql::Driver* _driver;
		class sql::Connection* _con;
		class sql::Statement* _stmt;
		class sql::ResultSet* _res;
	};
}