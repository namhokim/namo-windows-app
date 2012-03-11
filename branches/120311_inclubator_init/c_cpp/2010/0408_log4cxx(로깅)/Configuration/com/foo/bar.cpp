// file bar.cpp
#include "bar.h"

using namespace com::foo;
using namespace log4cxx;

#pragma comment(lib, "log4cxx.lib")

LoggerPtr Bar::logger(Logger::getLogger("com.foo.Bar"));

void Bar::doIt() {
	LOG4CXX_DEBUG(logger, "Did it again!");
}
