#include <Windows.h>
#include <stdlib.h>
#include <iostream>

int main(int argc, char *argv[])
{
	// 자동 리셋 타이머를 생성
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("TESTTIMER"));
	DWORD dwError = GetLastError();
	if(ERROR_ALREADY_EXISTS!=dwError) {
		std::cout << "Not Existence" << std::endl;
		CloseHandle(hEvent);
		return EXIT_FAILURE;
	}

	SetEvent(hEvent);
	std::cout << dwError << std::endl;
	CloseHandle(hEvent);

	return EXIT_SUCCESS;
}
