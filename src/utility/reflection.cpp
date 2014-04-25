/*
 * reflection.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: root
 */
#include "../include/reflection.h"
using namespace adbiz::utility;

map<char*, shared_ptr<Class> > Class::class_map;

char* Class::get_name() const {
	return name;
}

shared_ptr<Class> Class::for_name(char* class_name) {
	if (class_map.count(class_name) != 0) {
		return class_map[class_name];
	}
	return shared_ptr<Class>();
}

shared_ptr<Object> Class::new_instance() const {
	return factory();
}

