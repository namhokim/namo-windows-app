#ifndef _TSTRING_H_
#define _TSTRING_H_

#include <tchar.h>	// for _T

namespace std {
	typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> >	tstring;
	typedef basic_stringstream<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstringstream;
}
#endif	// end of __NAMO_ADDED_TSTRING__

/*
 * Copyright (c) 2009 by Namho Kim.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */
