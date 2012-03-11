// TimesNoOneWait.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


const unsigned int OneHour = 3600;
void AddTime();
void Regex();

int _tmain(int argc, _TCHAR* argv[])
{
	AddTime();
	Regex();

	return 0;
}

#include "TimeUtil.h"
#include <Windows.h>
void AddTime()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	Util::TimeUtil tu;
	
	SYSTEMTIME addedSt = tu.addSYSTEMTIME(&st, OneHour);
	return;
}

#include <regex>
using namespace std;
const tr1::regex SinhanPattern("[0-9][0-9][0-9][ ]*-[ ]*[0-9][0-9][0-9][ ]*-[ ]*[0-9][0-9][0-9][0-9][0-9][0-9]");
void Regex()
{
	string someSinhanNum("212 - 213 -123512");
	string noSinhanNum("212a -213 -123512");
	// try to match the string with the regular expression
	bool res1 = std::tr1::regex_match(someSinhanNum, SinhanPattern);
	bool res2 = std::tr1::regex_match(noSinhanNum, SinhanPattern);
	return;
}
