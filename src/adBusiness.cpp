//============================================================================
// Name        : adBusiness.cpp
// Author      : Xuefeng Du
// Version     :
// Copyright   : adSage
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <boost/asio.hpp>
#include "include/log.h"
#include "include/model.h"
#include "include/database.h"
#include "include/socket.h"
#include "include/websocket.h"

using namespace std;
using namespace boost::asio;
using namespace adbiz::model;
using namespace adbiz::db::manage;
using namespace adbiz::socket;
using namespace adbiz::websocket;

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

void test_func();

//int main() {
////	test_func();
////	db_client dbc;
////	client* clt = dbc.get_client("1919990403A1429940393D1996B19I6Z18.89512_10957");
////	bool res = dbc.remove_client(clt->client_id);
//	io_service ios;
//	server serv(ios);
//	ios.run();
//	return 0;
//}

void test_func() {
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

#include "include/business.h"
void process_json() {
	string json = "{ \"people\": [ { \"firstName\": \"Brett\", \"lastName\":\"McLaughlin\", \"email\": \"aaaa\" },"
			"{ \"firstName\": \"Jason\", \"lastName\":\"Hunter\", \"email\": \"bbbb\"},"
			"{ \"firstName\": \"Elliotte\", \"lastName\":\"Harold\", \"email\": \"cccc\" } ]}";
	adbiz::business::client::process_message(json);
}

void reset_socket_connection_in_db() {
	adbiz::db::manage::db_connection_manager::db_connection_manager().remove_connection_by_machineid(MACHINE_ID);
}

int main() {
	adbiz::utility::InitLogger(true);
	LOG_INFO("First log test");
	reset_socket_connection_in_db();
	LOG_INFO("socket connections were cleared.");
	//mem_address_test();
	//process_json();
	//size_of_container();
	run_wspp_server();

}
