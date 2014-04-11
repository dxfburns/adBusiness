/*
 * utility.h
 *
 *  Created on: Mar 25, 2014
 *      Author: root
 */

#ifndef UTILITY_H_
#define UTILITY_H_
#include <string>
#include <vector>
#include "model.h"
using namespace std;
using namespace adbiz::model;

namespace adbiz {
	namespace utility {
		long get_time_stamp();
		string gen_uuid();
		string get_sha1(const string&);
		string get_base64_encode(const string&);
		vector<people> get_people_from_json(const string&);
		void set_people_to_json(vector<people>*, string&);
		void package_to_message(package&, string&);
		void message_to_package(const string&, package&);
	}
}




#endif /* UTILITY_H_ */
