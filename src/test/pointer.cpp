/*
 * pointer.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: root
 */
#include <iostream>
#include <string>

using namespace std;

class A {
private:
	int x;
	long y;
	string s;
public:
	A(int i) :
			x(i), y(0), s("xuefengdu") {
		cout << "Execute constructor to create a object, this : " << this << endl;
	}
	A(const A& a) {
		x = a.x;
		y = a.y;
		s = a.s;
		cout << "Execute copy constructor to create a object, this :" << this << ", a : " << &a << endl;
	}
	~A() {
		cout << "Execute destructor, this : " << this << endl;
	}
	int get() {
		return x;
	}
};

//A& func() {
//	A *p = new A(99);
//	cout << "p : " << p <<endl;
//	return *p;
//}

A* func() {
	A *p = new A(99);
	cout << "p : " << p << endl;
	return p;
}

A simple_func() {
	A a(18);
	return a;
}

void test_func() {
	A b = simple_func();
	//	A& r = func();
	//	cout<< "r : " << &r << endl;
	//	cout << r.get() << endl;
	//	A *pa = &r;
	//	delete pa;
	//	cout << r.get() << endl;
	A *r = func();
	cout << "r : " << r << endl;
	cout << r->get() << endl;
	A a(9);
	cout << "sizeof(a) : " << sizeof(a) << endl;
	A *p = new A(99);
	cout << "sizeof(r) : " << sizeof(r) << ", sizeof(*r) : " << sizeof(*r) << endl;
	cout << "sizeof(p) : " << sizeof(p) << ", sizeof(*p) : " << sizeof(*p) << endl;
	delete r;
	cout << r->get() << endl;
	delete p;
	cout << p->get() << endl;
}

#include<vector>
#include<deque>
#include<list>
#include<set>
#include<map>
void size_of_container() {
	vector<int> v(100, 4);
	deque<int> d(100, 4);
	list<int> l(100, 4);
	set<int> s;
	s.insert(10);
	s.insert(13);
	map<int, int> m;
	m.insert(pair<int, int>(10, 4));
	m.insert(pair<int, int>(20, 4));
	cout << "vector : " << sizeof(v) << endl;
	cout << "deque : " << sizeof(d) << endl;
	cout << "list : " << sizeof(l) << endl;
	cout << "set : " << sizeof(s) << endl;
	cout << "map : " << sizeof(m) << endl;
//	vector : 12
//	deque : 40
//	list : 8
//	set : 24
//	map : 24
}

void mem_address_test() {
	string str1("address test1");
	string str2("address test2");
	set<string> s;
	s.insert(str1);
	s.insert(str2);
	cout << "orig str1:" << (int) &str1 << ", orig str2:" << (int) &str2 << endl;
	cout << "int str1:" << (int) &str1 << ", int str2:" << (int) &str2 << endl;
	set<int> si;
	si.insert((int) &str1);
	si.insert((int) &str2);

	set<string>::iterator iter = s.find(str1);
	cout << *iter << " : " << (int) &(*iter) << endl;
	for (iter = s.begin(); iter != s.end(); iter++) {
//		int add = (int)&(*iter);
		cout << *iter << " : " << (int) &(*iter) << endl;
	}

	set<int>::iterator iter2;
	for (iter2 = si.begin(); iter2 != si.end(); iter2++) {
		cout << *iter2 << " : " << (int) &(*iter2) << endl;
		string* ps1 = (string*) (*iter2);
		cout << "ps1 value : " << *ps1 << endl;

//		string* ps2 = (string*) ((int) &(*iter2));
//		cout << "ps2 value : " << *ps2 << endl;	//not available, it is a wild pointer
	}
}

#include "../include/business.h"
void process_json() {
	string json = "{ \"people\": [ { \"firstName\": \"Brett\", \"lastName\":\"McLaughlin\", \"email\": \"aaaa\" },"
			"{ \"firstName\": \"Jason\", \"lastName\":\"Hunter\", \"email\": \"bbbb\"},"
			"{ \"firstName\": \"Elliotte\", \"lastName\":\"Harold\", \"email\": \"cccc\" } ]}";
	adbiz::business::client::process_message(json);
}
