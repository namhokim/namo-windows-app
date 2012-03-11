// test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//


#include "stdafx.h"
#include "Windows.h"
#include <crtdbg.h>
#include <list>
using std::list;

typedef list<int> IntList;
typedef IntList::iterator IntListIter;

IntList myList;

int main()
{
	_CrtMemDumpAllObjectsSince(0);
	STARTUPINFO
	return 0;
}

