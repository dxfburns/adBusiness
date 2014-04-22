/*
 * log.cpp
 *
 *  Created on: Apr 11, 2014
 *      Author: root
 */

#include <log4cplus/fileappender.h>
#include <log4cplus/layout.h>
#include <log4cplus/configurator.h>

#include "../include/log.h"

Logger logger = Logger::getInstance(LOG4CPLUS_TEXT("adbiz_log"));

namespace adbiz {
	namespace utility {
		void InitLogger(bool daemonized)
		{
			if (daemonized)
				PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("/usr/local/workspace/adBusiness/src/log4cplus.conf"));
			else
				PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("/usr/local/workspace/adBusiness/src/log4cplusd.conf"));
		}
	}
}

