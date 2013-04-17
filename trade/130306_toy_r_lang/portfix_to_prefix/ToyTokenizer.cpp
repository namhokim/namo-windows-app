#include "StdAfx.h"
#include "ToyTokenizer.h"
#include <stack>	// ǥ��� ��ȯ�� ���
#include <cmath>	// log

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
		case TOKEN_DOT:
			return TOKEN_FLOAT_PTR;
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
	m_curr_pos = m_prog;
}

// ��ū ȹ�� : �ٽ�1
int ToyTokenizer::getToken(std::string& token, int& type)
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
					return NON_ERROR;	// ��� ȹ�� �Ϸ�(����)
				}
				// ó�� ��ȣ�� ������ ��
				else {
					token.push_back(curr_ch);
					++m_curr_pos;	// ���� ��ġ ����
					return NON_ERROR;
				}
			case TOKEN_DIGIT:
				switch(tb) {
					case TOKEN_STRING:		// ���ڴ��� ����
						type = TOKEN_STRING;	// Ÿ���� ���ڷ� ����
					case TOKEN_NOT_DEFINED:	// ó�� �����ϳ�
					case TOKEN_DIGIT:		// ���ڴ��� ����
					case TOKEN_DOT:			// ��(.)���� ����
						token.push_back(curr_ch);
						++m_curr_pos;	// ���� ��ġ
						break;
					default:
						break;
				}
				break;
			case TOKEN_NEGATIVE:
				if (tb==TOKEN_NOT_DEFINED) {
					type = TOKEN_DIGIT;	// ù ��° ������ -�� ������ ����
					token.push_back(curr_ch);
					++m_curr_pos;	// ���� ��ġ
				} else {	// ����
					token.clear();
					type = TOKEN_NOT_DEFINED;
					return NON_ERROR;
				}
				break;
			case TOKEN_DOT:
				if (tb==TOKEN_DIGIT || tb==TOKEN_NOT_DEFINED) {	// ����|ó�� ��(.)�� ����
					token.push_back(curr_ch);
					++m_curr_pos;	// ���� ��ġ
				} else {			// ����|ó���̿ܿ� ��(.)�� ����
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
							++m_curr_pos;	// ���� ��ġ
						}
						break;
					case TOKEN_ALPHA:	// ���ڴ����� ����
					case TOKEN_DIGIT:	// ���ڴ����� ����
						token.push_back(curr_ch);
						++m_curr_pos;	// ���� ��ġ
						break;
					default:
						break;
				}
				break;
			case TOKEN_SPACE:
				do {
					++m_curr_pos;	// ������ ���

					// ���α׷��� ���� ������
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

				if (tb==TOKEN_NOT_DEFINED) {	// ��ū ȹ���
					type = TOKEN_NOT_DEFINED;	// ������ �տ� �ִ� ���
				} else {						// �ڿ� ���� �ִ� ���
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

		if (tb!=TOKEN_DOT) tb = type;	// ���� Ÿ���� ����
		curr_ch = (*m_curr_pos);		// ���� ���ڿ�
	}

	if (tb!=TOKEN_NOT_DEFINED) {
		type = ConvertExternalType(tb);
		return NON_ERROR;	// ��� ȹ�� �Ϸ�(����)
	} else {	// ��ū���� ���α׷��� ������
		type = TOKEN_EOP;
		return EOP_ERROR;
	}
}

