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
	const char* m_prog;			// 프로그램을 가리키는 문자열 포인터(NULL 종료)
	const char* m_current_position;	// 파싱하는 위치(NULL이면 끝, 초기화 필요)
};
