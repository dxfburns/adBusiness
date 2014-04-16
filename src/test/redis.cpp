/*
 * redis.cpp
 *
 *  Created on: Apr 15, 2014
 *      Author: root
 */
#include <iostream>
#include <boost/date_time.hpp>
#include <redis/redisclient.h>

using namespace std;
using namespace boost;

shared_ptr<redis::client> connect_client() {
	const char* host = "localhost";
	return shared_ptr<redis::client>(new redis::client(host));
}

void basic_test(shared_ptr<redis::client>&);

void test_redis() {
	shared_ptr<redis::client> c(connect_client());
	basic_test(c);
}

void basic_test(shared_ptr<redis::client>& c) {
	c->flushall();	//clear all dbs

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

	try {
		c->select(14);
		c->flushdb();	//clear current db
		assert(c->dbsize() == (redis::client::int_type )0);

		string foo("foo"), bar("bar"), baz("baz"), buz("buz"), goo("goo");

		//binary save values
		int repeations = 3;
		string bin;
		for (int i = 0; i < repeations; i++) {
			for (int j = 0; j <= 255; j++) {
				bin += (char) j;
			}
		}
		c->set("binary", bin);
		string response = c->get("binary");
		assert(response.size() == (size_t )repeations * 256);

		c->append("binary", bin);
		assert(c->get("binary") == bin + bin);
		string half = c->substr("binary", bin.size(), -1);
		assert(half == bin);

		//getset
		c->set(foo, bar);
		cout << c->getset(foo, baz) << endl;

		//mget
		string x_val("hello"), y_val("world");
		c->set("x", x_val);
		c->set("y", y_val);
		redis::client::string_vector keys;
		keys.push_back("x");
		keys.push_back("y");
		redis::client::string_vector vals;
		c->mget(keys, vals);
		assert(vals[0] == x_val);

		//setnx (set Not eXist)
		assert(c->setnx(foo, bar) == false);
		assert(c->setnx(buz, baz) == true);
		assert(c->get(buz) == baz);

		//incr & decr
		c->incr(goo);	//0->1
		c->decr(goo);	//1->0
		c->incrby(goo, 3);	//0->3
		assert(c->get(goo) == "3");

		//exists
		assert(c->exists(goo) == true);

		//del
		c->del(goo);
		assert(c->exists(goo) == false);

		//type
		assert(c->type(goo) == redis::client::datatype_none);
		c->set(goo, "redis");
		assert(c->type(goo) == redis::client::datatype_string);
		c->rpush("mykey", "hello");
		c->lpush("mykey", "redis");
		assert(c->type("mykey") == redis::client::datatype_list);

		//keys
		keys.clear();
		assert(c->keys("*oo", keys) == 2L);
		assert(keys.size() == (size_t )2);
		assert(keys[0] == foo);

		//rename
		if (c->exists(foo) && !c->exists("doo")) {
			c->rename(foo, "doo");
			assert(c->exists(foo) == false);
			assert(c->exists("doo") == true);
		}

		//renamenx
		if (!c->exists(foo) && c->exists("doo")) {
			c->renamenx("doo", foo);
			assert(c->exists(foo) == true);
			assert(c->exists("doo") == false);
		}

		//expire
		c->expire(goo, 1);
		cerr << "please wait a few seconds.." << endl;
		sleep(2);
		assert(c->exists(goo) == false);

		//move
		c->select(15);
		assert(c->exists("ttt") == false);
		c->select(14);
		c->set("ttt", "uuu");
		c->move("ttt", 15);
		assert(c->exists("ttt") == false);
		c->select(15);
		assert(c->exists("ttt") == true);

	} catch (redis::redis_error& e) {
		cout << "Redis Error : " << e.what() << endl;
	}
}

