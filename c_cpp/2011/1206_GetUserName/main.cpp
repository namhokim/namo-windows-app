#include <windows.h>// for GetUserName
#include <tchar.h>	// for _tprintf
#include <Lmcons.h>	// for UNLEN
#include <locale.h>	// for LC_ALL, setlocale

const int LPN_SIZE = (UNLEN +1);

int main()
{
	TCHAR szName[LPN_SIZE] = {0};
	DWORD dw = LPN_SIZE;
	BOOL bRes;

	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms724432(v=vs.85).aspx
	bRes = GetUserName(szName, &dw);
	setlocale ( LC_ALL, "" );
	_tprintf(_T("Res: %d, %s, %u\n"), bRes, szName, dw);

	return 0;
}