#include "StdAfx.h"
#include "ToyTokenizer.h"
#include <stack>	// 표기법 변환시 사용
#include <cmath>	// log

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
		case TOKEN_DOT:
			return TOKEN_FLOAT_PTR;
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
	m_curr_pos = m_prog;
}

// 토큰 획득 : 핵심1
int ToyTokenizer::getToken(std::string& token, int& type)
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
					return NON_ERROR;	// 토근 획득 완료(성공)
				}
				// 처음 괄호를 만났을 때
				else {
					token.push_back(curr_ch);
					++m_curr_pos;	// 다음 위치 수정
					return NON_ERROR;
				}
			case TOKEN_DIGIT:
				switch(tb) {
					case TOKEN_STRING:		// 문자다음 숫자
						type = TOKEN_STRING;	// 타입을 문자로 간주
					case TOKEN_NOT_DEFINED:	// 처음 숫자하나
					case TOKEN_DIGIT:		// 숫자다음 숫자
					case TOKEN_DOT:			// 점(.)다음 숫자
						token.push_back(curr_ch);
						++m_curr_pos;	// 다음 위치
						break;
					default:
						break;
				}
				break;
			case TOKEN_NEGATIVE:
				if (tb==TOKEN_NOT_DEFINED) {
					type = TOKEN_DIGIT;	// 첫 번째 나오는 -는 음수로 간주
					token.push_back(curr_ch);
					++m_curr_pos;	// 다음 위치
				} else {	// 에러
					token.clear();
					type = TOKEN_NOT_DEFINED;
					return NON_ERROR;
				}
				break;
			case TOKEN_DOT:
				if (tb==TOKEN_DIGIT || tb==TOKEN_NOT_DEFINED) {	// 숫자|처음 점(.)이 나옴
					token.push_back(curr_ch);
					++m_curr_pos;	// 다음 위치
				} else {			// 숫자|처음이외에 점(.)이 나옴
					token.clear();
					type = TOKEN_NOT_DEFINED;
					return SYMBOL_ERROR;
				}
				break;
			case TOKEN_ALPHA:
				switch(tb) {
					case TOKEN_DOT:
						type = TOKEN_NOT_DEFINED;
						return SYMBOL_ERROR;
					case TOKEN_NOT_DEFINED:
						if(IsReservedWord(m_curr_pos, token)) {
							type = TOKEN_RESERVED;
							m_curr_pos += token.size();
							return NON_ERROR;
						} else {
							token.push_back(curr_ch);
							++m_curr_pos;	// 다음 위치
						}
						break;
					case TOKEN_ALPHA:	// 문자다음에 문자
					case TOKEN_DIGIT:	// 숫자다음에 문자
						token.push_back(curr_ch);
						++m_curr_pos;	// 다음 위치
						break;
					default:
						break;
				}
				break;
			case TOKEN_SPACE:
				do {
					++m_curr_pos;	// 공백을 통과

					// 프로그램의 끝을 만나면
					if ( (*m_curr_pos) == '\0' ) {
						if (tb==TOKEN_NOT_DEFINED) {
							type = TOKEN_EOP;
							return EOP_ERROR;
						} else {
							type = ConvertExternalType(tb);
							return NON_ERROR;
						}
					}
				} while (assumeTypeByChar(*m_curr_pos)==TOKEN_SPACE);

				if (tb==TOKEN_NOT_DEFINED) {	// 토큰 획득시
					type = TOKEN_NOT_DEFINED;	// 공백이 앞에 있는 경우
				} else {						// 뒤에 공백 있는 경우
					type = ConvertExternalType(tb);
					return NON_ERROR;
				}
				break;
			case TOKEN_NOT_DEFINED:
				token.clear();
				token.push_back(curr_ch);
				m_curr_pos = NULL;
				return SYMBOL_ERROR;
			case TOKEN_EOP:
				if (tb!=TOKEN_NOT_DEFINED) {
					type = ConvertExternalType(tb);
					return NON_ERROR;
				} else {
					return EOP_ERROR;
				}
		}

		if (tb!=TOKEN_DOT) tb = type;	// 현재 타입을 저장
		curr_ch = (*m_curr_pos);		// 현재 문자열
	}

	if (tb!=TOKEN_NOT_DEFINED) {
		type = ConvertExternalType(tb);
		return NON_ERROR;	// 토근 획득 완료(성공)
	} else {	// 토큰없이 프로그램이 끝났다
		type = TOKEN_EOP;
		return EOP_ERROR;
	}
}

