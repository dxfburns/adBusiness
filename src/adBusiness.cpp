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
#include "include/connection_manager.h"
#include "include/socket.h"
#include "include/websocket.h"
#include "include/cache.h"
#include "include/auto_assign.h"

using namespace std;
using namespace boost::asio;
using namespace adbiz::model;
using namespace adbiz::business;
using namespace adbiz::socket;
using namespace adbiz::websocket;
using namespace adbiz::business::auto_assign;

void reset_socket_connection_in_db() {
	connection_manager::get_client_instance()->remove_connections_by_machine(MACHINE_ID);
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
//	regist_classes();
//	read_strategy_xml();
//	anylizer_processor::execute_client();

//	adbiz::cache::cache_manager::set_cache_type(mem_redis);
//	test_client();
//	adbiz::utility::InitLogger(true);
//	LOG_INFO("First log test");
	adbiz::cache::cache_manager::set_cache_type(mem_redis);
	reset_socket_connection_in_db();
//	LOG_INFO("socket connections were cleared.");

	run_wspp_server();
}
