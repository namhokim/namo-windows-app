#include <gtest/gtest.h>

#include "lcm.h"

TEST (TestSample, TestAdd) {
	ASSERT_EQ(1, lcm(1, 1));
	ASSERT_EQ(2, lcm(2, 1));	// prime number
	ASSERT_EQ(2, lcm(2, 2));
	ASSERT_EQ(4, lcm(4, 2));
	ASSERT_EQ(21, lcm(3, 7));	// prime number
}
