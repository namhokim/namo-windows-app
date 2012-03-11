// Ft2St.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

void Usage();
void PrtSysTime(const LPSYSTEMTIME st);

using namespace std;

// '/' �����ڷ� ����
// '/' ���� : dwHighDateTime
// '/' ���� : dwLowDateTime
FILETIME ToFileTime(wstring strFt)
{
	vector<wstring> r = Util::getTokenVec<wchar_t>(strFt, '/');

	FILETIME ft;
	if(r.size() == 2) {
		ft.dwHighDateTime = Util::str2num<wchar_t, DWORD>(r.at(0));
		ft.dwLowDateTime = Util::str2num<wchar_t, DWORD>(r.at(1));
	}
	else throw (int)0;

	return ft;
}

void PrtToSysTime(wstring wstrFt)
{
	try {
		FILETIME ft;
		ft = ToFileTime(wstrFt);
		//wcout << ft.dwHighDateTime << "/" << ft.dwLowDateTime << endl;

		SYSTEMTIME st;
		::FileTimeToSystemTime(&ft, &st);
		PrtSysTime(&st);
	}
	catch (int e) {
		if(e == 0) Usage();
		else cout << "Unknown exception...\n";
	}
	catch (...) {
		cout << "Unknown exception...\n";
	}
}

// SYSTEMTIME ����ü ���(�̰� ������ ���� ����)
void PrtSysTime(const LPSYSTEMTIME st)
{
	printf("%d-%02d-%02d %02d:%02d:%02d.%03d\n", st->wYear, st->wMonth, st->wDay,
		st->wHour, st->wMinute, st->wSecond, st->wMilliseconds);
}


////////////////////////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	if(argc != 2) Usage();
	else {
		PrtToSysTime(argv[1]);
	}
	return 0;
}

void Usage()
{
	cout << "Usage) Ft2St(.exe) [file time]\n";
	cout << "ex.\n> Ft2St 29995500/3606152000\n";
}

/*
 * Copyright (c) 2009 by Namho Kim.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */