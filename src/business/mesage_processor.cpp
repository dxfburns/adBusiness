/*
 * mesage_processor.cpp
 *
 *  Created on: Apr 5, 2014
 *      Author: root
 */
#include "../include/message_processor.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

boost::function<void(const string&, const string&)> func_send_to_client;
boost::function<void(const string&, const string&)> func_send_to_waiter;
boost::function<void(const string&)> func_disconnect_client;

namespace adbiz {
	namespace business {
		using namespace boost;
		using namespace boost::property_tree;

		message_processor_client* message_processor_client::p_instance = 0;
		message_processor_waiter* message_processor_waiter::p_instance = 0;

		void message_processor::set_package_from_message(const string& msg, package& pack) {
			ptree pt, pt1;
			stringstream stream;

			stream << msg;
			boost::property_tree::read_json(stream, pt);
			pack.from = pt.get < string > ("from");
			pack.to = pt.get < string > ("to");
			pack.site_id = atoi(pt.get < string > ("site_id").c_str());
			pack.session_id = atoi(pt.get < string > ("session_id").c_str());
			pack.message_type = (msg_t) (atoi(pt.get < string > ("message_type").c_str()));

			pt1 = pt.get_child("messages");
			queue<message> q;
			for (ptree::iterator iter = pt1.begin(); iter != pt1.end(); iter++) {
				ptree pt2 = iter->second;
				message msg;
				msg.text = pt2.get < string > ("text");
				msg.time = atol(pt2.get < string > ("time").c_str());

				q.push(msg);
			}
			pack.messages = q;
		}

		void message_processor::set_message_from_package(package& pack, string& msg) {
			ptree pt, pt1;
			pt.put("from", pack.from);
			pt.put("to", pack.to);
			pt.put<int>("site_id", pack.site_id);
			pt.put("session_id", pack.session_id);
			pt.put<short>("message_type", (short) pack.message_type);
			while (!pack.messages.empty()) {
				message msg = pack.messages.front();
				ptree pt2;
				pt2.put("text", msg.text);
				pt2.put<long>("time", msg.time);

				pt1.push_back(std::make_pair("", pt2));

				pack.messages.pop();
			}

			pt.put_child("messages", pt1);

			ostringstream os;
			boost::property_tree::write_json(os, pt);

			msg = os.str();
		}

		void message_processor::init_connection(connection_hdl& hdl) {
			conn_m cm;
			cm.conn_address = (int) hdl.lock().get();

			p_conn_mgr->add_connection(cm, hdl);
		}

		void message_processor::update_connection(int conn_addr, string& client_id, int site_id) {
			conn_m cm;
			cm.conn_address = conn_addr;
			cm.client_id = client_id;
			cm.site_id = site_id;

			p_conn_mgr->update_connection(cm);
		}

		void message_processor::remove_connection(string& client_id) {
			p_conn_mgr->remove_connection(client_id);
		}

		void message_processor::remove_connection(connection_hdl& hdl) {
			p_conn_mgr->remove_connection(hdl);
		}

		vector<connection_hdl> message_processor::get_connections(const string& client_id) {
			return p_conn_mgr->get_connections(client_id);
		}

		void message_processor::message_dispatcher(connection_hdl& hdl, const string& msg) {
			package pack;
			this->set_package_from_message(msg, pack);
			msg_t type(pack.message_type);

			switch (type) {
			case waiter_to_client:
				func_send_to_client(pack.to, msg);
				break;
			case client_to_waiter:
				func_send_to_waiter(pack.to, msg);
				break;
			case client_disconnect:
				func_disconnect_client(pack.to);
				break;
			case waiter_connect:
			case client_connect:
				int conn_addr = (int) hdl.lock().get();
				this->update_connection(conn_addr, pack.from, pack.site_id);
				break;
			}
		}
	}
}

