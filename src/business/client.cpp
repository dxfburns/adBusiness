/*
 * client.cpp
 *
 *  Created on: Mar 25, 2014
 *      Author: root
 */
#include <iostream>
#include <vector>
#include "../include/model.h"
#include "../include/database.h"
#include "../include/business.h"
#include "../include/utility.h"

using namespace std;
using namespace adbiz::model;
using namespace adbiz::db::manage;
using namespace adbiz::utility;

bool adbiz::business::client::init(string& client_id, string& alias, string& track_path, string& ip, string& ip_position, string& ip_description,
		string& user_agent) {
	int current_time = get_time_stamp();

	adbiz::model::client clt;
	clt.client_id = client_id;
	clt.alias = alias;
	clt.first_time = current_time;
	clt.last_time = current_time;
	clt.track_path = track_path;
	clt.ip = ip;
	clt.ip_position = ip_position;
	clt.ip_description = ip_description;
	clt.user_agent = user_agent;

	db_client dbc;
	bool res = dbc.add_client(clt);

	return res;
}

bool adbiz::business::client::remove(string& client_id) {
	db_client dbc;
	bool res = dbc.remove_client(client_id);

	return res;
}

void adbiz::business::client::process_message(const string& json_msg) {
	vector<people> v(get_people_from_json(json_msg));
	vector<people>::iterator iter;
	for (iter = v.begin(); iter != v.end(); iter++) {
		people p(*iter);
		cout << p.firstName << "," << p.lastName << "," << p.email << endl;
	}

	string json;
	adbiz::utility::set_people_to_json(&v, json);
	cout << json << endl;
}

void adbiz::business::client::set_package(string& json) {
	package pack;
	pack.from = "1919990403A1429940393D2000B15I26Z14.21994_10957";
	pack.to = "001@10957";
	pack.site_id = 10957;
	pack.session_id = adbiz::utility::gen_uuid();
	pack.message_type = waiter_to_client;
	queue<message> msgs;
	message msg;
	msg.text = "Hello, I am Xuefeng Du";
	msg.time = adbiz::utility::get_time_stamp();
	msgs.push(msg);
	msg.text = "Can I help you?";
	msg.time = adbiz::utility::get_time_stamp();
	msgs.push(msg);
	pack.messages = msgs;

	adbiz::utility::package_to_message(pack, json);
	cout << json << endl;
}
