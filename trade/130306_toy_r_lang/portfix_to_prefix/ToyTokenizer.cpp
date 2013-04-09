#include "StdAfx.h"
#include "ToyTokenizer.h"

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
	if(m_current_position==NULL) {
		type = TOY_R_TOKEN_EOP;
		return false;
	}

	int type_before = TOY_R_TOKEN_NOT_DEFINED;
	while(m_current_position) {	// NULL�� �ƴ� ������ �ݺ�
		char curr_ch = (*m_current_position);	// ���� ���ڿ�
		type = assumeTypeByChar(curr_ch);
		switch(type) {
			case TOY_R_TOKEN_PARENTHESIS:
				token.push_back(curr_ch);
				++m_current_position;	// ���� ��ġ
				return true;
			case TOY_R_TOKEN_DIGIT:
				if (type_before==TOY_R_TOKEN_NOT_DEFINED
					|| type_before==TOY_R_TOKEN_DIGIT)
				{
					token.push_back(curr_ch);
					++m_current_position;	// ���� ��ġ
					type_before = type;
					break;
				} else {
					type = TOY_R_TOKEN_NUMBER;
					return true;
				}
			case TOY_R_TOKEN_EOP:
				type = type_before;
				return true;
		}
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

int ToyTokenizer::assumeTypeByChar(char ch)
{
	if (is_parenthesis(ch)) return TOY_R_TOKEN_PARENTHESIS;

	// ref. http://www.cplusplus.com/reference/cctype/isdigit/
	if (isdigit(ch)!=0) return TOY_R_TOKEN_DIGIT;

	return TOY_R_TOKEN_EOP;
}