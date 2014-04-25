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
#include "include/cache.h"

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

#include "include/business.h"
extern bool adbiz::business::client::init(string&, string&, string&, string&, string&, string&, string&);

void test_client() {
	string client_id("12354243fsf_10957"), alias("北京病人"), track_path("http://biz-clt.adsage.cn");
	string ip("220.231.5.70"), ip_position("北京"), ip_description(""), user_agent("WOW");
	adbiz::business::client::init(client_id, alias, track_path, ip, ip_position, ip_description, user_agent);
}

void regist_classes();
void read_strategy_xml();

int main() {
	//test_redis();
	//test_reflection();
	regist_classes();
	read_strategy_xml();

	adbiz::cache::cache_manager::set_cache_type(mem_redis);
	test_client();
	adbiz::utility::InitLogger(true);
	LOG_INFO("First log test");
	reset_socket_connection_in_db();
	LOG_INFO("socket connections were cleared.");

	run_wspp_server();
}
