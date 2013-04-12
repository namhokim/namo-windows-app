#include "StdAfx.h"
#include "ToyTokenizer.h"

//#define VERBOSE

// 예약어 목록
namespace Toy_R_Reserved {
	namespace IF {
		const char* str = "IF";
		int size = 2;
	}
	namespace MINUS {
		const char* str = "MINUS";
		int size = 5;
	}
}


// 외부에서 사용하는 타입으로 변환해주는 유틸리티 함수
// 예) 한 글자의 숫자(digit) -> 숫자(number)
// 예) 한 글자의 문자(alphabet) -> 문자열(string)
int ConvertExternalType(int type)
{
	switch(type) {
		case TOKEN_DIGIT:
			return TOKEN_NUMBER;
		case TOKEN_ALPHA:
			return TOKEN_STRING;
		default:
			return type;
	}
}

// 예약어인지 판단한다.
bool IsReservedWord(const char* ptr, std::string& word)
{
	// IF
	if (strncmp(Toy_R_Reserved::IF::str, ptr, Toy_R_Reserved::IF::size)==0) {
		word.assign(Toy_R_Reserved::IF::str);
		return true;
	}

	// MINUS
	if (strncmp(Toy_R_Reserved::MINUS::str, ptr, Toy_R_Reserved::MINUS::size)==0) {
		word.assign(Toy_R_Reserved::MINUS::str);
		return true;
	}

	// etc.
	return false;
}

//////////////////////////////////////////////////////////////////////////
// public

ToyTokenizer::ToyTokenizer(void)
:m_curr_pos(NULL)
{
}

ToyTokenizer::~ToyTokenizer(void)
{
}

void ToyTokenizer::setProg(const char* prog)
{
	m_prog = prog;
	if (prog==NULL)
		throw new std::invalid_argument("Not allow, NULL value in prog parameter.");

	m_curr_pos = m_prog;
}

bool ToyTokenizer::getToken(std::string& token, int& type)
{
	// 출력 인자 초기화
	token.clear();
	type = TOKEN_NOT_DEFINED;

	int tb = TOKEN_NOT_DEFINED;	// type of berfore(이전 상태를 저장)
	char curr_ch = (*m_curr_pos);	// 현재 문자열

	while(curr_ch!='\0') {	// NULL이 아닐 때까지 반복
		
		type = assumeTypeByChar(curr_ch);
		switch(type) {
			case TOKEN_PARENTHESIS:
				// 다른 타입에서 괄호를 만났을 때
				if (tb!=TOKEN_NOT_DEFINED && tb!=TOKEN_PARENTHESIS) {
					type = ConvertExternalType(tb);
					return true;	// 토근 획득 완료(성공)
				}
				// 처음 괄호를 만났을 때
				else {
					token.push_back(curr_ch);
					++m_curr_pos;	// 다음 위치 수정
					return true;
				}
			case TOKEN_DIGIT:
				// 처음 숫자하나
				if (tb==TOKEN_NOT_DEFINED || tb==TOKEN_DIGIT) {
					token.push_back(curr_ch);
					++m_curr_pos;	// 다음 위치
					break;
				} else {
					type = TOKEN_NUMBER;
					return true;
				}
			case TOKEN_NEGATIVE:
				if (tb==TOKEN_NOT_DEFINED) {
					type = TOKEN_DIGIT;	// 첫 번째 나오는 -는 음수로 간주
					token.push_back(curr_ch);
					++m_curr_pos;	// 다음 위치
				} else {	// 에러
					token.clear();
					type = TOKEN_NOT_DEFINED;
					return false;
				}
				break;
			case TOKEN_ALPHA:
				if (tb==TOKEN_NOT_DEFINED && IsReservedWord(m_curr_pos, token)) {
					type = TOKEN_RESERVED;
					m_curr_pos += token.size();
					return true;
				} else if (tb==TOKEN_DIGIT) {
					type = ConvertExternalType(tb);
					return true;
				}
				if (tb==TOKEN_NOT_DEFINED || tb==TOKEN_ALPHA) {
					token.push_back(curr_ch);
					++m_curr_pos;	// 다음 위치
				} else {
					type = TOKEN_STRING;
					return true;
				}
				break;
			case TOKEN_SPACE:
				do {
					++m_curr_pos;	// 공백을 통과

					// 프로그램의 끝을 만나면
					if ( (*m_curr_pos) == '\0' ) {
						type = TOKEN_EOP;
						return false;
					}
				} while (assumeTypeByChar(*m_curr_pos)==TOKEN_SPACE);

				if (tb==TOKEN_NOT_DEFINED) {	// 토큰 획득시
					type = TOKEN_NOT_DEFINED;	// 공백이 앞에 있는 경우
				} else {						// 뒤에 공백 있는 경우
					type = ConvertExternalType(tb);
					return true;
				}
				break;
			case TOKEN_NOT_DEFINED:
				token.clear();
				token.push_back(curr_ch);
				m_curr_pos = NULL;
				return false;
			case TOKEN_EOP:
				if (tb!=TOKEN_NOT_DEFINED) {
					type = ConvertExternalType(tb);
					return true;
				} else {
					return false;
				}
		}
		tb = type;	// 현재 타입을 저장
		curr_ch = (*m_curr_pos);	// 현재 문자열
	}

	if (tb!=TOKEN_NOT_DEFINED) {
		type = ConvertExternalType(tb);
		return true;	// 토근 획득 완료(성공)
	} else {	// 토큰없이 프로그램이 끝났다
		type = TOKEN_EOP;
		return false;
	}
}

