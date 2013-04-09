#pragma once

#include <string>

class ToyTokenizer
{
public:
	ToyTokenizer(void);
	~ToyTokenizer(void);
	void setProg(const char* prog);	// 프로그램(문자열)을 지정한다
	bool getToken(std::string& token, int& type);	// 토큰을 가져온다

private:
	const char* m_prog;
};
