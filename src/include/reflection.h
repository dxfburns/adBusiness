/*
 * reflection.h
 *
 *  Created on: Apr 14, 2014
 *      Author: root
 */

#ifndef REFLECTION_H_
#define REFLECTION_H_
#include <map>
#include <boost/smart_ptr.hpp>
using namespace std;
using namespace boost;

namespace adbiz {
	namespace utility {
		class Object {
		};

		template<typename T>
		class ObjectFactory {
		public:
			static shared_ptr<Object> create() {
				return shared_ptr<T>(new T);
			}
		};

		class Class {
		private:
			shared_ptr<Object> (*factory)(void);
			static map<char*, shared_ptr<Class> > class_map;
			char* name;
			Class() {
			}
		public:
			template<typename T>
			static void regist(char* class_name)	{
				if (class_map.count(class_name) == 0) {
					shared_ptr<Class> cls(new Class);
					cls->factory = &ObjectFactory<T>::create;
					cls->name = class_name;
					class_map[class_name] = cls;
				}
			}
			static shared_ptr<Class> for_name(char*);
			shared_ptr<Object> new_instance() const;
			char* get_name() const;
			virtual ~Class() {
			}
		};
	}
}
#endif /* REFLECTION_H_ */
