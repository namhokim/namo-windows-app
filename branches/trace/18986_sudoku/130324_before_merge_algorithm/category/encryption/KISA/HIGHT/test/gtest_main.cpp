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