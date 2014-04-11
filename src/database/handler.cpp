/*
 * handler.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: root
 */
#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <prepared_statement.h>
#include "../include/database.h"

using namespace sql;
using namespace std;

Driver * driver = mysql::get_driver_instance();

adbiz::db::common::mysql_handler::mysql_handler() : conn(NULL) {
	this->set_conn();
}

adbiz::db::common::mysql_handler::~mysql_handler() {
	delete this->conn;
}

void adbiz::db::common::mysql_handler::set_conn() {
	ConnectOptionsMap com;
	string hostName("tcp://localhost:3306"), userName("root"), password("111111"), schema("im_service");
	com["hostName"] = hostName;
	com["userName"] = userName;
	com["password"] = password;
	com["schema"] = schema;

	try {
		this->conn = driver->connect(com);
	} catch (exception& ex) {
		cout << ex.what() << endl;
	}
}

ResultSet* adbiz::db::common::mysql_handler::get_ResultSet(const char* sql) {
	auto_ptr < PreparedStatement > ps(conn->prepareStatement(sql));
	return ps->executeQuery();
}

bool adbiz::db::common::mysql_handler::execute(const char* sql) {
	auto_ptr < PreparedStatement > ps(conn->prepareStatement(sql));
	return ps->execute();
}
