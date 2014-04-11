/*
 * websocket.h
 *
 *  Created on: Mar 29, 2014
 *      Author: root
 */

#ifndef WEBSOCKET_H_
#define WEBSOCKET_H_
#include <set>
#include <map>
#include <queue>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/common/thread.hpp>
#include "site_manager.h"
#include "connection_manager.h"
#include "message_processor.h"

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::connection_hdl;
using websocketpp::lib::mutex;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

using namespace std;
using namespace adbiz::business;

namespace adbiz {
	namespace websocket {
		enum action_type {
			SUBSCRIBE, UNSUBSCRIBE, MESSAGE
		};

		struct action {
			action(action_type t, connection_hdl h) :
					type(t), hdl(h) {
			}
			action(action_type t, connection_hdl h, server::message_ptr m) :
					type(t), hdl(h), msg(m) {
			}

			action_type type;
			websocketpp::connection_hdl hdl;
			server::message_ptr msg;
		};

		class wspp_server {
		protected:
			typedef set<connection_hdl> con_list;
			typedef map<int, connection_hdl> con_map;
			server m_server;
			con_list m_connections;
			con_map m_conn_map;
			queue<action> m_actions;

			mutex m_action_lock;
			mutex m_connection_lock;
			condition_variable m_action_cond;
		public:
			wspp_server();
			virtual ~wspp_server() {}
			virtual void run(uint16_t);
			virtual void on_open(connection_hdl&);
			virtual void on_close(connection_hdl&);
			virtual void on_message(connection_hdl&, server::message_ptr&);
			virtual void send_message(const string&, const string&);
			virtual void disconnect_client(const string&);
			virtual void process_messages();

			message_processor* p_mp;
		};

		class wspp_server_client : public wspp_server {
		public:
			wspp_server_client();
		};

		class wspp_server_waiter : public wspp_server {
		public:
			wspp_server_waiter();
		};

		void run_wspp_server();
	}
}

#endif /* WEBSOCKET_H_ */
