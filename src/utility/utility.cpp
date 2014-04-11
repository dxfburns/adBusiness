/*
 * utility.cpp
 *
 *  Created on: Mar 25, 2014
 *      Author: root
 */
#include <time.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "../include/utility.h"
#include <sstream>

using namespace boost;
using namespace boost::property_tree;

long adbiz::utility::get_time_stamp() {
	return (long) time(0);
}

string adbiz::utility::gen_uuid() {
	boost::uuids::uuid u;
//	stringstream ss;
//	ss << u;

//	return ss.str().c_str();
//	uuids::uuid u = lexical_cast<uuids::uuid>("01234567-89ab-cdef-0123-456789abcdef");
	cout << u << endl;
	string str = lexical_cast<string>(u);
	return str;
}

string adbiz::utility::get_sha1(const string& s) {
	const char HEX_CHAR[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
	const unsigned long K[] = { 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6 };
	//扩展成K*512位
	unsigned long *data;
	int l;
	l = s.length() * 8;
	data = new unsigned long[((l / 512) + 1) * 512];
	memset(data, 0, sizeof(data[0]) * ((l / 512) + 1) * 512);
	for (unsigned int i = 0; i < s.length(); ++i) {
		data[i / 4] |= s[i] << 8 * (3 - (i % 4));
	}
	data[s.length() / 4] |= 0x80 << 8 * (3 - (s.length() % 4));
	data[((l / 512) + 1) * 512 / 32 - 1] = l;
	l = (l / 512) + 1;
	//开始计算
	unsigned long H[5], G[5];
	H[0] = G[0] = 0x67452301;
	H[1] = G[1] = 0xEFCDAB89;
	H[2] = G[2] = 0x98BADCFE;
	H[3] = G[3] = 0x10325476;
	H[4] = G[4] = 0xC3D2E1F0;
	for (int i = 0; i < l; ++i) {
		unsigned long W[80];
		int t;
		for (t = 0; t < 16; ++t)
			W[t] = data[i * 16 + t];
		for (t = 16; t < 80; ++t) {
			unsigned long tmp = W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16];
			W[t] = (tmp << 1) | (tmp >> 31);
		}
		unsigned long tmp;
		for (t = 0; t < 5; ++t)
			H[t] = G[t];
		for (t = 0; t < 20; ++t) {
			tmp = ((H[0] << 5) | (H[0] >> 27)) + ((H[1] & H[2]) | (~H[1] & H[3])) + H[4] + W[t] + K[0];
			H[4] = H[3];
			H[3] = H[2];
			H[2] = (H[1] << 30) | (H[1] >> 2);
			H[1] = H[0];
			H[0] = tmp;
		}
		for (t = 20; t < 40; ++t) {
			tmp = ((H[0] << 5) | (H[0] >> 27)) + (H[1] ^ H[2] ^ H[3]) + H[4] + W[t] + K[1];
			H[4] = H[3];
			H[3] = H[2];
			H[2] = (H[1] << 30) | (H[1] >> 2);
			H[1] = H[0];
			H[0] = tmp;
		}
		for (t = 40; t < 60; ++t) {
			tmp = ((H[0] << 5) | (H[0] >> 27)) + ((H[1] & H[2]) | (H[2] & H[3]) | (H[1] & H[3])) + H[4] + W[t] + K[2];
			H[4] = H[3];
			H[3] = H[2];
			H[2] = (H[1] << 30) | (H[1] >> 2);
			H[1] = H[0];
			H[0] = tmp;
		}
		for (t = 60; t < 80; ++t) {
			tmp = ((H[0] << 5) | (H[0] >> 27)) + (H[1] ^ H[2] ^ H[3]) + H[4] + W[t] + K[3];
			H[4] = H[3];
			H[3] = H[2];
			H[2] = (H[1] << 30) | (H[1] >> 2);
			H[1] = H[0];
			H[0] = tmp;
		}
		for (t = 0; t < 5; ++t)
			G[t] += H[t];
	}
	delete data;
	char buf[41];
	for (int i = 0; i < 40; ++i) {
		buf[i] = HEX_CHAR[(G[i / 8] >> (4 * (7 - (i % 8)))) & 0xf];
	}
	buf[40] = '\0';

	return string(buf);
}

