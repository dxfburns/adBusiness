/*
 * auto_assign.h
 *
 *  Created on: Apr 24, 2014
 *      Author: root
 */

#ifndef AUTO_ASSIGN_H_
#define AUTO_ASSIGN_H_
#include <map>
#include <boost/smart_ptr.hpp>
#include "reflection.h"

using namespace std;
using namespace boost;
using namespace adbiz::utility;

namespace adbiz{
	namespace business {
		namespace auto_assign {
			class processor_base;

			class anylizer {
			private:
				shared_ptr<processor_base> processor;
			public:
				void set_context(map<string, long>&);
				void execute();
			};

			class processor_base {
			public:
				map<string, long> context;
				virtual ~processor_base() {}
				virtual void execute() = 0;
			};

			class processor_last_waiter : public processor_base, public Object {
			public:
				void execute();
			};

			class processor_capcity: public processor_base, public Object {
			public:
				void execute();
			};

			class processor_policy_zone: public processor_base, public Object {
			public:
				void execute();
			};

			class processor_policy_page: public processor_base, public Object {
			public:
				void execute();
			};

			class processor_final: public processor_base, public Object {
			public:
				void execute();
			};

			struct context_key {
				static const string can_next;
				static const string last_waiter;
				static const string candidates;
				static const string final_assigner;
			};
		}
	}
}



#endif /* AUTO_ASSIGN_H_ */
