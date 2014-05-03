/*
 * mesage_processor.cpp
 *
 *  Created on: Apr 5, 2014
 *      Author: root
 */
#include "../include/message_processor.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string.hpp>

boost::function<void(const string&, const string&)> func_send_to_client;
boost::function<void(const string&, const string&)> func_send_to_waiter;
boost::function<void(const string&)> func_disconnect_client;
boost::function<void(const string&, const string&)> func_send_to_dispatcher;

namespace adbiz {
	namespace business {
		using namespace boost;
		using namespace boost::property_tree;

		message_processor_client* message_processor_client::p_instance = 0;
		message_processor_waiter* message_processor_waiter::p_instance = 0;
		message_processor_dispatcher* message_processor_dispatcher::p_instance = 0;

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

		void message_processor::set_dispatch_package_from_message(const string& msg, dispatch_package& dis_pack) {
			ptree pt;
			stringstream stream;

			stream << msg;
			read_json(stream, pt);

			string str_to = pt.get<string>("to");
			vector<string> v_to;
			boost::split(v_to, str_to, boost::is_any_of(";"));

			//string str_msg = pt.get<string>("msg");
			package pack;
			//set_package_from_message(str_msg, pack);
			pack.from = pt.get<string>("msg.from");
			pack.to = pt.get<string>("msg.to");
			pack.site_id = atoi(pt.get < string > ("msg.site_id").c_str());
			pack.session_id = atoi(pt.get < string > ("msg.session_id").c_str());
			pack.message_type = (msg_t) (atoi(pt.get < string > ("msg.message_type").c_str()));
			ptree pt1 = pt.get_child("msg.messages");
			queue<message> q;
			for (ptree::iterator iter = pt1.begin(); iter != pt1.end(); iter++) {
				ptree pt2 = iter->second;
				message m;
				m.text = pt2.get < string > ("text");
				m.time = atol(pt2.get < string > ("time").c_str());

				q.push(m);
			}
			pack.messages = q;

			dis_pack.to = v_to;
			dis_pack.msg = pack;
		}

		void message_processor::set_message_from_dispatch_package(dispatch_package& dis_pack, string& msg) {
			ptree pt, pt1, pt2;
			string str_to;
			for (vector<string>::iterator iter = dis_pack.to.begin(); iter != dis_pack.to.end(); iter++) {
				str_to += *iter;
				str_to += ";";
			}

			if(str_to.size() == 0) {
				return;
			}
			str_to = str_to.substr(0, str_to.size() - 1);

			package& pack = dis_pack.msg;
			pt1.put("from", pack.from);
			pt1.put("to", pack.to);
			pt1.put<int>("site_id", pack.site_id);
			pt1.put("session_id", pack.session_id);
			pt1.put<short>("message_type", (short) pack.message_type);
			while (!pack.messages.empty()) {
				message msg = pack.messages.front();
				ptree pt3;
				pt3.put("text", msg.text);
				pt3.put<long>("time", msg.time);

				pt2.push_back(std::make_pair("", pt3));

				pack.messages.pop();
			}
			pt1.put_child("messages", pt2);

			pt.put("to", str_to);
			pt.put_child("msg", pt1);

			ostringstream os;
			write_json(os, pt);

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

			//event listener
			switch (type) {
			case waiter_to_client:
			case client_to_waiter:
				{
					vector<string> v_to;
					v_to.push_back("001@10957");
					v_to.push_back("002@10957");
					dispatch_package dis_pack;
					dis_pack.to = v_to;
					dis_pack.msg = pack;
					string dis_msg;
					set_message_from_dispatch_package(dis_pack, dis_msg);
					func_send_to_dispatcher("", dis_msg);
				}
				break;
			case client_disconnect:
				func_disconnect_client(pack.to);
				break;
			case waiter_connect:
			case client_connect:
				{
					int conn_addr = (int) hdl.lock().get();
					this->update_connection(conn_addr, pack.from, pack.site_id);
					//update account_id, conn_addr, machine_id into redis as key, hkey, hval
				}
				break;
			}
		}
	}
}

