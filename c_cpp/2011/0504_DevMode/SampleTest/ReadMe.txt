========================================================================
    라이브러리 : SampleTest 프로젝트 개요
========================================================================

아래와 같은 설정을 하면 됩니다.


main.cpp
	의 내용을 복사합니다. 진입점입니다.
	
.vcproj
    프로젝트의 속성을 조정합니다.
    1)
    일반-추가 포함 디렉토리(모든구성) 추가
		"$(SolutionDir)3rd_gtest\include"

    2)
    프로젝트 종속성 추가
		공용 속성-프로젝트 종속성(3rd_gtest에 종속 선택)

/////////////////////////////////////////////////////////////////////////////

TEST functions (in gtest.h)

GTEST_TEST_BOOLEAN_ 계열
	ASSERT_ 계열
		ASSERT_TRUE(condition) 
		ASSERT_FALSE(condition) 
	EXPECT_ 계열
		EXPECT_TRUE(condition) 
		... 

GTEST_PRED_FORMAT1_ 계열
	ASSERT_ 계열
		ASSERT_HRESULT_SUCCEEDED(expr) 
		ASSERT_HRESULT_FAILED(expr) 
	EXPECT_ 계열
		EXPECT_HRESULT_SUCCEEDED(expr) 
		... 

GTEST_PRED_FORMAT2_ 계열
	ASSERT_ 계열
		ASSERT_EQ(expected, actual)   // == 
		ASSERT_NE(val1, val2)              // != 
		EXPECT_LE(val1, val2)              // <= 
		ASSERT_LT(val1, val2)               // < 
		ASSERT_GE(val1, val2)               // >= 
		ASSERT_GT(val1, val2)               // > 
		ASSERT_STREQ(expected, actual)   // 문자열 비교(대소문자 구분) 
		ASSERT_STRNE(s1, s2) 
		ASSERT_STRCASEEQ(expected, actual)   // 문자열 대소문자 무시 비교 
		ASSERT_STRCASENE(s1, s2) 
		ASSERT_FLOAT_EQ(expected, actual) 
		ASSERT_DOUBLE_EQ(expected, actual) 
	EXPECT_ 계열
		EXPECT_EQ(expected, actual) 
		... 

GTEST_PRED_FORMAT3_ 계열
	ASSERT_ 계열
		ASSERT_NEAR(val1, val2, abs_error) 
	EXPECT_ 계열
		EXPECT_NEAR(val1, val2, abs_error) 

/////////////////////////////////////////////////////////////////////////////
