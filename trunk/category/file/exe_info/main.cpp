#include <stdio.h>
#include <locale.h>
#include "exe_info.h"

int main()
{
	setlocale ( LC_ALL, "" );

	wchar_t szDir[256], szExe[256];
	GetExeDirectory(szDir);
	GetExeFilename(szExe);
	wprintf(L"GetExeDirectory() : %s\n", szDir);
	wprintf(L"GetExeFilename() : %s\n", szExe);

	return 0;
}