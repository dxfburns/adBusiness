/*
 * cache.cpp
 *
 *  Created on: Apr 23, 2014
 *      Author: root
 */
#include "../include/cache.h"
#include <iostream>
#include <redis/redisclient.h>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

void adbiz::cache::cache_local::set_client(client& clt) {

}

void adbiz::cache::cache_local::get_client(string& client_id, client& clt) {

}

void adbiz::cache::cache_local::set_conn_mgnt(conn_m& conn) {

}

void adbiz::cache::cache_local::remove_conn_mngt(const string& key, const string& hkey) {

}

void adbiz::cache::cache_local::remove_conn_mngt(const string& key) {

}

redis::client* redis_connect() {
	const char* host = "localhost";
	return new redis::client(host);
}

void adbiz::cache::cache_redis::set_client(client& clt) {
	shared_ptr<redis::client> rc(redis_connect());
	rc->select(0);

	string key(clt.client_id);
	rc->hset(key, "alias", clt.alias);
	rc->hset(key, "last_waiter", clt.last_waiter);
	rc->hset(key, "first_time", lexical_cast<string>(clt.first_time));
	rc->hset(key, "last_time", lexical_cast<string>(clt.last_time));
	rc->hset(key, "session_count", lexical_cast<string>(clt.session_count));
	rc->hset(key, "visit_count", lexical_cast<string>(clt.visit_count));
	rc->hset(key, "ip", clt.ip);
	rc->hset(key, "ip_position", clt.ip_position);
	rc->hset(key, "ip_description", clt.ip_description);
	rc->hset(key, "url", clt.url);
	rc->hset(key, "title", clt.title);
	rc->hset(key, "track_path", clt.track_path);
	rc->hset(key, "user_agent", clt.user_agent);

	rc->expire(key, 240);
}

void adbiz::cache::cache_redis::get_client(string& client_id, client& clt) {
	shared_ptr<redis::client> rc(redis_connect());
	rc->select(0);

	string key = client_id;
	if (!rc->exists(key)) {
		return;
	}

	clt.client_id = client_id;
	clt.alias = rc->hget(key, "alias");
	clt.last_waiter = rc->hget(key, "last_waiter");
	clt.first_time = atoi(rc->hget(key, "first_time").c_str());
	clt.last_time = atoi(rc->hget(key, "last_time").c_str());
	clt.session_count = atoi(rc->hget(key, "session_count").c_str());
	clt.visit_count = atoi(rc->hget(key, "visit_count").c_str());
	clt.ip = rc->hget(key, "ip");
	clt.ip_position = rc->hget(key, "ip_position");
	clt.ip_description = rc->hget(key, "ip_description");
	clt.url = rc->hget(key, "url");
	clt.title = rc->hget(key, "title");
	clt.track_path = rc->hget(key, "track_path");
	clt.user_agent = rc->hget(key, "user_agent");
}

void adbiz::cache::cache_redis::set_conn_mgnt(conn_m& conn) {
	shared_ptr<redis::client> rc(redis_connect());
	rc->select(1);

	string key(conn.client_id);
	rc->hset(key, lexical_cast<string>(conn.conn_address), lexical_cast<string>(conn.machine_id));

}

void adbiz::cache::cache_redis::remove_conn_mngt(const string& key, const string& hkey) {
	shared_ptr<redis::client> rc(redis_connect());
	rc->select(1);

	rc->hdel(key, hkey);
	if(rc->hlen(key) == 0) {
		rc->del(key);
	}
}

void adbiz::cache::cache_redis::remove_conn_mngt(const string& key) {
	shared_ptr<redis::client> rc(redis_connect());
	rc->select(1);

	rc->del(key);
}

shared_ptr<adbiz::cache::cache> adbiz::cache::cache_manager::ch;

void adbiz::cache::cache_manager::set_cache_type(int ct) {
	cache_type type = cache_type(ct);

	switch (type) {
	case mem_local:
		cache_manager::ch = shared_ptr<cache>(new cache_local);
		break;
	case mem_redis:
		cache_manager::ch = shared_ptr<cache>(new cache_redis);
		break;
	}
}

void adbiz::cache::cache_manager::set_client(client& clt) {
	ch->set_client(clt);
}

void adbiz::cache::cache_manager::get_client(string& client_id, client& clt) {
	ch->get_client(client_id, clt);
}

void adbiz::cache::cache_manager::set_conn_mgnt(conn_m& conn) {
	ch->set_conn_mgnt(conn);
}

void adbiz::cache::cache_manager::remove_conn_mngt(const string& key, const string& hkey) {
	ch->remove_conn_mngt(key, hkey);
}

void adbiz::cache::cache_manager::remove_conn_mngt(const string& key) {
	ch->remove_conn_mngt(key);
}

