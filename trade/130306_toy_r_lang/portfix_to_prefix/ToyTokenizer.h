#pragma once

#define TOKEN_NOT_DEFINED	(-1)
#define TOKEN_EOP			0	// end of program
#define TOKEN_NUMBER		1	// group
#define TOKEN_DIGIT			11	// single (internal)
#define TOKEN_NEGATIVE		12	// single (internal)
#define TOKEN_STRING		2	// group
#define TOKEN_ALPHA			13	// single (internal)
#define TOKEN_RESERVED		3	// 예약어(IF, MINUS)
#define TOKEN_PARENTHESIS	4	// '(' or ')'
#define TOKEN_SPACE			5	// 공백 (internal)
#define TOKEN_FLOAT_PTR		6	// 소수
#define TOKEN_DOT			14	// 점 (internal)

#define NON_ERROR		0	//	에러없음
#define EOP_ERROR		0	// 프로그램의 끝을 만남
#define L_PAREN_ERROR	1	// '('괄호의 사용이 잘못되었습니다
#define R_PAREN_ERROR	2	// ')'괄호의 사용이 잘못되었습니다
#define OPERATOR_ERROR	3	// 연산자오류입니다
#define OPERAND_ERRPR	4	// 피연산자오류입니다
#define FLOAT_P_ERROR	5	// 실수를 입력하셨습니다
#define SYMBOL_ERROR	6	// 잘못된 기호가 입력되었습니다
#define UNDEFINED_ERROR	7	// Undefined
#define NO_PROG_ERROR	8	// 프로그램이 없음
#define NO_BEGIN_ERROR	9	// 중간코드가 begin으로 시작하지 않음
#define NO_END_ERROR	10	// 중간코드가 end로 끝나지 않음
#define BAD_IC_ERROR	11	// 잘못된 중간코드

#include <string>
#include <vector>

class ToyTokenizer
{
public:
	ToyTokenizer(void);
	~ToyTokenizer(void);
	void setProg(const char* prog);	// 프로그램(문자열)을 지정한다
	int getToken(std::string& token, int& type);	// 토큰을 가져온다
	int getInnerProg(std::string& subProg);	// 다음 닫는 괄호를 찾을 때까지 문장을 획득
	int getCurrentIndex() const;				// 현재 tokenize 위치를 가져온다(0~N)

private:
	const char* m_prog;			// 프로그램을 가리키는 문자열 포인터(NULL 종료)
	const char* m_curr_pos;		// 파싱하는 현재위치(NULL이면 끝, 초기화 필요)
	int assumeTypeByChar(char ch);	// 글자 하나를 가지고 타입을 추측
};

const char* TokenTypeToString(int type);
int make_im_code(const char* prog, std::vector<std::string>& out);	// 중간코드 생성
int postfix_to_prefix(const char* prog, std::string& out);		// 후위->전위식 변환
int evaluation(const std::vector<std::string>& in, int& out);	// 중간코드를 평가하는 함수


const char* ErrorCodeToStringA(int code);
const wchar_t* ErrorCodeToStringW(int code);

#ifdef UNICODE
#define ErrorCodeToString ErrorCodeToStringW
#else
#define ErrorCodeToString ErrorCodeToStringA
#endif