/*
 * connection_manager.h
 *
 *  Created on: Apr 4, 2014
 *      Author: root
 */

#ifndef CONNECTION_MANAGER_H_
#define CONNECTION_MANAGER_H_

#include <map>
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include "../include/model.h"

using namespace std;
using namespace adbiz::model;
using websocketpp::connection_hdl;

namespace adbiz {
	namespace business {
		class connection_manager {
		private:
			static connection_manager* p_client_instance;
			static connection_manager* p_waiter_instance;
			typedef map<int, connection_hdl> con_map;
			typedef map<string, vector<int> > client_conn_map;
			con_map m_connections;
			client_conn_map m_client_connections;
			connection_manager() {};
		public:
			static connection_manager* get_client_instance() {
				if (p_client_instance == 0) {
					static connection_manager p1;
					p_client_instance = &p1;
				}

				return p_client_instance;
			}
			static connection_manager* get_waiter_instance() {
				if (p_waiter_instance == 0) {
					static connection_manager p2;
					p_waiter_instance = &p2;
				}

				return p_waiter_instance;
			}
			void add_connection(conn_m&,const connection_hdl&);
			void update_connection(conn_m&);
			void remove_connection(const connection_hdl&);
			void remove_connection(const string&);
			vector<connection_hdl> get_connections(const string&);
		};
	}
}



#endif /* CONNECTION_MANAGER_H_ */
