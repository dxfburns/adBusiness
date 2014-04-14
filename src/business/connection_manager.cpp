/*
 * connection_manager.cpp
 *
 *  Created on: Apr 4, 2014
 *      Author: root
 */

#include "../include/database.h"
#include "../include/connection_manager.h"
#include <algorithm>

using namespace adbiz::db::manage;

adbiz::business::connection_manager* adbiz::business::connection_manager::p_client_instance = 0;
adbiz::business::connection_manager* adbiz::business::connection_manager::p_waiter_instance = 0;

void adbiz::business::connection_manager::add_connection(conn_m& cm, const connection_hdl& hdl) {
	const int conn_addr = (int) hdl.lock().get();
	m_connections.insert(make_pair<int, connection_hdl>(conn_addr, hdl));

	db_connection_manager dbcm;
	dbcm.add_connection(cm);
}

void adbiz::business::connection_manager::update_connection(conn_m& cm) {
	bool has_addr = false;

	if (m_client_connections.count(cm.client_id) == 0) {
		vector<int> v;
		v.push_back(cm.conn_address);
		m_client_connections[cm.client_id] = v;
	} else {
		vector<int>& v = m_client_connections[cm.client_id];
		int num = std::count(v.begin(), v.end(), cm.conn_address);
		has_addr = (num > 0);

		if (!has_addr) {
			v.push_back(cm.conn_address);
		}
	}

	if (!has_addr) {
		db_connection_manager dbcm;
		dbcm.update_connection(cm);
	}
}

void adbiz::business::connection_manager::remove_connection(const connection_hdl& hdl) {
	const int conn_addr = (int) hdl.lock().get();
	m_connections.erase(conn_addr);

	db_connection_manager dbcm;
	conn_m cm = dbcm.get_connection_manager(conn_addr);
	if (cm.client_id.compare("") == 0) {
		return;
	} else {
		dbcm.remove_connection_by_address(conn_addr);
	}

	if (m_client_connections.count(cm.client_id) > 0) {
		vector<int>& v = m_client_connections[cm.client_id];
		if (v.size() > 1) {
			vector<int>::iterator end = std::remove(v.begin(), v.end(), conn_addr);
			v.erase(end, v.end());
		} else {
			m_client_connections.erase(cm.client_id);
		}
	}
}

void adbiz::business::connection_manager::remove_connection(const string& client_id) {
	client_conn_map::iterator iter = m_client_connections.find(client_id);
	if (iter != m_client_connections.end()) {
		vector<int>::iterator it;
		for (it = m_client_connections[client_id].begin(); it != m_client_connections[client_id].end(); it++) {
			m_connections.erase(*it);
		}

		m_client_connections.erase(client_id);

		db_connection_manager dbcm;
		dbcm.remove_connection_by_clientid(client_id);
	}
}

vector<connection_hdl> adbiz::business::connection_manager::get_connections(const string& client_id) {
	vector<connection_hdl> v_conn;
	client_conn_map::iterator iter = m_client_connections.find(client_id);
	if (iter != m_client_connections.end()) {
		vector<int>::iterator it;
		for (it = m_client_connections[client_id].begin(); it != m_client_connections[client_id].end(); it++) {
			v_conn.push_back(m_connections[*it]);
		}
	}

	return v_conn;
}
