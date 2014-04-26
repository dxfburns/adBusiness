/*
 * auto_assign.h
 *
 *  Created on: Apr 24, 2014
 *      Author: root
 */

#ifndef AUTO_ASSIGN_H_
#define AUTO_ASSIGN_H_
#include <map>

using namespace std;

namespace adbiz{
	namespace business {
		namespace auto_assign {
			class processor_base;
			struct context;

			template<class T>
			class processor_factory {
			public:
				static processor_base* create();
			};

			class my_class {
			private:
				string name;
				processor_base* (*factory)(void);
				static map<string, my_class*> class_map;
			public:
				template<typename T>
				static void regist(string);
				static my_class* for_name(string);
				processor_base* new_instance();
				string get_name() const;
			};

			class anylizer {
			private:
				processor_base* processor;
			public:
				anylizer(processor_base*);
				void set_context(context*);
				void execute();
			};

			class processor_base {
			public:
				context* p_cxt;
				virtual ~processor_base() {}
				virtual void execute() = 0;
			};

			class processor_last_waiter : public processor_base {
			public:
				void execute();
			};

			class processor_capcity: public processor_base {
			public:
				void execute();
			};

			class processor_policy_zone: public processor_base {
			public:
				void execute();
			};

			class processor_policy_page: public processor_base {
			public:
				void execute();
			};

			class processor_final: public processor_base {
			public:
				void execute();
			};

			struct context {
				bool can_next;
				string last_waiter;
				vector<string> candidates;
				string final_assigner;
			};

			class anylizer_processor {
			public:
				static void execute_client();
			};
		}
	}
}



#endif /* AUTO_ASSIGN_H_ */
