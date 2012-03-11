#include <windows.h>
#include <string>
#include <iostream>
#include <tchar.h>
#include "FileOpenDialogBox.h"
#include <list>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//HWND hWnd;
	//hWnd = FindWindow(NULL, _T("1214_FileOpenDialog"));
	//if(hWnd) SetForegroundWindow(hWnd);
	//else {
	//	cout << "Can't find parents windows\n";
	//	return 0;
	//}
	FileOpenDialogBox fodb;
//	fodb.setWindowHandle(hWnd);
	//fodb.addFilter(_T("텍스트(*.txt)"), _T("*.txt"));

	std::wcout.imbue(std::locale("korean"));	// 경로&파일 이름에 한글이 포함되어 있으면 이것을 포함!
	setlocale(LC_ALL, "Korean");				// wstring -> string 등의 변환을 사용한다면(한글)
	_COUT << fodb.showAndGetFilename();
	return 0;
}