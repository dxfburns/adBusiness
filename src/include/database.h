/*
 * database.h
 *
 *  Created on: Mar 23, 2014
 *      Author: root
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <set>
#include <vector>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <prepared_statement.h>

using namespace sql;
using namespace std;


namespace adbiz {
	namespace db {
		namespace common {
			class mysql_handler {
			private:
				Connection* conn;
				void set_conn();
			public:
				mysql_handler();
				~mysql_handler();
				ResultSet* get_ResultSet(const char*);
				bool execute(const char*);
			};
		}
	}
}

#include "model.h"
using namespace adbiz::model;

namespace adbiz {
	namespace db {
	namespace manage {

		class db_client {
		private:
			client* p_client;
		public:
			db_client();
			~db_client();
			client* get_client(const string&);
			bool add_client(const client&);
			bool remove_client(const string&) const;
		};

		class db_connection_manager {
		public:
			bool add_connection(conn_m&);
			bool update_connection(conn_m&);
			bool remove_connection_by_address(const int);
			bool remove_connection_by_siteid(const int);
			void remove_connection_by_machineid(const int);
			bool remove_connection_by_clientid(const string&);
			conn_m get_connection_manager(const int);
			set<conn_m> get_connection_managers(const string&);
			vector<string> get_client_by_machine(const int);
		};
}}}
#endif /* DATABASE_H_ */
