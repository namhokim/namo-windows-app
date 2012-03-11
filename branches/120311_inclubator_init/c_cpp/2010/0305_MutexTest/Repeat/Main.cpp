#include <Windows.h>
#include <iostream>

using namespace std;

LPCWSTR MY_GUID = L"5DB7851E-C151-410f-B488-D18072EB23AC";

int main()
{
	unsigned int i = 0;

	HANDLE hMutex = ::CreateMutex(NULL, NULL, MY_GUID);
	if(NULL==hMutex) exit(1);

	while(1) {
		DWORD code = ::WaitForSingleObject(hMutex, 0);
		if(WAIT_TIMEOUT == code) break;
		::ReleaseMutex(hMutex);

		cout << i << ">반복작업" << endl;
		++i;
		Sleep(3000);
	}

	::CloseHandle(hMutex);

	return 0;
}