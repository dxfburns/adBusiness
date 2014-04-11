/*
 * server.cpp
 *
 *  Created on: Mar 25, 2014
 *      Author: root
 */
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include "../include/utility.h"
#include "../include/socket.h"

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace adbiz::socket;
using namespace adbiz::utility;

adbiz::socket::server::server(io_service & io) :
		ios(io), acceptor(ios, tcp::endpoint(tcp::v4(), 6688)) {
	this->start();
}

void adbiz::socket::server::start() {
	sock_pt sock(new tcp::socket(ios));
	this->acceptor.async_accept(*sock, boost::bind(&server::accept_handler, this, placeholders::error(), sock));
}

void adbiz::socket::server::response_websocket_connection(sock_pt& sock, char* buff) {
	list<string> headers;
	map<string, string> mp;
	boost::split(headers, buff, boost::is_any_of("\r\n\r\n"), boost::token_compress_on);
	for (list<string>::iterator iter = headers.begin(); iter != headers.end(); iter++) {
		cout << *iter;
		if (boost::contains(*iter, ":")) {
			int flag = (*iter).find_first_of(':');
			cout << ",pos : " << flag;
			string key = (*iter).substr(0, flag);
			int pend = (*iter).size() - flag;
			string value = (*iter).substr(flag + 1, pend);
			std::pair<const string, string> par(key, value);
			mp.insert(par);
		}
		cout << endl;
	}
	boost::format fmt("HTTP/1.1 101 Switching Protocols\r\n"
			"Upgrade:websocket\r\n"
			"Connection:Upgrade\r\n"
			"Sec-WebSocket-Accept:%1%\r\n"
			"WebSocket-Origin:%2%\r\n"
			"WebSocket-Location:ws://%3%/WebManagerSocket\r\n"
			"WebSocket-Protocol:WebManagerSocket");

	boost::trim(mp["Sec-WebSocket-Origin"]);
	boost::trim(mp["Sec-WebSocket-Key"]);
	boost::trim(mp["Host"]);

	string sec_origion, sec_key, sec_host;
	sec_origion = mp["Sec-WebSocket-Origin"];
	sec_key = mp["Sec-WebSocket-Key"].append("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	sec_host = mp["Host"];

	if (sec_origion.empty()) {
		sec_origion = "192.168.254.130";
	}
	string res;
	cout<<"Please input the key : ";
	cin>>res;
	sec_key =res; //get_base64_encode(get_sha1(sec_key));
	fmt % sec_key % sec_origion % sec_host;
	string text = fmt.str();
	cout << text << endl;

	sock->async_send(boost::asio::buffer(text), boost::bind(&server::write_handler, this, placeholders::error()));
}

void adbiz::socket::server::accept_handler(const boost::system::error_code & ec, sock_pt sock) {
	if (ec) {
		return;
	}

	try {
		cout << "client : ";
		cout << sock->remote_endpoint().address() << endl;

		char buff[1024];
		int size = sock->receive(buffer(buff, 1024));
		buff[size] = 0;
		cout << "receive " << size << " bytes from client:\n" << buff << endl;

		bool is_json = buff[0] == '{' ? true : false;
		if (!is_json) {
			bool is_ws_req = boost::contains(buff, "websocket");
			if (is_ws_req) {
				response_websocket_connection(sock, buff);
			}
		}
	} catch (std::exception& ex) {
		cout << ex.what() << endl;
	}

	start();
}

void adbiz::socket::server::write_handler(const boost::system::error_code & ec) {
	cout << "send msg complete." << endl;
}