int ToyTokenizer::getInnerProg(std::string& subProg)
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

	if (paren_count<0) {	// (가 많다
		return L_PAREN_ERROR;
	} else if (paren_count==0) {
		return NON_ERROR;
	} else {				// )가 많다
		return R_PAREN_ERROR;
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

	if (ch=='.') return TOKEN_DOT;

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
// 파싱관련

// 파싱시 상태
enum parsing_state {
	normal,		// 기본상태
	terminal,	// 종료상태
	error,		// 파싱에러
	need_operator,	// IF, MINUS 필요 상태
};

// 중간코드 생성
int make_im_code(const char* prog, std::vector<std::string>& out)
{
	out.clear();	// 입력을 정리한다

	// 문법체크
	std::string prefix;
	int res = postfix_to_prefix(prog, prefix);
	if(res!=NON_ERROR) {
		return res;
	}
	
	// 중간코드로 변경
	ToyTokenizer tokenizer;
	tokenizer.setProg(prog);

	std::string token;
	int type;
	bool bCont = true;	// continue

	out.push_back("begin");
	while( bCont && tokenizer.getToken(token, type)==NON_ERROR ) {
		switch(type) {
			case TOKEN_NUMBER:
			case TOKEN_STRING:
				out.push_back("push " + token);
				break;
			case TOKEN_RESERVED:
				out.push_back(token);
				break;
			case TOKEN_EOP:
				bCont = false;
				break;
		}
	}
	out.push_back("end");

	return NON_ERROR;
}

// 문법체크 : 핵심2
int postfix_to_prefix(const char* prog, std::string& out)
{
	if (prog==NULL) return NO_PROG_ERROR;

	ToyTokenizer tokenizer;
	tokenizer.setProg(prog);
	parsing_state state = normal;
	std::stack<std::string> stck;
	std::string op1, op2;
	std::string innerProg;

	std::string token;
	int tok_res, type;
	bool bCont = true;	// continue
	while( bCont && state != error && (tok_res=tokenizer.getToken(token, type))==NON_ERROR) {
		switch(type) {
			case TOKEN_FLOAT_PTR:	// 실수는 토큰으로 보지만, 문법상 허용안함
				tok_res = FLOAT_P_ERROR;
				state = error;
				break;
			case TOKEN_NOT_DEFINED:
				tok_res = SYMBOL_ERROR;
				state = error;
				break;
		}

		switch (state) {
			case normal:	// (, string, number
				switch(type) {
					case TOKEN_STRING:
					case TOKEN_NUMBER:
						stck.push(token);
						state = terminal;
						break;
					case TOKEN_PARENTHESIS:
						if (token=="(") {
							int inner_res = tokenizer.getInnerProg(innerProg);
							if (inner_res!=NON_ERROR) {
								tok_res = inner_res;
								state = error;
							} else {
								std::string tmp;
								inner_res = postfix_to_prefix(innerProg.c_str(), tmp);
								if (inner_res==NON_ERROR) {
									stck.push("(" + tmp + ")");
									state = terminal;
									innerProg.clear();
								} else {	// 내부 괄호 안에서 에러 발생
									state = error;	// 더 이상 진행할 필요 없음
									tok_res = inner_res;
								}
							}
						}
						else {	// 처음부터 괄호를 닫을 수는 없다.
							state = error;
							tok_res = R_PAREN_ERROR;
						}
						break;
					default:
						state = error;
						tok_res = UNDEFINED_ERROR;
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
						if (token=="(") {
							int inner_res = tokenizer.getInnerProg(innerProg);
							if (inner_res!=NON_ERROR) {
								tok_res = inner_res;
								state = error;
							} else {
								std::string tmp;
								inner_res = postfix_to_prefix(innerProg.c_str(), tmp);
								if (inner_res==NON_ERROR) {
									stck.push("(" + tmp + ")");
									state = need_operator;
									innerProg.clear();
								} else {	// 내부 괄호 안에서 에러 발생
									state = error;	// 더 이상 진행할 필요 없음
									tok_res = inner_res;
								}
							}
						}
						else {	// 처음부터 괄호를 닫을 수는 없다.
							state = error;
							tok_res = L_PAREN_ERROR;
						}
						break;
					case TOKEN_RESERVED:	// eg. (1 MINUS)
						state = error;
						tok_res = OPERAND_ERRPR;
						break;
					case TOKEN_EOP:			// 잘 끝났다
						tok_res = NON_ERROR;
						bCont = false;
						break;
					default:
						state = error;
						tok_res = UNDEFINED_ERROR;
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
					case TOKEN_STRING:
						state = error;
						tok_res = UNDEFINED_ERROR;
						break;
					default:	// 연산자 오류
						state = error;
						tok_res = OPERATOR_ERROR;
						break;
				}
				break;
		}
	}

	int res;
	if (state==terminal && tok_res==EOP_ERROR) {
		res = NON_ERROR;
	} else {
		res = tok_res;
	}

	if(res==NON_ERROR && !stck.empty()) {	// 성공이면
		while(!stck.empty()) {
			out.append(stck.top()); 
			stck.pop();
		}
	}

	return res;
}

bool SameUnitOfDigit(const std::string& str, int num)
{
	if (num==0) {
		return ("0"==str);
	} else if (num>0) {
		int digit = int(log10(double(num))) +1; 
		size_t len = str.length();
		return (digit==len);
	} else {	// 음수
		// 상용로그는 양수로만 취할 수 있으므로
		// 양수로 변환하고, '-'의 길이를 고려해 +2를 추가함
		int digit = int(log10(double(-num))) +2;
		size_t len = str.length();
		return (digit==len);
	}
}

// 정상: NON_ERROR
// 연산을 수행(MINUS, IF)
int operate(int op, std::stack<int>& stck, int& result)
{
	if (stck.size()<2) return OPERAND_ERRPR;	// 두 개 이상이 스택에 있어야 함

	int op1, op2;			// operands
	op2 = stck.top();
	stck.pop();
	op1 = stck.top();
	stck.pop();
	result = op1-op2;

	switch(op) {
		case OP_MINUS:
			result = op1 - op2;
			break;
		case OP_IF:
			result = (op1 > 0 ? op2 : 0);
			break;
		default:
			return OPERATOR_ERROR;
	}

	return NON_ERROR;
}

int evaluation(const std::vector<std::string>& in, int& out)
{
	size_t ps = in.size();	// program size
	if (ps<2) return BAD_IC_ERROR;

	if (in[0]!="begin") return NO_BEGIN_ERROR;
	if (in[ps-1]!="end") return NO_END_ERROR;

	std::stack<int> stck;
	int result;				// result of operation
	int op_res;				// success or fail of operation
	size_t pc;				// program counter

	for (pc=1; pc<(ps-1); ++pc) {	// begin < ~~ < end
		if (in[pc]=="MINUS") {
			op_res = operate(OP_MINUS, stck, result);
			if (op_res==NON_ERROR) stck.push(result);
			else return op_res;
		} else if (in[pc]=="IF") {
			op_res = operate(OP_IF, stck, result);
			if (op_res==NON_ERROR) stck.push(result);
			else return op_res;
		} else {	// push
			if (in[pc].find_first_of("push ") != std::string::npos) {
				std::string num = in[pc].substr(5);
				int i = atoi(num.c_str());
				if (!SameUnitOfDigit(num, i)) return UNDEFINED_ERROR;	// 피연산자로 문자가 들어왔다
				stck.push(i);
			} else {
				return UNDEFINED_ERROR;
			}

		}
	}

	if (stck.size()==1) {
		out = stck.top();
		return NON_ERROR;
	} else {
		return false;
	}
}

const char* ErrorCodeToStringA(int code)
{
	switch(code) {
		case NON_ERROR:
			return "작업을 완료했습니다.";
		case L_PAREN_ERROR:
			return "'('괄호의 사용이 잘못되었습니다.";
		case R_PAREN_ERROR:
			return "')'괄호의 사용이 잘못되었습니다.";
		case OPERATOR_ERROR:
			return "연산자오류입니다.";
		case OPERAND_ERRPR:
			return "피연산자오류입니다.";
		case FLOAT_P_ERROR:
			return "실수를 입력하셨습니다.";
		case SYMBOL_ERROR:
			return "잘못된 기호가 입력되었습니다.";
		case UNDEFINED_ERROR:
			return "Undefined";
		case NO_PROG_ERROR:
			return "프로그램이 없습니다.";
		case NO_BEGIN_ERROR:
			return "중간코드가 begin으로 시작하지 않습니다.";
		case NO_END_ERROR:
			return "중간코드가 end로 끝나지 않습니다.";
		case BAD_IC_ERROR:
			return "잘못된 중간코드입니다.";
		default:
			return "알 수 없는 에러코드입니다.";
	}
}

const wchar_t* ErrorCodeToStringW(int code)
{
	switch(code) {
		case NON_ERROR:
			return L"작업을 완료했습니다.";
		case L_PAREN_ERROR:
			return L"'('괄호의 사용이 잘못되었습니다.";
		case R_PAREN_ERROR:
			return L"')'괄호의 사용이 잘못되었습니다.";
		case OPERATOR_ERROR:
			return L"연산자오류입니다.";
		case OPERAND_ERRPR:
			return L"피연산자오류입니다.";
		case FLOAT_P_ERROR:
			return L"실수를 입력하셨습니다.";
		case SYMBOL_ERROR:
			return L"잘못된 기호가 입력되었습니다.";
		case UNDEFINED_ERROR:
			return L"Undefined";
		case NO_PROG_ERROR:
			return L"프로그램이 없습니다.";
		case NO_BEGIN_ERROR:
			return L"중간코드가 begin으로 시작하지 않습니다.";
		case NO_END_ERROR:
			return L"중간코드가 end로 끝나지 않습니다.";
		case BAD_IC_ERROR:
			return L"잘못된 중간코드입니다.";
		default:
			return L"알 수 없는 에러코드입니다.";
	}
}