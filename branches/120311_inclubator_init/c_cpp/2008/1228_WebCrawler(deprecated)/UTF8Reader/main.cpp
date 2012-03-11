#include "HttpRequest.h"
#include "FileManager.h"
#include <iostream>
using namespace std;



void testFileManager()
{
	FileManager fm;
	if(fm.open(L"comu_notice_list.html")) {
		wcout << fm.readByString();
		fm.close();
	}
	else cout << "fail\n";
}

void testReadUTFRead()
{
	FileManager fm;
	if(fm.open(L"index.htm")) {
		_STRING str = fm.readByString(CP_UTF8);
		wcout.imbue(locale("kor")); 

		wcout << str;
		fm.close();
	}
	else cout << "fail\n";
}

void testHttpReqSaveToFile()
{
	HttpRequest hr;
	hr.open(L"redo.egloos.com");
	hr.requestSaveToFile(TEXT("318502"), TEXT("318502.html"));
	hr.close();
}

int main()
{
	//testReadUTFRead();
	HttpRequest hr;
	hr.open(L"www.google.co.kr");
	hr.requestSaveToFile(L"/images/nav_logo4.png",L"nav_logo4.png");
	hr.close();
	
}