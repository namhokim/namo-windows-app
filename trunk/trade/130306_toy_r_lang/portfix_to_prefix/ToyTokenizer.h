#pragma once

#include <string>

class ToyTokenizer
{
public:
	ToyTokenizer(void);
	~ToyTokenizer(void);
	void setProg(const char* prog);	// ���α׷�(���ڿ�)�� �����Ѵ�
	bool getToken(std::string& token, int& type);	// ��ū�� �����´�

private:
	const char* m_prog;
};
