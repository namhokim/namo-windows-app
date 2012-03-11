#include <Windows.h>
#include <stdio.h>
#include <vector>
using std::vector;

int main()
{
//	wchar_t bom = 0xFEFF;	// LITTLE ENDIAN (실제 저장은 FE FF로 된다)

	char *acp = "한글과 English";
	// ACP->UTF-16
	int needSize = MultiByteToWideChar(CP_ACP, 0, acp, strlen(acp), NULL, 0);
	vector<WCHAR> buf1(needSize);
	MultiByteToWideChar(CP_ACP, 0, acp, strlen(acp), (LPWSTR)&buf1[0], needSize);

	// 필요한 버퍼의 크기를 구한다
	int needBuf = WideCharToMultiByte(CP_UTF8, 0, &buf1[0], needSize, NULL, 0, NULL, NULL);
	vector<BYTE> buf(needBuf);
	// 변환한다
	int res = WideCharToMultiByte(CP_UTF8, 0, &buf1[0], needSize, (LPSTR)&buf[0], needBuf, NULL, NULL);

	HANDLE hFile = CreateFile(TEXT("outfile.txt"), GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// 에러 처리를 하지 않았다.
	DWORD dwWritten;
//	WriteFile(hFile, &bom, 2, &dwWritten, NULL);
	WriteFile(hFile, &buf[0], needBuf, &dwWritten, NULL);
	CloseHandle(hFile);
	printf("Writtten : %u\n", dwWritten);
	
	return 0;
}