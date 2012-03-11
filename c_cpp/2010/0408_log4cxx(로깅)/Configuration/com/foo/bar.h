// file com/foo/bar.h
#include "log4cxx/logger.h"

namespace com {
	namespace foo {
		class Bar {
			static log4cxx::LoggerPtr logger;

		public:
			void doIt();
		};
	}
}