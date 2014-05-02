/*
 * cache.h
 *
 *  Created on: Apr 23, 2014
 *      Author: root
 */

#ifndef CACHE_H_
#define CACHE_H_
#include "model.h"
#include <boost/smart_ptr.hpp>
#include "connection_manager.h"

using namespace boost;
using namespace adbiz::model;

namespace adbiz {
	namespace cache {
		class cache {
		public:
			virtual ~cache() {}
			virtual void set_client(client&) = 0;
			virtual void get_client(string&, client&) = 0;
			virtual void set_conn_mgnt(conn_m&) = 0;
			virtual void remove_conn_mngt(const string&, const string&) = 0;
			virtual void remove_conn_mngt(const string&) = 0;
		};

		class cache_local : public cache {
		public:
			void set_client(client&);
			void get_client(string&, client&);
			void set_conn_mgnt(conn_m&);
			void remove_conn_mngt(const string&, const string&);
			void remove_conn_mngt(const string&);
		};

		class cache_redis : public cache {
		public:
			void set_client(client&);
			void get_client(string&, client&);
			void set_conn_mgnt(conn_m&);
			void remove_conn_mngt(const string&, const string&);
			void remove_conn_mngt(const string&);
		};

		class cache_manager {
		private:
			static shared_ptr<cache> ch;
		public:
			static void set_cache_type(int);
			static void set_client(client&);
			static void get_client(string&, client&);
			static void set_conn_mgnt(conn_m&);
			static void remove_conn_mngt(const string&, const string&);
			static void remove_conn_mngt(const string&);
		};
	}
}
#endif /* CACHE_H_ */
