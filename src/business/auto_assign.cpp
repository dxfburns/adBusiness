/*
 * auto_assign.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: root
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/smart_ptr.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/assign.hpp>
#include "../include/auto_assign.h"

using namespace boost;
using namespace boost::assign;
using namespace boost::property_tree;
using namespace adbiz::business::auto_assign;

template<typename T>
processor_base* processor_factory<T>::create() {
	return new T;
}

map<string, my_class*> my_class::class_map;

template<typename T>
void my_class::regist(string class_name) {
	if (class_map.count(class_name.c_str()) == 0) {
		my_class* cls = new my_class;
		cls->factory = &processor_factory<T>::create;
		cls->name = class_name;
		class_map[class_name] = cls;
	}
}

processor_base* my_class::new_instance() {
	return this->factory();
}

my_class* my_class::for_name(string class_name) {
	if (class_map.count(class_name) != 0) {
		return class_map[class_name];
	}

	return 0;
}

string my_class::get_name() const {
	return name;
}

void regist_classes() {
	my_class::regist<processor_last_waiter>("adbiz::business::auto_assign::processor_last_waiter");
	my_class::regist<processor_capcity>("adbiz::business::auto_assign::processor_capcity");
	my_class::regist<processor_policy_zone>("adbiz::business::auto_assign::processor_policy_zone");
	my_class::regist<processor_policy_page>("adbiz::business::auto_assign::processor_policy_page");
	my_class::regist<processor_final>("adbiz::business::auto_assign::processor_final");
}

static vector<string> client_class;

void read_strategy_xml() {
	ptree pt;
	read_xml("strategy.xml", pt);

	BOOST_AUTO(child, pt.get_child("auto_assign.client"));
	for (BOOST_AUTO(iter, child.begin()); iter != child.end(); iter++) {
		client_class += iter->second.get_value<string>();
	}
}

anylizer::anylizer(processor_base* p_base) {
	this->processor = p_base;
}

void anylizer::set_context(context* p_cxt) {
	processor->p_cxt = p_cxt;
}

void anylizer::execute() {
	processor->execute();
}

void processor_last_waiter::execute() {
	string last_waiter = "12345_10957";
	p_cxt->last_waiter = last_waiter;
}

void processor_capcity::execute() {
	p_cxt->candidates += "12345_10957";
	p_cxt->candidates += "12346_10957";
	p_cxt->candidates += "12347_10957";
	p_cxt->candidates += "12348_10957";
	p_cxt->candidates += "12349_10957";
}

void processor_policy_zone::execute() {
	vector<string>::iterator end = remove(p_cxt->candidates.begin(), p_cxt->candidates.end(), "12345_10957");
	p_cxt->candidates.erase(end, p_cxt->candidates.end());

	if (p_cxt->candidates.size() == 0) {
		p_cxt->can_next = false;
	}
}

void processor_policy_page::execute() {
	vector<string>::iterator end = remove(p_cxt->candidates.begin(), p_cxt->candidates.end(), "12346_10957");
	p_cxt->candidates.erase(end, p_cxt->candidates.end());

	if (p_cxt->candidates.size() == 0) {
		p_cxt->can_next = false;
	}
}

#include <stdlib.h>
void processor_final::execute() {
	std::srand((int) time(0));
	double count = p_cxt->candidates.size() * 1.0;
	int index = (int) (count * rand() / RAND_MAX);
	p_cxt->final_assigner = p_cxt->candidates[index];
}

#include <boost/thread.hpp>
void anylizer_processor::execute_client() {
	mutex io_mu;
	mutex::scoped_lock lock(io_mu);

	shared_ptr<context> sp_cxt(new context);
	sp_cxt->can_next = true;

	for (BOOST_AUTO(iter, client_class.begin()); iter != client_class.end(); iter++) {
		cout << *iter << endl;
		shared_ptr<my_class> sp_mc(my_class::for_name(*iter));
		shared_ptr<processor_base> sp_pb(sp_mc->new_instance());
		anylizer alz(sp_pb.get());
		alz.set_context(sp_cxt.get());
		alz.execute();

		if (!sp_cxt->can_next) {
			break;
		}
	}

	cout << sp_cxt->final_assigner << endl;

	lock.unlock();
}
