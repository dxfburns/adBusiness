/*
 * reflection.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: root
 */
#include "../include/reflection.h"
using namespace adbiz::utility;

char* adbiz::utility::Class::get_name() const {
	return name;
}

shared_ptr<Class> adbiz::utility::Class::for_name(char* class_name) {
	if (class_map.count(class_name) != 0) {
		return class_map[class_name];
	}
	return shared_ptr<Class>();
}

shared_ptr<Object> adbiz::utility::Class::new_instance() const {
	return factory();
}

