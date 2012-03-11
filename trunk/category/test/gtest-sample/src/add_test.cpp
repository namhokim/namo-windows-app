/*
	This sample for /MTd or /MT code generation.

	Follow:
	 1. svn checkout
	  > svn checkout http://namo-windows-app.googlecode.com/svn/trunk/category/test/gtest-1.6.0/ namo-windows-app-read-only
	 2. build
	  - Open with Visual Studio 2008 above or 2003
	    2008 above : gtest-1.6.0/msvc_2008/gtest.sln
		2003       : gtest-1.6.0/msvc/gtest.sln
	  - Build Debug & Release
	  - Run gtest-1.6.0/msvc_2008/gather_lib.bat script then output .lib to lib folder
	 3. copy header & libraries
	  - Header
	    From : gtest-1.6.0/include/gtest
		To   : C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\include/gtest
	  - Libraries
	    From : gtest-1.6.0/msvc_2008/lib
		To   : C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\lib
 */

#include <gtest/gtest.h>
#include "add.h"

#if defined(_MT) && defined(_DLL)	// /MD code generation
#ifdef _DEBUG
#pragma comment(lib, "gtest_mdd.lib")
#pragma comment(lib, "gtest_main-mdd.lib")
#else
#pragma comment(lib, "gtest_md.lib")
#pragma comment(lib, "gtest_main-md.lib")
#endif
#else				// /MT code generation
#ifdef _DEBUG
#pragma comment(lib, "gtest_mtd.lib")
#pragma comment(lib, "gtest_main-mtd.lib")
#else
#pragma comment(lib, "gtest_mt.lib")
#pragma comment(lib, "gtest_main-mt.lib")
#endif
#endif	// _MT && _DLL

TEST(add_test, zero_add_zero_Returns_zero)
{
	ASSERT_EQ(0, add(0,0));
}

TEST(add_test, zero_add_zero_Returns_not_one)
{
	ASSERT_NE(1, add(0,0));
}

TEST(add_test, zero_add_one_Same_one_add_zero)
{
	ASSERT_EQ(add(0,1), add(1,0));
}