/*
 * model.h
 *
 *  Created on: Mar 23, 2014
 *      Author: root
 */

#ifndef MODEL_H_
#define MODEL_H_

#define MACHINE_ID 1

#include <string>
#include <queue>

using namespace std;

namespace adbiz {
	namespace model {
		struct client;
		//typedef tuple<char*, char*, char*, int, int, int, int, char*, char*, char*, char*, char*, char*, char*> tuple_client;

		struct client {
			string client_id;
			string alias;
			string last_waiter;
			int first_time;
			int last_time;
			int session_count;
			int visit_count;
			string ip;
			string ip_position;
			string ip_description;
			string url;
			string title;
			string user_agent;
			string track_path;
		};

		struct people {
			string firstName;
			string lastName;
			string email;
		};

		struct message {
			string text;
			long time;
		};

		typedef enum message_type {
			waiter_to_client = 1,
			client_to_waiter = 2,
			waiter_connect = 3,
			client_connect = 4,
			client_disconnect = 5
		} msg_t;

		struct package {
			string from;
			string to;
			int site_id;
			string session_id;
			msg_t message_type;
			queue<message> messages;
		};

		typedef struct connection {
			int conn_address;
			int site_id;
			string client_id;
			short machine_id;
			connection() {
				site_id = 0;
				client_id = "";
				machine_id = MACHINE_ID;
			}
			bool operator < (const connection& c) const{//must add 'const', otherwise can't pass compliration
				if (conn_address < c.conn_address) {
					return true;
				}
				return false;
			}
		} conn_m;
	}
}
#endif /* MODEL_H_ */
