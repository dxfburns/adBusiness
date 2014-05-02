/*
 * message_processor.h
 *
 *  Created on: Apr 7, 2014
 *      Author: root
 */

#ifndef MESSAGE_PROCESSOR_H_
#define MESSAGE_PROCESSOR_H_
#include <vector>
#include <websocketpp/config/asio_no_tls.hpp>
#include "model.h"
#include "connection_manager.h"

using namespace std;
using namespace boost;
using namespace adbiz::model;

using websocketpp::connection_hdl;

namespace adbiz {
	namespace business {

		class message_processor {
		protected:
			connection_manager* p_conn_mgr;
		public:
			virtual ~message_processor() {}
			void set_package_from_message(const string&, package&);
			void set_message_from_package(package&, string&);
			void set_dispatch_package_from_message(const string&, dispatch_package&);
			void set_message_from_dispatch_package(dispatch_package&, string&);
			void init_connection(connection_hdl&);
			void update_connection(int, string&, int);
			void remove_connection(string&);
			void remove_connection(connection_hdl&);
			vector<connection_hdl> get_connections(const string&);
			void message_dispatcher(connection_hdl&, const string&);
		};

		class message_processor_client: public message_processor {
		private:
			static message_processor_client* p_instance;
			message_processor_client() { p_conn_mgr = connection_manager::get_client_instance();}
		public:
			static message_processor_client* get_instance() {
				if (p_instance == 0) {
					static message_processor_client p;
					p_instance = &p;
				}

				return p_instance;
			}
		};

		class message_processor_waiter: public message_processor {
		private:
			static message_processor_waiter* p_instance;
			message_processor_waiter() { p_conn_mgr = connection_manager::get_waiter_instance();}
		public:
			static message_processor_waiter* get_instance() {
				if (p_instance == 0) {
					static message_processor_waiter p;
					p_instance = &p;
				}

				return p_instance;
			}
		};

		class message_processor_dispatcher: public message_processor {
		private:
			static message_processor_dispatcher* p_instance;
			message_processor_dispatcher() { p_conn_mgr = connection_manager::get_dispatcher_instance();}
		public:
			static message_processor_dispatcher* get_instance() {
				if (p_instance == 0) {
					static message_processor_dispatcher p;
					p_instance = &p;
				}

				return p_instance;
			}
		};
	}
}


#endif /* MESSAGE_PROCESSOR_H_ */
