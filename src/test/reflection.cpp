/*
 * reflection.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: root
 */
#include "../include/reflection.h"

using namespace adbiz::utility;

class MyClass: public Object {
public:
	MyClass() {
		cout << "MyClass is initialized" << endl;
	}
	~MyClass() {
		cout << "MyClass is destroyed" << endl;
	}
};

class MyClass2: public Object {
public:
	MyClass2() {
		cout << "MyClass2 is initialized" << endl;
	}
	~MyClass2() {
		cout << "MyClass2 is destroyed" << endl;
	}
};

map<char*, shared_ptr<Class> > adbiz::utility::Class::class_map;

void test_reflection() {
	Class::regist<MyClass>("MyClass");
	Class::regist<MyClass2>("MyClass2");

	weak_ptr<Class> wp(Class::for_name("MyClass"));
	if (!wp._empty()) {
		shared_ptr<Class> cls = wp.lock();
		shared_ptr<Object> obj = cls->new_instance();
	}
	wp.reset();
	wp = weak_ptr<Class>(Class::for_name("MyClass2"));
	if (!wp._empty()) {
		shared_ptr<Class> cls = wp.lock();
		shared_ptr<Object> obj = cls->new_instance();
	}
}

