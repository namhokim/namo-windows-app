#include <Windows.h>
#include <iostream>

using namespace std;

LPCWSTR MY_GUID = L"5DB7851E-C151-410f-B488-D18072EB23AC";

int main()
{
	unsigned int i = 0;

	HANDLE hMutex = ::CreateMutex(NULL, NULL, MY_GUID);
	if(NULL==hMutex) exit(1);
	if(::GetLastError()==ERROR_ALREADY_EXISTS) {
		cout << "실행중입니다" << endl;
	}

	DWORD code = ::WaitForSingleObject(hMutex, INFINITE);
	cout << code << "시그널" << endl;
	Sleep(5000);

	::CloseHandle(hMutex);

	return 0;
}