/*
 * site.h
 *
 *  Created on: Apr 1, 2014
 *      Author: root
 */

#ifndef SITE_H_
#define SITE_H_
#include <set>
#include <map>
#include <websocketpp/config/asio_no_tls.hpp>

using namespace std;
using websocketpp::connection_hdl;

namespace adbiz {
	namespace business {
		class site;

		class site_manager {
		private:
			static site_manager* ptr_instance;
			map<int, site*> site_map;
		public:
			static site_manager* get_instance() {
				if (ptr_instance == 0) {
					static site_manager p;
					ptr_instance = &p;
				}

				return ptr_instance;
			}
			void add_site(int);
			void remove_site(int);
			site* get_site(int);
		};

		//site_manager* site_manager::ptr_instance = 0;

		class site {
		private:
			map<string, set<connection_hdl*> > conn_map;
		public:
			site(int id) : site_id(id) {}
			void add_connection(connection_hdl&, const string&);
			void update_connection(connection_hdl&, const string&);
			void remove_connection(const string&);
			void remove_connection(connection_hdl&);
		public:
			int site_id;
		};
	}
}



#endif /* SITE_H_ */
