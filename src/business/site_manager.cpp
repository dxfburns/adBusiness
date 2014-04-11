/*
 * site_manager.cpp
 *
 *  Created on: Apr 1, 2014
 *      Author: root
 */

#include "../include/site_manager.h"
using namespace adbiz::business;

void adbiz::business::site::add_connection(connection_hdl& hdl, const string& client_id) {
}

void adbiz::business::site::update_connection(connection_hdl& hdl, const string& client_id) {

}

void adbiz::business::site::remove_connection(connection_hdl& hdl) {

}

void adbiz::business::site_manager::add_site(int site_id) {
	site* p_site = this->get_site(site_id);
	if (p_site != 0) {
		return;
	}

	p_site = new site(site_id);
	site_map.insert(std::make_pair(site_id, p_site));
}

void adbiz::business::site_manager::remove_site(int site_id) {
	site* p_site = this->get_site(site_id);
	if (p_site == 0) {
		return;
	}

	site_map.erase(site_id);
	delete p_site;
}

site* adbiz::business::site_manager::get_site(int site_id) {
	map<int, site*>::iterator iter = site_map.find(site_id);
	if (iter == site_map.end()) {
		return 0;
	}

	return site_map[site_id];
}
