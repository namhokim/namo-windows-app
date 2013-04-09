#include "StdAfx.h"
#include "ToyTokenizer.h"

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
		case TOY_R_TOKEN_DIGIT:
			return TOY_R_TOKEN_NUMBER;
		case TOY_R_TOKEN_ALPHA:
			return TOY_R_TOKEN_STRING;
		default:
			return type;
	}
}

bool IsReservedWord(const char* ptr, std::string& word)
{
	if (strncmp(Toy_R_Reserved::IF::str, ptr, Toy_R_Reserved::IF::size)==0) {
		word.assign(Toy_R_Reserved::IF::str);
		return true;
	}
	if (strncmp(Toy_R_Reserved::MINUS::str, ptr, Toy_R_Reserved::MINUS::size)==0) {
		word.assign(Toy_R_Reserved::MINUS::str);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// public

ToyTokenizer::ToyTokenizer(void)
:m_current_position(NULL)
{
}

ToyTokenizer::~ToyTokenizer(void)
{
}

void ToyTokenizer::setProg(const char* prog)
{
	m_prog = prog;
	if (prog==NULL) throw new std::invalid_argument("Not allow, NULL value in prog parameter.");

	m_current_position = m_prog;
}

bool ToyTokenizer::getToken(std::string& token, int& type)
{
	// �ʱ�ȭ
	token.clear();
	type = TOY_R_TOKEN_NOT_DEFINED;

	if(m_current_position==NULL) {
		type = TOY_R_TOKEN_EOP;
		return false;
	}

	int type_before = TOY_R_TOKEN_NOT_DEFINED;
	int type_internal;

	while(m_current_position) {	// NULL�� �ƴ� ������ �ݺ�
		char curr_ch = (*m_current_position);	// ���� ���ڿ�
		type = assumeTypeByChar(curr_ch);
		switch(type) {
			case TOY_R_TOKEN_PARENTHESIS:
				if (type_before!=TOY_R_TOKEN_NOT_DEFINED
					&& type_before!=TOY_R_TOKEN_PARENTHESIS)
				{
					type = ConvertExternalType(type_before);
					return true;
				} else {
					token.push_back(curr_ch);
					++m_current_position;	// ���� ��ġ
					return true;
				}
			case TOY_R_TOKEN_DIGIT:
				if (type_before==TOY_R_TOKEN_NOT_DEFINED
					|| type_before==TOY_R_TOKEN_DIGIT)
				{
					token.push_back(curr_ch);
					++m_current_position;	// ���� ��ġ
					break;
				} else {
					type = TOY_R_TOKEN_NUMBER;
					return true;
				}
			case TOY_R_TOKEN_ALPHA:
				if (type_before==TOY_R_TOKEN_NOT_DEFINED
					&& IsReservedWord(m_current_position, token)) {
					type = TOY_R_TOKEN_RESERVED;
					m_current_position += token.size();
					return true;
				} 
				if (type_before==TOY_R_TOKEN_NOT_DEFINED
					|| type_before==TOY_R_TOKEN_ALPHA) {
					token.push_back(curr_ch);
					++m_current_position;	// ���� ��ġ
				} else {
					type = TOY_R_TOKEN_STRING;
					return true;
				}
				break;
			case TOY_R_TOKEN_SPACE:
				// ������ �����ϰ�
				do {
					++m_current_position;
					if (m_current_position==NULL) {
						type = TOY_R_TOKEN_EOP;
						return false;
					}
					char c = (*m_current_position);
					type_internal = assumeTypeByChar(c);
				} while (type_internal==TOY_R_TOKEN_SPACE);

				if (type_before!=TOY_R_TOKEN_NOT_DEFINED) {	// ó���̶��
					type = ConvertExternalType(type_before);
					return true;
				}
				break;
			case TOY_R_TOKEN_EOP:
				if (type_before!=TOY_R_TOKEN_NOT_DEFINED) {
					type = ConvertExternalType(type_before);
					return true;
				} else {
					return false;
				}
		}
		type_before = type;	// ���� Ÿ���� ����
	}

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
	if (is_parenthesis(ch)) return TOY_R_TOKEN_PARENTHESIS;

	// ref. http://www.cplusplus.com/reference/cctype/isdigit/
	if (isdigit(ch)!=0) return TOY_R_TOKEN_DIGIT;

	// ref. http://www.cplusplus.com/reference/cctype/isalpha/
	if (isalpha(ch)!=0) return TOY_R_TOKEN_ALPHA;

	if (is_space(ch)) return TOY_R_TOKEN_SPACE;

	return TOY_R_TOKEN_EOP;
}


//////////////////////////////////////////////////////////////////////////
const char* TokenTypeToString(int type)
{
	switch(type) {
		case TOY_R_TOKEN_NOT_DEFINED:
			return "Not defined";
		case TOY_R_TOKEN_EOP:
			return "End of Program";
		case TOY_R_TOKEN_NUMBER:
			return "Number";
		case TOY_R_TOKEN_STRING:
			return "String";
		case TOY_R_TOKEN_RESERVED:
			return "Reserved";
		case TOY_R_TOKEN_PARENTHESIS:
			return "Parenthesis";
		case TOY_R_TOKEN_SPACE:
			return "Space";
		default:
			return "Not defined";
	}
}
