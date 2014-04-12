/*
 * websocket.cpp
 *
 *  Created on: Mar 29, 2014
 *      Author: root
 */
#include <iostream>
#include "../include/websocket.h"
#include "../include/business.h"
#include "../include/utility.h"
#include "../include/message_processor.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using websocketpp::lib::thread;

adbiz::websocket::wspp_server::wspp_server() {
	// Initialize Asio Transport
	m_server.init_asio();

	// Register handler callbacks
	m_server.set_open_handler(bind(&wspp_server::on_open, this, ::_1));
	m_server.set_close_handler(bind(&wspp_server::on_close, this, ::_1));
	m_server.set_message_handler(bind(&wspp_server::on_message, this, ::_1, ::_2));
}

void adbiz::websocket::wspp_server::run(uint16_t port) {
	m_server.listen(port);
	m_server.start_accept();

	try {
		m_server.run();
	} catch (const std::exception& e) {
		cout << e.what() << endl;
	} catch (websocketpp::lib::error_code& e) {
		cout << e.message() << endl;
	} catch (...) {
		cout << "other exception" << endl;
	}
}

void adbiz::websocket::wspp_server::on_open(connection_hdl& hdl) {
	p_mp->init_connection(hdl);
}

void adbiz::websocket::wspp_server::on_close(connection_hdl& hdl) {
	p_mp->remove_connection(hdl);
}

void adbiz::websocket::wspp_server::process_messages() {
	while (true) {
		unique_lock<mutex> lock(m_action_lock);

		while (m_actions.empty()) {
			m_action_cond.wait(lock);
		}

		action a = m_actions.front();
		m_actions.pop();

		lock.unlock();
	}
}

void adbiz::websocket::wspp_server::on_message(connection_hdl& hdl, server::message_ptr& msg) {
	p_mp->message_dispatcher(hdl, msg->get_payload());
}

void adbiz::websocket::wspp_server::send_message(const string& to, const string& msg) {
	websocketpp::frame::opcode::value ev = websocketpp::frame::opcode::text;
	vector<connection_hdl> hdls = p_mp->get_connections(to);
	for (vector<connection_hdl>::iterator iter = hdls.begin(); iter != hdls.end(); iter++) {
		m_server.send(*iter, msg, ev);
	}
}

void adbiz::websocket::wspp_server::disconnect_client(const string& client_id) {
	vector<connection_hdl> hdls = p_mp->get_connections(client_id);
	for (vector<connection_hdl>::iterator iter = hdls.begin(); iter != hdls.end(); iter++) {
		m_server.close(*iter, 1000, "Server closed.");
	}

// if need check, should uncomment the code below
//	connection_manager* p_cm = connection_manager::get_client_instance();
//	p_cm->remove_connection(client_id);
}

adbiz::websocket::wspp_server_client::wspp_server_client() {
	p_mp = message_processor_client::get_instance();
}

adbiz::websocket::wspp_server_waiter::wspp_server_waiter() {
	p_mp = message_processor_waiter::get_instance();
}

extern boost::function<void(const string&, const string&)> func_send_to_client;
extern boost::function<void(const string&, const string&)> func_send_to_waiter;
extern boost::function<void(const string&)> func_disconnect_client;

void adbiz::websocket::run_wspp_server() {
	try {
		shared_ptr<wspp_server> server_client(new wspp_server_client);
		shared_ptr<wspp_server> server_waiter(new wspp_server_waiter);

		func_send_to_client = boost::bind(&wspp_server::send_message, server_client, ::_1, ::_2);
		func_send_to_waiter = boost::bind(&wspp_server::send_message, server_waiter, ::_1, ::_2);
		func_disconnect_client = boost::bind(&wspp_server::disconnect_client, server_client, ::_1);

		thread t1(bind(&wspp_server::process_messages, server_client));
		thread t2(bind(&wspp_server::run, server_waiter, 6689)); //if can use async?
		thread t3(bind(&wspp_server::process_messages, server_waiter));
		server_client->run(6688);
		t1.join();
	} catch (std::exception & e) {
		cout << e.what() << endl;
	}
}