bool ToyTokenizer::getInnerProg(std::string& subProg)
{
	subProg.clear();

	char curr_ch = (*m_curr_pos);	// 현재 문자열
	int paren_count = 1;

	while(curr_ch!='\0' && paren_count>0) {	// NULL이 아닐 때까지 반복
		if (curr_ch=='(') {
			paren_count++;
			if (paren_count>1) {	// 중첩된 괄호시
				subProg.push_back(curr_ch);
			}
		} else if (curr_ch==')') {
			if (paren_count>1) {	// 중첩된 괄호시
				subProg.push_back(curr_ch);
			}
			paren_count--;
		} else {
			subProg.push_back(curr_ch);
		}

		m_curr_pos++;
		curr_ch = (*m_curr_pos);	// 현재 문자열
	}

	if (paren_count==0) {
		return true;
	} else {
		return false;
	}
}

int ToyTokenizer::getCurrentIndex() const
{
	int position = m_curr_pos - m_prog;
	return position;
}

//////////////////////////////////////////////////////////////////////////
// private

// 괄호인지 판단
bool is_parenthesis(char ch)
{
	return (ch=='(' || ch==')');
}

// 공백인지 판단
bool is_space(char ch)
{
	return (ch==' ');
}

int ToyTokenizer::assumeTypeByChar(char ch)
{
	if (is_parenthesis(ch)) return TOKEN_PARENTHESIS;

	// ref. http://www.cplusplus.com/reference/cctype/isdigit/
	if (isdigit(ch)!=0) return TOKEN_DIGIT;

	// ref. http://www.cplusplus.com/reference/cctype/isalpha/
	if (isalpha(ch)!=0) return TOKEN_ALPHA;

	if (is_space(ch)) return TOKEN_SPACE;

	if (ch=='-') return TOKEN_NEGATIVE;

	if (ch=='\0') return TOKEN_EOP;

	return TOKEN_NOT_DEFINED;
}


//////////////////////////////////////////////////////////////////////////
const char* TokenTypeToString(int type)
{
	switch(type) {
		case TOKEN_NOT_DEFINED:
			return "Not defined";
		case TOKEN_EOP:
			return "End of Program";
		case TOKEN_NUMBER:
			return "Number";
		case TOKEN_STRING:
			return "String";
		case TOKEN_RESERVED:
			return "Reserved";
		case TOKEN_PARENTHESIS:
			return "Parenthesis";
		case TOKEN_SPACE:
			return "Space";
		default:
			return "Not defined";
	}
}

//////////////////////////////////////////////////////////////////////////
// 파싱을 수행
#ifdef VERBOSE
#include <iostream>
#endif

enum parsing_state {
	normal,		// 기본상태
	terminal,	// 종료상태
	error,		// 파싱에러
	need_operator,	// IF, MINUS 필요 상태
};