int ToyTokenizer::getInnerProg(std::string& subProg)
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

	if (paren_count<0) {	// (�� ����
		return L_PAREN_ERROR;
	} else if (paren_count==0) {
		return NON_ERROR;
	} else {				// )�� ����
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
// �Ľ̰���

// �Ľ̽� ����
enum parsing_state {
	normal,		// �⺻����
	terminal,	// �������
	error,		// �Ľ̿���
	need_operator,	// IF, MINUS �ʿ� ����
};

// �߰��ڵ� ����
int make_im_code(const char* prog, std::vector<std::string>& out)
{
	out.clear();	// �Է��� �����Ѵ�

	// ����üũ
	std::string prefix;
	int res = postfix_to_prefix(prog, prefix);
	if(res!=NON_ERROR) {
		return res;
	}
	
	// �߰��ڵ�� ����
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

// ����üũ : �ٽ�2
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
			case TOKEN_FLOAT_PTR:	// �Ǽ��� ��ū���� ������, ������ ������
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
								} else {	// ���� ��ȣ �ȿ��� ���� �߻�
									state = error;	// �� �̻� ������ �ʿ� ����
									tok_res = inner_res;
								}
							}
						}
						else {	// ó������ ��ȣ�� ���� ���� ����.
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
								} else {	// ���� ��ȣ �ȿ��� ���� �߻�
									state = error;	// �� �̻� ������ �ʿ� ����
									tok_res = inner_res;
								}
							}
						}
						else {	// ó������ ��ȣ�� ���� ���� ����.
							state = error;
							tok_res = L_PAREN_ERROR;
						}
						break;
					case TOKEN_RESERVED:	// eg. (1 MINUS)
						state = error;
						tok_res = OPERAND_ERRPR;
						break;
					case TOKEN_EOP:			// �� ������
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
					default:	// ������ ����
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

	if(res==NON_ERROR && !stck.empty()) {	// �����̸�
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
	} else {	// ����
		// ���α״� ����θ� ���� �� �����Ƿ�
		// ����� ��ȯ�ϰ�, '-'�� ���̸� ����� +2�� �߰���
		int digit = int(log10(double(-num))) +2;
		size_t len = str.length();
		return (digit==len);
	}
}

// ����: NON_ERROR
// ������ ����(MINUS, IF)
int operate(int op, std::stack<int>& stck, int& result)
{
	if (stck.size()<2) return OPERAND_ERRPR;	// �� �� �̻��� ���ÿ� �־�� ��

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
				if (!SameUnitOfDigit(num, i)) return UNDEFINED_ERROR;	// �ǿ����ڷ� ���ڰ� ���Դ�
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
			return "�۾��� �Ϸ��߽��ϴ�.";
		case L_PAREN_ERROR:
			return "'('��ȣ�� ����� �߸��Ǿ����ϴ�.";
		case R_PAREN_ERROR:
			return "')'��ȣ�� ����� �߸��Ǿ����ϴ�.";
		case OPERATOR_ERROR:
			return "�����ڿ����Դϴ�.";
		case OPERAND_ERRPR:
			return "�ǿ����ڿ����Դϴ�.";
		case FLOAT_P_ERROR:
			return "�Ǽ��� �Է��ϼ̽��ϴ�.";
		case SYMBOL_ERROR:
			return "�߸��� ��ȣ�� �ԷµǾ����ϴ�.";
		case UNDEFINED_ERROR:
			return "Undefined";
		case NO_PROG_ERROR:
			return "���α׷��� �����ϴ�.";
		case NO_BEGIN_ERROR:
			return "�߰��ڵ尡 begin���� �������� �ʽ��ϴ�.";
		case NO_END_ERROR:
			return "�߰��ڵ尡 end�� ������ �ʽ��ϴ�.";
		case BAD_IC_ERROR:
			return "�߸��� �߰��ڵ��Դϴ�.";
		default:
			return "�� �� ���� �����ڵ��Դϴ�.";
	}
}

const wchar_t* ErrorCodeToStringW(int code)
{
	switch(code) {
		case NON_ERROR:
			return L"�۾��� �Ϸ��߽��ϴ�.";
		case L_PAREN_ERROR:
			return L"'('��ȣ�� ����� �߸��Ǿ����ϴ�.";
		case R_PAREN_ERROR:
			return L"')'��ȣ�� ����� �߸��Ǿ����ϴ�.";
		case OPERATOR_ERROR:
			return L"�����ڿ����Դϴ�.";
		case OPERAND_ERRPR:
			return L"�ǿ����ڿ����Դϴ�.";
		case FLOAT_P_ERROR:
			return L"�Ǽ��� �Է��ϼ̽��ϴ�.";
		case SYMBOL_ERROR:
			return L"�߸��� ��ȣ�� �ԷµǾ����ϴ�.";
		case UNDEFINED_ERROR:
			return L"Undefined";
		case NO_PROG_ERROR:
			return L"���α׷��� �����ϴ�.";
		case NO_BEGIN_ERROR:
			return L"�߰��ڵ尡 begin���� �������� �ʽ��ϴ�.";
		case NO_END_ERROR:
			return L"�߰��ڵ尡 end�� ������ �ʽ��ϴ�.";
		case BAD_IC_ERROR:
			return L"�߸��� �߰��ڵ��Դϴ�.";
		default:
			return L"�� �� ���� �����ڵ��Դϴ�.";
	}
}