#include "StdAfx.h"
#include "ToyTokenizer.h"

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

	while(m_curr_pos) {	// NULL�� �ƴ� ������ �ݺ�
		char curr_ch = (*m_curr_pos);	// ���� ���ڿ�
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
					// ������ ���
					++m_curr_pos;

					// ���α׷��� ���� ������
					if (m_curr_pos==NULL) {
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
			case TOKEN_EOP:
				if (tb!=TOKEN_NOT_DEFINED) {
					type = ConvertExternalType(tb);
					return true;
				} else {
					return false;
				}
		}
		tb = type;	// ���� Ÿ���� ����
	}

	// ���α׷��� ������
	type = TOKEN_EOP;
	return false;
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

	return TOKEN_EOP;
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
