#include <Windows.h>
#include <stdio.h>
#include <vector>
using std::vector;

int main()
{
//	wchar_t bom = 0xFEFF;	// LITTLE ENDIAN (���� ������ FE FF�� �ȴ�)

	char *acp = "�ѱ۰� English";
	// ACP->UTF-16
	int needSize = MultiByteToWideChar(CP_ACP, 0, acp, strlen(acp), NULL, 0);
	vector<WCHAR> buf1(needSize);
	MultiByteToWideChar(CP_ACP, 0, acp, strlen(acp), (LPWSTR)&buf1[0], needSize);

	// �ʿ��� ������ ũ�⸦ ���Ѵ�
	int needBuf = WideCharToMultiByte(CP_UTF8, 0, &buf1[0], needSize, NULL, 0, NULL, NULL);
	vector<BYTE> buf(needBuf);
	// ��ȯ�Ѵ�
	int res = WideCharToMultiByte(CP_UTF8, 0, &buf1[0], needSize, (LPSTR)&buf[0], needBuf, NULL, NULL);

	HANDLE hFile = CreateFile(TEXT("outfile.txt"), GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	// ���� ó���� ���� �ʾҴ�.
	DWORD dwWritten;
//	WriteFile(hFile, &bom, 2, &dwWritten, NULL);
	WriteFile(hFile, &buf[0], needBuf, &dwWritten, NULL);
	CloseHandle(hFile);
	printf("Writtten : %u\n", dwWritten);
	
	return 0;
}