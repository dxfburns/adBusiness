/*
 * auto_assign.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: root
 */
#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/typeof/typeof.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "../include/auto_assign.h"
#include "../include/utility.h"

using namespace adbiz::business::auto_assign;
using namespace adbiz::utility;

void regist_classes() {
	Class::regist<processor_last_waiter>("adbiz::business::auto_assign::processor_last_waiter");
	Class::regist<processor_capcity>("adbiz::business::auto_assign::processor_capcity");
	Class::regist<processor_policy_zone>("adbiz::business::auto_assign::processor_policy_zone");
	Class::regist<processor_policy_page>("adbiz::business::auto_assign::processor_policy_page");
	Class::regist<processor_final>("adbiz::business::auto_assign::processor_final");
}

static vector<string> client_class;

#include <boost/assign.hpp>
void read_strategy_xml() {
	using namespace boost::property_tree;
	using namespace boost::assign;

	ptree pt;
	read_xml("strategy.xml", pt);

	BOOST_AUTO(child, pt.get_child("auto_assign.client"));
	for (BOOST_AUTO(iter, child.begin()); iter != child.end(); iter++) {
		client_class += iter->second.get_value<string>();
	}
}

void anylizer::set_context(map<string, long>& context) {
	processor->context = context;
}

void anylizer::execute() {
	processor->execute();
}

void processor_last_waiter::execute() {
	string last_waiter = "12345_10957";
	this->context[context_key::last_waiter] = (long) &last_waiter;
	bool can_next = true;
	this->context[context_key::can_next] = (long) &can_next;
}

void processor_capcity::execute() {
	bool* p_can_next = (bool*) this->context[context_key::can_next];
	if (!*p_can_next) {
		bool can_next = false;
		this->context[context_key::can_next] = (long) &can_next;

		return;
	}

	vector<string> v;
	v.push_back("12345_10957");
	v.push_back("12346_10957");
	v.push_back("12347_10957");
	this->context[context_key::candidates] = (long) &v;

	bool can_next = true;
	this->context[context_key::can_next] = (long) &can_next;
}

void processor_policy_zone::execute() {
	bool* p_can_next = (bool*) this->context[context_key::can_next];
	if (!*p_can_next) {
		bool can_next = false;
		this->context[context_key::can_next] = (long) &can_next;

		return;
	}

	vector<string>* p_v = (vector<string>*) this->context[context_key::candidates];
	vector<string>::iterator new_end = remove(p_v->begin(), p_v->end(), "12345_10957");
	p_v->erase(new_end, p_v->end());

	this->context[context_key::candidates] = (long) p_v;
	bool can_next = true;
	this->context[context_key::can_next] = (long) &can_next;
}

void processor_policy_page::execute() {
	bool* p_can_next = (bool*) this->context[context_key::can_next];
	if (!*p_can_next) {
		bool can_next = false;
		this->context[context_key::can_next] = (long) &can_next;

		return;
	}

	vector<string>* p_v = (vector<string>*) this->context[context_key::candidates];
	vector<string>::iterator new_end = remove(p_v->begin(), p_v->end(), "12346_10957");
	p_v->erase(new_end, p_v->end());

	this->context[context_key::candidates] = (long) p_v;
	bool can_next = true;
	this->context[context_key::can_next] = (long) &can_next;
}

void processor_final::execute() {
	bool* p_can_next = (bool*) this->context[context_key::can_next];
	if (!*p_can_next) {
		bool can_next = false;
		this->context[context_key::can_next] = (long) &can_next;

		return;
	}

	string final_assigner;
	vector<string>* p_v = (vector<string>*) this->context[context_key::candidates];
	vector<string>::iterator iter;
	for (iter = p_v->begin(); iter != p_v->end(); iter++) {
		final_assigner = *iter;
	}

	this->context[context_key::final_assigner] = (long) &final_assigner;
	bool can_next = false;
	this->context[context_key::can_next] = (long) &can_next;
}

const string context_key::can_next = "CAN_NEXT";
const string context_key::last_waiter = "LAST_WAITER";
const string context_key::candidates = "CANDIDATES";
const string context_key::final_assigner = "FINAL_ASSIGNER";

