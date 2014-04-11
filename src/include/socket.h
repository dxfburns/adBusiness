/*
 * socket_server.h
 *
 *  Created on: Mar 25, 2014
 *      Author: root
 */

#ifndef SOCKET_SERVER_H_
#define SOCKET_SERVER_H_
#include <boost/asio.hpp>
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

namespace adbiz {
	namespace socket {
		class server {
		private:
			io_service &ios;
			tcp::acceptor acceptor;
			typedef boost::shared_ptr<tcp::socket> sock_pt;
			void response_websocket_connection(sock_pt&, char*);
		public:
			server(io_service &);
			void start();
			void accept_handler(const boost::system::error_code &, sock_pt);
			void write_handler(const boost::system::error_code &);
		};
	}
}



#endif /* SOCKET_SERVER_H_ */
