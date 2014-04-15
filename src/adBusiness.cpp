//============================================================================
// Name        : adBusiness.cpp
// Author      : Xuefeng Du
// Version     :
// Copyright   : adSage
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <boost/asio.hpp>
#include "include/log.h"
#include "include/model.h"
#include "include/database.h"
#include "include/socket.h"
#include "include/websocket.h"

using namespace std;
using namespace boost::asio;
using namespace adbiz::model;
using namespace adbiz::db::manage;
using namespace adbiz::socket;
using namespace adbiz::websocket;

void reset_socket_connection_in_db() {
	adbiz::db::manage::db_connection_manager::db_connection_manager().remove_connection_by_machineid(MACHINE_ID);
}

void test_reflection();
void test_redis();

int main() {
	test_redis();
	//test_reflection();
	adbiz::utility::InitLogger(true);
	LOG_INFO("First log test");
	reset_socket_connection_in_db();
	LOG_INFO("socket connections were cleared.");

	run_wspp_server();
}
