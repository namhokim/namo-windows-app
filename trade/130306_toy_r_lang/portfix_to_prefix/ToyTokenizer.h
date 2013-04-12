#pragma once

#define TOKEN_NOT_DEFINED	(-1)
#define TOKEN_EOP			0	// end of program
#define TOKEN_NUMBER		1	// group
#define TOKEN_DIGIT			11	// single (internal)
#define TOKEN_NEGATIVE		12	// single (internal)
#define TOKEN_STRING		2	// group
#define TOKEN_ALPHA			21	// single (internal)
#define TOKEN_RESERVED		3	// 예약어(IF, MINUS)
#define TOKEN_PARENTHESIS	5	// '(' or ')'
#define TOKEN_SPACE			7	// 공백 (internal)

#include <string>

class ToyTokenizer
{
public:
	ToyTokenizer(void);
	~ToyTokenizer(void);
	void setProg(const char* prog);	// 프로그램(문자열)을 지정한다
	bool getToken(std::string& token, int& type);	// 토큰을 가져온다
	bool getInnerProg(std::string& subProg);	// 다음 닫는 괄호를 찾을 때까지 문장을 획득
	int getCurrentIndex() const;				// 현재 tokenize 위치를 가져온다(0~N)

private:
	const char* m_prog;			// 프로그램을 가리키는 문자열 포인터(NULL 종료)
	const char* m_curr_pos;		// 파싱하는 현재위치(NULL이면 끝, 초기화 필요)
	int assumeTypeByChar(char ch);	// 글자 하나를 가지고 타입을 추측
};

const char* TokenTypeToString(int type);
bool parse(const char* prog, int* error_pos = NULL);	// 파싱을 수행(문법 체크)
