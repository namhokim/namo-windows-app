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

#define NON_ERROR		0	//	��������
#define EOP_ERROR		0	// ���α׷��� ���� ����
#define L_PAREN_ERROR	1	// '('��ȣ�� ����� �߸��Ǿ����ϴ�
#define R_PAREN_ERROR	2	// ')'��ȣ�� ����� �߸��Ǿ����ϴ�
#define OPERATOR_ERROR	3	// �����ڿ����Դϴ�
#define OPERAND_ERRPR	4	// �ǿ����ڿ����Դϴ�
#define FLOAT_P_ERROR	5	// �Ǽ��� �Է��ϼ̽��ϴ�
#define SYMBOL_ERROR	6	// �߸��� ��ȣ�� �ԷµǾ����ϴ�
#define UNDEFINED_ERROR	7	// Undefined
#define NO_PROG_ERROR	8	// ���α׷��� ����
#define NO_BEGIN_ERROR	9	// �߰��ڵ尡 begin���� �������� ����
#define NO_END_ERROR	10	// �߰��ڵ尡 end�� ������ ����
#define BAD_IC_ERROR	11	// �߸��� �߰��ڵ�

#include <string>
#include <vector>

class ToyTokenizer
{
public:
	ToyTokenizer(void);
	~ToyTokenizer(void);
	void setProg(const char* prog);	// ���α׷�(���ڿ�)�� �����Ѵ�
	int getToken(std::string& token, int& type);	// ��ū�� �����´�
	int getInnerProg(std::string& subProg);	// ���� �ݴ� ��ȣ�� ã�� ������ ������ ȹ��
	int getCurrentIndex() const;				// ���� tokenize ��ġ�� �����´�(0~N)

private:
	const char* m_prog;			// ���α׷��� ����Ű�� ���ڿ� ������(NULL ����)
	const char* m_curr_pos;		// �Ľ��ϴ� ������ġ(NULL�̸� ��, �ʱ�ȭ �ʿ�)
	int assumeTypeByChar(char ch);	// ���� �ϳ��� ������ Ÿ���� ����
};

const char* TokenTypeToString(int type);
int make_im_code(const char* prog, std::vector<std::string>& out);	// �߰��ڵ� ����
int postfix_to_prefix(const char* prog, std::string& out);		// ����->������ ��ȯ
int evaluation(const std::vector<std::string>& in, int& out);	// �߰��ڵ带 ���ϴ� �Լ�


const char* ErrorCodeToStringA(int code);
const wchar_t* ErrorCodeToStringW(int code);

#ifdef UNICODE
#define ErrorCodeToString ErrorCodeToStringW
#else
#define ErrorCodeToString ErrorCodeToStringA
#endif