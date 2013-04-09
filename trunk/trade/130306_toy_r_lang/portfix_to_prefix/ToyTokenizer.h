#pragma once

#define TOY_R_TOKEN_NOT_DEFINED	(-1)
#define TOY_R_TOKEN_EOP			0	// end of program
#define TOY_R_TOKEN_NUMBER		1	// group
#define TOY_R_TOKEN_DIGIT		2	// single (internal)
#define TOY_R_TOKEN_PARENTHESIS	3
#define TOY_R_TOKEN_STRING		4	// group
#define TOY_R_TOKEN_ALPHA		5	// single (internal)
#define TOY_R_TOKEN_RESERVED	6	// �����(IF, MINUS)
#define TOY_R_TOKEN_SPACE		7	// ���� (internal)

#include <string>

class ToyTokenizer
{
public:
	ToyTokenizer(void);
	~ToyTokenizer(void);
	void setProg(const char* prog);	// ���α׷�(���ڿ�)�� �����Ѵ�
	bool getToken(std::string& token, int& type);	// ��ū�� �����´�

private:
	const char* m_prog;			// ���α׷��� ����Ű�� ���ڿ� ������(NULL ����)
	const char* m_current_position;	// �Ľ��ϴ� ��ġ(NULL�̸� ��, �ʱ�ȭ �ʿ�)
	int assumeTypeByChar(char ch);	// ���� �ϳ��� ������ Ÿ���� ����
};
