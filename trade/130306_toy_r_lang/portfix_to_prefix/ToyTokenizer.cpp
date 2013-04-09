#include "StdAfx.h"
#include "ToyTokenizer.h"

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
				if (type_before==TOY_R_TOKEN_NOT_DEFINED) {
					// ��������� �˻��Ѵ�
					// ������̸� return
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