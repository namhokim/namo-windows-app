#include <gtest/gtest.h>

// user library for TEST
#include "Sample.h"

TEST (TestSample, TestAdd) {
	int i,j;
	i = 1;
	j = 2;

	ASSERT_EQ( (i+j), add(i, j) );
}
