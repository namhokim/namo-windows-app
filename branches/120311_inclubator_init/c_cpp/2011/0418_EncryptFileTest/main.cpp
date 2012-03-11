#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	BOOL bRes;
	::system("echo test file > text.txt");
	
	bRes = ::EncryptFile(TEXT("text.txt"));
	printf("EncryptFile result : %d", bRes);
	return 0;
}