#include "StdAfx.h"
#include "ToyTokenizer.h"

//#define VERBOSE

// ����� ���
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


// �ܺο��� ����ϴ� Ÿ������ ��ȯ���ִ� ��ƿ��Ƽ �Լ�
// ��) �� ������ ����(digit) -> ����(number)
// ��) �� ������ ����(alphabet) -> ���ڿ�(string)
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

// ��������� �Ǵ��Ѵ�.
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
	// ��� ���� �ʱ�ȭ
	token.clear();
	type = TOKEN_NOT_DEFINED;

	int tb = TOKEN_NOT_DEFINED;	// type of berfore(���� ���¸� ����)
	char curr_ch = (*m_curr_pos);	// ���� ���ڿ�

	while(curr_ch!='\0') {	// NULL�� �ƴ� ������ �ݺ�
		
		type = assumeTypeByChar(curr_ch);
		switch(type) {
			case TOKEN_PARENTHESIS:
				// �ٸ� Ÿ�Կ��� ��ȣ�� ������ ��
				if (tb!=TOKEN_NOT_DEFINED && tb!=TOKEN_PARENTHESIS) {
					type = ConvertExternalType(tb);
					return true;	// ��� ȹ�� �Ϸ�(����)
				}
				// ó�� ��ȣ�� ������ ��
				else {
					token.push_back(curr_ch);
					++m_curr_pos;	// ���� ��ġ ����
					return true;
				}
			case TOKEN_DIGIT:
				// ó�� �����ϳ�
				if (tb==TOKEN_NOT_DEFINED || tb==TOKEN_DIGIT) {
					token.push_back(curr_ch);
					++m_curr_pos;	// ���� ��ġ
					break;
				} else {
					type = TOKEN_NUMBER;
					return true;
				}
			case TOKEN_NEGATIVE:
				if (tb==TOKEN_NOT_DEFINED) {
					type = TOKEN_DIGIT;	// ù ��° ������ -�� ������ ����
					token.push_back(curr_ch);
					++m_curr_pos;	// ���� ��ġ
				} else {	// ����
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
					++m_curr_pos;	// ���� ��ġ
				} else {
					type = TOKEN_STRING;
					return true;
				}
				break;
			case TOKEN_SPACE:
				do {
					++m_curr_pos;	// ������ ���

					// ���α׷��� ���� ������
					if ( (*m_curr_pos) == '\0' ) {
						type = TOKEN_EOP;
						return false;
					}
				} while (assumeTypeByChar(*m_curr_pos)==TOKEN_SPACE);

				if (tb==TOKEN_NOT_DEFINED) {	// ��ū ȹ���
					type = TOKEN_NOT_DEFINED;	// ������ �տ� �ִ� ���
				} else {						// �ڿ� ���� �ִ� ���
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
		tb = type;	// ���� Ÿ���� ����
		curr_ch = (*m_curr_pos);	// ���� ���ڿ�
	}

	if (tb!=TOKEN_NOT_DEFINED) {
		type = ConvertExternalType(tb);
		return true;	// ��� ȹ�� �Ϸ�(����)
	} else {	// ��ū���� ���α׷��� ������
		type = TOKEN_EOP;
		return false;
	}
}

bool ToyTokenizer::getInnerProg(std::string& subProg)
{
	subProg.clear();

	char curr_ch = (*m_curr_pos);	// ���� ���ڿ�
	int paren_count = 1;

	while(curr_ch!='\0' && paren_count>0) {	// NULL�� �ƴ� ������ �ݺ�
		if (curr_ch=='(') {
			paren_count++;
			if (paren_count>1) {	// ��ø�� ��ȣ��
				subProg.push_back(curr_ch);
			}
		} else if (curr_ch==')') {
			if (paren_count>1) {	// ��ø�� ��ȣ��
				subProg.push_back(curr_ch);
			}
			paren_count--;
		} else {
			subProg.push_back(curr_ch);
		}

		m_curr_pos++;
		curr_ch = (*m_curr_pos);	// ���� ���ڿ�
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

// ��ȣ���� �Ǵ�
bool is_parenthesis(char ch)
{
	return (ch=='(' || ch==')');
}

// �������� �Ǵ�
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
// �Ľ��� ����
#ifdef VERBOSE
#include <iostream>
#endif

enum parsing_state {
	normal,		// �⺻����
	terminal,	// �������
	error,		// �Ľ̿���
	need_operator,	// IF, MINUS �ʿ� ����
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
	int pb = tokenizer.getCurrentIndex();	// position of before (���� ��ū ��ġ)

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
						pb = tokenizer.getCurrentIndex();	// position of before (���� ��ū ��ġ)
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

		pb = tokenizer.getCurrentIndex();	// position of before (���� ��ū ��ġ)
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
	
	// �߰��ڵ�� ����
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