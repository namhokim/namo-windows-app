// 201107.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

/*
 range:
	0<=x<=52
 rules:
	(52-x)%9=2
	(52-x)%4=3
	(52-x)%7=5
 */
bool isMatchRules(int x)
{
	return ( (52-x)%9==2 && (52-x)%4==3 && (52-x)%7==5 );
}

int _tmain(int argc, _TCHAR* argv[])
{
	for(int i=0; i<=52; ++i)
	{
		if(isMatchRules(i)) {
			cout << "number of lost: " << i << endl;
			return 0;
		}
	}
	cout << "cannot found" << endl;

	return 1;
}

