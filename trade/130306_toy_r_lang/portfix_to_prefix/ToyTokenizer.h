#pragma once

#define TOY_R_TOKEN_NOT_DEFINED	(-1)
#define TOY_R_TOKEN_EOP			0	// end of program
#define TOY_R_TOKEN_NUMBER		1	// group
#define TOY_R_TOKEN_DIGIT		11	// single (internal)
#define TOY_R_TOKEN_STRING		2	// group
#define TOY_R_TOKEN_ALPHA		12	// single (internal)
#define TOY_R_TOKEN_RESERVED	3	// 예약어(IF, MINUS)
#define TOY_R_TOKEN_PARENTHESIS	5	// '(' or ')'
#define TOY_R_TOKEN_SPACE		7	// 공백 (internal)

#include <string>

class ToyTokenizer
{
public:
	ToyTokenizer(void);
	~ToyTokenizer(void);
	void setProg(const char* prog);	// 프로그램(문자열)을 지정한다
	bool getToken(std::string& token, int& type);	// 토큰을 가져온다

private:
	const char* m_prog;			// 프로그램을 가리키는 문자열 포인터(NULL 종료)
	const char* m_current_position;	// 파싱하는 위치(NULL이면 끝, 초기화 필요)
	int assumeTypeByChar(char ch);	// 글자 하나를 가지고 타입을 추측
};

const char* TokenTypeToString(int type);