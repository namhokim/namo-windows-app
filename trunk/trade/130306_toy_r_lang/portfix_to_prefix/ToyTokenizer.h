#pragma once

class ToyTokenizer
{
public:
	ToyTokenizer(void);
	~ToyTokenizer(void);
	void setProg(const char* prog);	// 프로그램(문자열)을 지정한다

private:
	const char* m_prog;
};
