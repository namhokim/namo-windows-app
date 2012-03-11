#include <gtest/gtest.h>

#ifdef _DEBUG
#pragma comment(lib, "../Debug/gtestd.lib")
#else
#pragma comment(lib, "../Release/gtest.lib")
#endif

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}