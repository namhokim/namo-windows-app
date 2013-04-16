#pragma once

#define TOKEN_NOT_DEFINED	(-1)
#define TOKEN_EOP			0	// end of program
#define TOKEN_NUMBER		1	// group
#define TOKEN_DIGIT			11	// single (internal)
#define TOKEN_NEGATIVE		12	// single (internal)
#define TOKEN_STRING		2	// group
#define TOKEN_ALPHA			13	// single (internal)
#define TOKEN_RESERVED		3	// �����(IF, MINUS)
#define TOKEN_PARENTHESIS	4	// '(' or ')'
#define TOKEN_SPACE			5	// ���� (internal)
#define TOKEN_FLOAT_PTR		6	// �Ҽ�
#define TOKEN_DOT			14	// �� (internal)

#include <string>
#include <vector>

class ToyTokenizer
{
public:
	ToyTokenizer(void);
	~ToyTokenizer(void);
	void setProg(const char* prog);	// ���α׷�(���ڿ�)�� �����Ѵ�
	bool getToken(std::string& token, int& type);	// ��ū�� �����´�
	bool getInnerProg(std::string& subProg);	// ���� �ݴ� ��ȣ�� ã�� ������ ������ ȹ��
	int getCurrentIndex() const;				// ���� tokenize ��ġ�� �����´�(0~N)

private:
	const char* m_prog;			// ���α׷��� ����Ű�� ���ڿ� ������(NULL ����)
	const char* m_curr_pos;		// �Ľ��ϴ� ������ġ(NULL�̸� ��, �ʱ�ȭ �ʿ�)
	int assumeTypeByChar(char ch);	// ���� �ϳ��� ������ Ÿ���� ����
};

const char* TokenTypeToString(int type);
bool parse(const char* prog, int* error_pos = NULL);	// �Ľ��� ����(���� üũ)
bool make_im_code(const char* prog, std::vector<std::string>& out);	// �߰��ڵ� ����
bool postfix_to_prefix(const char* prog, std::string& out);		// ����->������ ��ȯ
bool evaluation(const std::vector<std::string>& in, int& out);	// �߰��ڵ带 ���ϴ� �Լ