bool parse(const char* prog, int* error_pos)
{
	if (prog==NULL) {
		if (error_pos!=NULL) (*error_pos) = 0;
		return false;
	}

	ToyTokenizer tokenizer;
	tokenizer.setProg(prog);

	parsing_state state = normal;
	std::string innerProg;
	int innerErrPos;
	int pb = tokenizer.getCurrentIndex();	// position of before (이전 토큰 위치)

	std::string token;
	int type;
	while( state != error && tokenizer.getToken(token, type) ) {
#ifdef VERBOSE
		std::cout << token << std::endl;
#endif
		switch (state) {
			case normal:	// (, string, number
				switch(type) {
					case TOKEN_STRING:
					case TOKEN_NUMBER:
						state = terminal;
						break;
					case TOKEN_PARENTHESIS:
						pb = tokenizer.getCurrentIndex();	// position of before (이전 토큰 위치)
						if (token=="(" && tokenizer.getInnerProg(innerProg)) {
							if(!parse(innerProg.c_str(), &innerErrPos)) {
		#ifdef VERBOSE
								std::cout << "error pos : " << innerErrPos << std::endl;
		#endif
								if (error_pos!=NULL) {
									*error_pos = pb + innerErrPos;
								}
								return false;
							}
							state = terminal;
						} else {
							state = error;
						}
						break;
					default:
						state = error;
						break;
				}
				break;
			case terminal:
				switch(type) {
					case TOKEN_STRING:
					case TOKEN_NUMBER:
						state = need_operator;
						break;
					case TOKEN_PARENTHESIS:
						if (token=="(" && tokenizer.getInnerProg(innerProg)) {
							if(!parse(innerProg.c_str(), &innerErrPos)) {
								if (error_pos!=NULL) {
									*error_pos = pb + innerErrPos;
								}
								return false;
							}
							state = need_operator;
						} else {
							state = error;
						}
						break;
					default:
						state = error;
						break;
				}
				break;
			case need_operator:
				switch(type) {
					case TOKEN_RESERVED:
						state = terminal;
						break;
					default:
						state = error;
						break;
				}
				break;
		}

		pb = tokenizer.getCurrentIndex();	// position of before (이전 토큰 위치)
	}

#ifdef VERBOSE
	std::cout << "final state : " << state << std::endl;
#endif
	bool res = (state==terminal);
	if(!res && error_pos) {
		(*error_pos) = pb;
	}
	return res;
}

bool make_im_code(const char* prog, std::vector<std::string>& out)
{
	out.clear();

	if(!parse(prog)) return false;
	
	// 중간코드로 변경
	ToyTokenizer tokenizer;
	tokenizer.setProg(prog);

	std::string token;
	int type;

	out.push_back("begin");
	while( tokenizer.getToken(token, type) ) {
		switch(type) {
			case TOKEN_NUMBER:
			case TOKEN_STRING:
				out.push_back("push " + token);
				break;
			case TOKEN_RESERVED:
				out.push_back(token);
				break;
		}
	}
	out.push_back("end");

	return true;
}

#include <stack>

bool postfix_to_prefix(const char* prog, std::string& out)
{
	if (prog==NULL) {
		return false;
	}

	ToyTokenizer tokenizer;
	tokenizer.setProg(prog);
	parsing_state state = normal;
	std::stack<std::string> stck;
	std::string op1, op2;
	std::string innerProg;

	std::string token;
	int type;
	while( state != error && tokenizer.getToken(token, type) ) {
		switch (state) {
			case normal:	// (, string, number
				switch(type) {
					case TOKEN_STRING:
					case TOKEN_NUMBER:
						stck.push(token);
						state = terminal;
						break;
					case TOKEN_PARENTHESIS:
						if (token=="(" && tokenizer.getInnerProg(innerProg)) {
#ifdef VERBOSE
							std::cout << innerProg << std::endl;
#endif
							std::string tmp;
							if (postfix_to_prefix(innerProg.c_str(), tmp)) {
								stck.push("(" + tmp + ")");
								state = terminal;
							} else {
								state = error;
							}
						}
						break;
				}
				break;
			case terminal:
				switch(type) {
					case TOKEN_STRING:
					case TOKEN_NUMBER:
						stck.push(token);
						state = need_operator;
						break;
					case TOKEN_PARENTHESIS:
						if (token=="(" && tokenizer.getInnerProg(innerProg)) {
#ifdef VERBOSE
							std::cout << innerProg << std::endl;
#endif
							std::string tmp;
							if (postfix_to_prefix(innerProg.c_str(), tmp)) {
								stck.push("(" + tmp + ")");
								state = need_operator;
							} else {
								state = error;
							}
						}
						break;
				}
				break;
			case need_operator:
				switch(type) {
					case TOKEN_RESERVED:
						out.append(token);
						out.push_back(' ');
						op2 = stck.top();
						stck.pop();
						out.append(stck.top());
						stck.pop();
						out.push_back(' ');
						out.append(op2);
						state = terminal;
						break;
				}
				break;
		}
	}

	bool res = (state==terminal);
	if(res && !stck.empty()) {
		while(!stck.empty()) {
			out.append(stck.top());
			stck.pop();
		}
	}

	return res;
}