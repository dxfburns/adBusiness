/*
 * business.h
 *
 *  Created on: Mar 25, 2014
 *      Author: root
 */

#ifndef BUSINESS_H_
#define BUSINESS_H_

#include "model.h"

namespace adbiz {
	namespace business {
		namespace client {
			bool init(string&, string&, string&, string&, string&, string&, string&);
			bool remove(string&);
			void process_message(const string&);
			void set_package(string&);
		}
	}
}



#endif /* BUSINESS_H_ */
