/*
 * redis.cpp
 *
 *  Created on: Apr 15, 2014
 *      Author: root
 */
#include <iostream>
#include <boost/date_time.hpp>
#include <boost/lexical_cast.hpp>
#include <redis/redisclient.h>

using namespace std;
using namespace boost;

shared_ptr<redis::client> connect_client() {
	const char* host = "localhost";
	return shared_ptr<redis::client>(new redis::client(host));
}

void basic_test(shared_ptr<redis::client>&);
void hash_test(shared_ptr<redis::client>&);
void list_test(shared_ptr<redis::client>&);

void test_redis() {
	shared_ptr<redis::client> c(connect_client());
	list_test(c);
	hash_test(c);
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

void hash_test(shared_ptr<redis::client>& c) {
	c->flushall();
	c->select(14);

	//hset
	c->hset("hash1", "key1", "val1");
	c->hset("hash1", "key2", "val2");
	c->hset("hash1", "key3", "val3");
	assert(c->exists("hash1") == true);
	assert(c->type("hash1") == redis::client::datatype_hash);
	assert(c->hlen("hash1") == 3L);

	//hget
	assert(c->hget("hash1", "key1") == "val1");

	//hsetnx
	assert(c->hsetnx("hash1", "key1", "val1_invalid") == false);
	assert(c->hget("hash1", "key1") == "val1");
	assert(c->hsetnx("hash1", "key4", "val4") == true);
	assert(c->hget("hash1", "key4") == "val4");

	redis::client::string_pair_vector content;
	//hmset
	content.push_back(make_pair("key1", "val1"));
	content.push_back(make_pair("key2", "val2"));
	content.push_back(make_pair("key3", "val3"));
	c->hmset("hash2", content);

	//hmget
	redis::client::string_vector fields;
	fields.push_back("key1");
	fields.push_back("key2");
	fields.push_back("key3");
	fields.push_back("key4");

	redis::client::string_vector values;
	c->hmget("hash2", fields, values);

	for (size_t i = 0; i < fields.size(); i++) {
		if (content.size() > i) {
			assert(values[i] == content[i].second);
		} else {
			assert(values[i] == redis::client::missing_value());
		}
	}

	//hincrby
	c->hset("hash3", "key1", "1");
	long l;
	l = c->hincrby("hash3", "key1", 1);
	assert(l == 2L);
	l = c->hincrby("hash3", "key2", 1);
	assert(l == 1L);
	l = c->hincrby("hash3", "key3", -3);
	assert(l == -3L);

	string s;
	s = c->hget("hash3", "key1");
	assert(s == "2");
	s = c->hget("hash3", "key2");
	assert(s == "1");

	//hexists
	assert(c->hexists("hash3", "key1"));
	assert(!c->hexists("hash3", "key4"));

	//hdel
	assert(c->hdel("hash3", "key1"));
	assert(!c->hexists("hash3", "key1"));
	assert(!c->hdel("hash3", "key4"));
	assert(!c->hexists("hash3", "key4"));

	c->hset("hash3", "key1", "val1");

	//hlen
	assert(c->hlen("hash3") == 3);
	assert(c->hlen("hash4") == 0);

	//hkeys
	fields.clear();
	c->hkeys("hash3", fields);
	assert(fields.size() == 3);
	std::sort(fields.begin(), fields.end());
	assert(fields[0] == "key1");
	assert(fields[1] == "key2");
	assert(fields[2] == "key3");

	values.clear();
	c->hvals("hash1", values);
	assert(values.size() == 4);
	std::sort(values.begin(), values.end());
	for (size_t i = 0; i < values.size(); i++) {
		assert(values[i] == "val" + lexical_cast<string>(i + 1));
	}

	//hgetall
	redis::client::string_pair_vector entries;
	c->hgetall("hash1", entries);
	std::sort(entries.begin(), entries.end());
	for (size_t i = 0; i < entries.size(); i++) {
		assert(entries[i].first == "key" + lexical_cast<string>(i + 1));
		assert(entries[i].second == "val" + lexical_cast<string>(i + 1));
	}
}

void list_test(shared_ptr<redis::client>& c) {
	c->flushall();
	c->select(15);

	//rpush
	assert(c->exists("list1") == false);
	c->rpush("list1", "val1");
	assert(c->llen("list1") == 1);
	assert(c->type("list1") == redis::client::datatype_list);
	c->rpush("list1", "val2");
	assert(c->lindex("list1", 0) == "val1");
	assert(c->lindex("list1", 1) == "val2");

	//lpush
	c->del("list1");
	c->lpush("list1", "val1");
	c->lpush("list1", "val2");
	assert(c->lindex("list1", 0) == "val2");
	assert(c->lindex("list1", 1) == "val1");

	//lrange
	redis::client::string_vector vals;
	assert(c->lrange("list1", 0, -1, vals) == 2);
	for (size_t i = 0; i < vals.size(); i++) {
		assert(vals[i] == "val" + lexical_cast<string>(vals.size() - i));
	}

	//get_list, equals lrange list1 0 -1
	vals.clear();
	assert(c->get_list("list1", vals) == 2);

	//ltrim
	c->ltrim("list1", 0, 0);
	assert(c->llen("list1") == 1);

	//lset
	c->rpush("list1", "g");
	c->lset("list1", 1, "z");
	assert(c->lindex("list1", 1) == "z");

	//lrem
	c->lrem("list1", 1, "z");

	//lpop
	c->lpop("list1");
	assert(c->llen("list1") == 0);

	//rpop
	c->rpush("list1", "hi");
	c->rpush("list1", "ok");
	assert(c->rpop("list1") == "ok");
	assert(c->rpop("list1") == "hi");
	assert(c->rpop("list1") == redis::client::missing_value());
}
