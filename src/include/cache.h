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

using namespace boost;
using namespace adbiz::model;

namespace adbiz {
	namespace cache {
		class cache {
		public:
			virtual ~cache() {}
			virtual void set_client(client&) = 0;
			virtual void get_client(string&, client&) = 0;
		};

		class cache_local : public cache {
		public:
			void set_client(client&);
			void get_client(string&, client&);
		};

		class cache_redis : public cache {
		public:
			void set_client(client&);
			void get_client(string&, client&);
		};

		class cache_manager {
		private:
			static shared_ptr<cache> ch;
		public:
			static void set_cache_type(int);
			static void set_client(client&);
			static void get_client(string&, client&);
		};
	}
}
#endif /* CACHE_H_ */
