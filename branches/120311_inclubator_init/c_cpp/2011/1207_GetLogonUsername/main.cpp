#include "GetLogonUsername.h"
#include <Windows.h>
#include <tchar.h>	// for _tprintf
#include <locale.h>	// for LC_ALL, setlocale

int main()
{
	TCHAR szName[UNLEN] = {0};
	DWORD dw = UNLEN;
	BOOL bRes;

	bRes = GetLogonUsername(szName, &dw);

	setlocale ( LC_ALL, "" );
	_tprintf(_T("Res: %d, %s, %u\n"), bRes, szName, dw);
	return 0;
}