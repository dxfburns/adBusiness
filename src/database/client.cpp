/*
 * client.cpp
 *
 *  Created on: Mar 24, 2014
 *      Author: root
 */
#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <prepared_statement.h>
#include <boost/format.hpp>
#include "../include/model.h"
#include "../include/database.h"


using namespace adbiz::db::common;

adbiz::db::manage::db_client::db_client() {
	this->p_client = new client;
}

adbiz::db::manage::db_client::~db_client() {
	delete this->p_client;
}

client* adbiz::db::manage::db_client::get_client(const string& client_id){
	mysql_handler handler;
	boost::format fmt("select * from client where client_id = '%1%'");
	fmt %client_id;
	auto_ptr<ResultSet> res(handler.get_ResultSet(fmt.str().data()));

	while(res->next()) {
		this->p_client->client_id = res->getString("client_id");
		this->p_client->alias = res->getString("alias");

		break;
	}

	return this->p_client;
}

bool adbiz::db::manage::db_client::add_client(const client& clt) {
	boost::format fmt("insert into client (client_id, alias, last_waiter, first_time, last_time, session_count, "
				"visit_count, track_path, ip, ip_position, ip_description, url, title, user_agent) values "
				"(%1%, %2%, '', %3%, %4%, 1, 1, '', %5%, %6%, %7%, '', '', %8%)");
	fmt %clt.client_id %clt.alias %clt.first_time %clt.last_time %clt.ip %clt.ip_position %clt.ip_description %clt.user_agent;
	mysql_handler handler;
	return handler.execute(fmt.str().data());
}

bool adbiz::db::manage::db_client::remove_client(const string& client_id) const {
	boost::format fmt("delete from client where client_id = '%1%'");
	fmt %client_id;
	mysql_handler handler;
	return handler.execute(fmt.str().data());
}
