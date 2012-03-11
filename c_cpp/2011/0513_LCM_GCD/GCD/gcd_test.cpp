#include <gtest/gtest.h>

#include "gcd.h"

TEST (TestSample, TestAdd) {
	ASSERT_EQ(1, gcd(1, 1));
	ASSERT_EQ(1, gcd(2, 1));
	ASSERT_EQ(2, gcd(2, 2));
	ASSERT_EQ(2, gcd(4, 2));
}
