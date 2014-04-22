/*
 * connection_manager.cpp
 *
 *  Created on: Apr 3, 2014
 *      Author: root
 */
#include <iostream>
#include <boost/format.hpp>
#include "../include/model.h"
#include "../include/database.h"

using namespace adbiz::db::common;

bool adbiz::db::manage::db_connection_manager::add_connection(conn_m& conn) {
	boost::format fmt("insert into connection_manager (conn_addr, site_id, client_id, machine_id) values (%1%, %2%, '%3%', %4%)");
	fmt % conn.conn_address % conn.site_id % conn.client_id % conn.machine_id;

	mysql_handler handler;
	return handler.execute(fmt.str().data());
}

bool adbiz::db::manage::db_connection_manager::update_connection(conn_m& conn) {
	boost::format fmt("update connection_manager set site_id = %1%, client_id = '%2%' where conn_addr = %3% and machine_id = %4%");
	fmt % conn.site_id % conn.client_id % conn.conn_address % MACHINE_ID;

	mysql_handler handler;
	return handler.execute(fmt.str().data());
}

bool adbiz::db::manage::db_connection_manager::remove_connection_by_address(const int conn_addr) {
	boost::format fmt("delete from connection_manager where conn_addr = %1% and machine_id = %2%");
	fmt % conn_addr % MACHINE_ID;

	mysql_handler handler;
	return handler.execute(fmt.str().data());
}

bool adbiz::db::manage::db_connection_manager::remove_connection_by_siteid(const int site_id) {
	boost::format fmt("delete from connection_manager where site_id = %1% and machine_id = %4%");
	fmt % site_id % MACHINE_ID;

	mysql_handler handler;
	return handler.execute(fmt.str().data());
}

void adbiz::db::manage::db_connection_manager::remove_connection_by_machineid(const int machine_id) {
	boost::format fmt("delete from connection_manager where machine_id = %1%");
	fmt % machine_id;

	mysql_handler handler;
	handler.execute(fmt.str().data());
}

bool adbiz::db::manage::db_connection_manager::remove_connection_by_clientid(const string& client_id) {
	boost::format fmt("delete from connection_manager where client_id = '%1%'");
	fmt % client_id;

	mysql_handler handler;
	return handler.execute(fmt.str().data());
}

conn_m adbiz::db::manage::db_connection_manager::get_connection_manager(const int conn_addr) {
	mysql_handler handler;
	boost::format fmt("select * from connection_manager where conn_addr = '%1%'");
	fmt % conn_addr;
	auto_ptr<ResultSet> res(handler.get_ResultSet(fmt.str().data()));

	conn_m conn;
	while (res->next()) {
		conn.conn_address = res->getInt("conn_addr");
		conn.site_id = res->getInt("site_id");
		conn.client_id = res->getString("client_id");
		conn.machine_id = res->getInt("machine_id");

		break;
	}

	return conn;
}

set<conn_m> adbiz::db::manage::db_connection_manager::get_connection_managers(const string& client_id) {
	mysql_handler handler;
	boost::format fmt("select * from connection_manager where client_id = '%1%'");
	fmt % client_id;
	auto_ptr<ResultSet> res(handler.get_ResultSet(fmt.str().data()));

	set<conn_m> s_conn;
	while (res->next()) {
		conn_m conn;
		conn.conn_address = res->getInt("conn_addr");
		conn.site_id = res->getInt("site_id");
		conn.client_id = res->getString("client_id");
		conn.machine_id = res->getInt("machine_id");

		s_conn.insert(conn);	//invoke operator <
	}

	return s_conn;
}

