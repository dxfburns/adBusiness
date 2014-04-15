/*
 * redis.cpp
 *
 *  Created on: Apr 15, 2014
 *      Author: root
 */
#include <iostream>
#include <boost/date_time.hpp>
#include "../redis/redisclient.h"

using namespace std;
using namespace boost;

shared_ptr<redis::client> connect_client() {
	const char* host = "localhost";
	return shared_ptr<redis::client>(new redis::client(host));
}

void test_redis() {
	shared_ptr<redis::client> c(connect_client());
	cout << "use_count = " << c.use_count() << endl;

	c->rpush("mykey", "hello");
	redis::client::string_vector vals;
	long num = c->get_list("mykey", vals);
	for (int i = 0; i < vals.size(); i++) {
		cout << vals[i] << " ";
	}
	cout << endl;

	c->set("mk", "abc123");
	string s(c->get("mk"));
	cout << "s : " << s << endl;
}