string adbiz::utility::get_base64_encode(const string &strString) {
	int nByteSrc = strString.length();
	string pszSource = strString;

	int i = 0;
	for (i; i < nByteSrc; i++)
		if (pszSource[i] < 0 || pszSource[i] > 127)
			throw "can not encode Non-ASCII characters";

	const char *enkey = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	string pszEncode(nByteSrc * 4 / 3 + 4, '\0');
	int nLoop = nByteSrc % 3 == 0 ? nByteSrc : nByteSrc - 3;
	int n = 0;
	for (i = 0; i < nLoop; i += 3) {
		pszEncode[n] = enkey[pszSource[i] >> 2];
		pszEncode[n + 1] = enkey[((pszSource[i] & 3) << 4) | ((pszSource[i + 1] & 0xF0) >> 4)];
		pszEncode[n + 2] = enkey[((pszSource[i + 1] & 0x0f) << 2) | ((pszSource[i + 2] & 0xc0) >> 6)];
		pszEncode[n + 3] = enkey[pszSource[i + 2] & 0x3F];
		n += 4;
	}

	switch (nByteSrc % 3) {
	case 0:
		pszEncode[n] = '\0';
		break;

	case 1:
		pszEncode[n] = enkey[pszSource[i] >> 2];
		pszEncode[n + 1] = enkey[((pszSource[i] & 3) << 4) | ((0 & 0xf0) >> 4)];
		pszEncode[n + 2] = '=';
		pszEncode[n + 3] = '=';
		pszEncode[n + 4] = '\0';
		break;

	case 2:
		pszEncode[n] = enkey[pszSource[i] >> 2];
		pszEncode[n + 1] = enkey[((pszSource[i] & 3) << 4) | ((pszSource[i + 1] & 0xf0) >> 4)];
		pszEncode[n + 2] = enkey[((pszSource[i + 1] & 0xf) << 2) | ((0 & 0xc0) >> 6)];
		pszEncode[n + 3] = '=';
		pszEncode[n + 4] = '\0';
		break;
	}

	return pszEncode.c_str();
}

vector<people> adbiz::utility::get_people_from_json(const string& json) {
	vector<people> v;
	ptree pt, p1, p2;
	stringstream stream;

	stream << json;
	boost::property_tree::read_json(stream, pt);
	p1 = pt.get_child("people");
	for (ptree::iterator iter = p1.begin(); iter != p1.end(); ++iter) {
		p2 = iter->second;
		people p;
		p.firstName = p2.get<string>("firstName");
		p.lastName = p2.get<string>("lastName");
		p.email = p2.get<string>("email");

		v.push_back(p);
	}

	return v;
}

void adbiz::utility::set_people_to_json(vector<people>* pv, string& json) {
	ptree pt, pt1;
	vector<people>::iterator iter;
	for (iter = pv->begin(); iter != pv->end(); iter++) {
		people p(*iter);
		ptree pt2;
		pt2.put("firstName", p.firstName);
		pt2.put("lastName", p.lastName);
		pt2.put("email", p.email);

		pt1.push_back(std::make_pair("", pt2));
	}
	pt.put_child("people", pt1);

	ostringstream os;
	boost::property_tree::write_json(os, pt, true);

	json = os.str();
}

void adbiz::utility::package_to_message(package& pack, string& json) {
	ptree pt, pt1;
	pt.put("from", pack.from);
	pt.put("to", pack.to);
	pt.put<int>("site_id", pack.site_id);
	pt.put("session_id", pack.session_id);
	pt.put<short>("message_type", (short) pack.message_type);
	while (!pack.messages.empty()) {
		message msg = pack.messages.front();
		ptree pt2;
		pt2.put("text", msg.text);
		pt2.put<long>("time", msg.time);

		pt1.push_back(std::make_pair("", pt2));

		pack.messages.pop();
	}

	pt.put_child("messages", pt1);

	ostringstream os;
	boost::property_tree::write_json(os, pt);

	json = os.str();
}

#include <cstdlib>
void adbiz::utility::message_to_package(const string& msg, package& pack) {
	ptree pt, pt1;
	stringstream stream;

	stream << msg;
	boost::property_tree::read_json(stream, pt);
	pack.from = pt.get<string>("from");
	pack.to = pt.get<string>("to");
	pack.site_id = atoi(pt.get<string>("site_id").c_str());
	pack.session_id = atoi(pt.get<string>("session_id").c_str());
	pack.message_type = (msg_t) (atoi(pt.get<string>("message_type").c_str()));

	pt1 = pt.get_child("messages");
	queue<message> q;
	for (ptree::iterator iter = pt1.begin(); iter != pt1.end(); iter++) {
		ptree pt2 = iter->second;
		message msg;
		msg.text = pt2.get<string>("text");
		msg.time = atol(pt2.get<string>("time").c_str());

		q.push(msg);
	}
	pack.messages = q;
}
