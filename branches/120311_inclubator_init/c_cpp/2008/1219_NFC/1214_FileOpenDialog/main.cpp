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
	//fodb.addFilter(_T("�ؽ�Ʈ(*.txt)"), _T("*.txt"));

	std::wcout.imbue(std::locale("korean"));	// ���&���� �̸��� �ѱ��� ���ԵǾ� ������ �̰��� ����!
	setlocale(LC_ALL, "Korean");				// wstring -> string ���� ��ȯ�� ����Ѵٸ�(�ѱ�)
	_COUT << fodb.showAndGetFilename();
	return 0;
}