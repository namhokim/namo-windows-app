#include <iostream>
#include <Windows.h>
#include "../DebugCommon/OutputDebugPrintf.h"

//#pragma comment(lib, "DebugCommon.lib")

using namespace std;

bool NHDeletePrinter(LPWSTR pPrinterName);

#define	NO_UI
#ifdef NO_UI
int APIENTRY wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPWSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(nCmdShow);

	if(lstrlenW(lpCmdLine)<1) {
		OutputDebugPrintf(TEXT("Usage : DeletePrinter [PrinterName]"));
	} else {
		NHDeletePrinter(lpCmdLine);	// pass with second argument
	}
}

bool NHDeletePrinter(LPWSTR pPrinterName)
{
	bool r = false;
	HANDLE hPrinter;
	PRINTER_DEFAULTS pDefault;

	::ZeroMemory(&pDefault, sizeof(PRINTER_DEFAULTS));
	pDefault.DesiredAccess = DELETE;

	if(0!=OpenPrinter(pPrinterName, &hPrinter, &pDefault)) {
		// ERROR_ACCESS_DENIED
		if(0!=DeletePrinter(hPrinter)) r = true;
		ClosePrinter(hPrinter);
	}

	TCHAR msg[1024] = {0};
	DWORD dw = ::GetLastError();
	OutputDebugPrintf(TEXT("Delete Printer : %s [%d]"), ErrorCodeToString(msg, dw), dw);

	return r;
}

#else
int wmain(int argc, wchar_t* argv[]) {

	if(argc<2) {
		cout << " Usage : DeletePrinter [PrinterName]" << endl;
	} else {
		NHDeletePrinter(argv[1]);	// pass with second argument
	}
}

bool NHDeletePrinter(LPWSTR pPrinterName)
{
	bool r = false;
	HANDLE hPrinter;
	PRINTER_DEFAULTS pDefault;

	::ZeroMemory(&pDefault, sizeof(PRINTER_DEFAULTS));
	pDefault.DesiredAccess = DELETE;

	if(0!=OpenPrinter(pPrinterName, &hPrinter, &pDefault)) {
		// ERROR_ACCESS_DENIED
		if(0!=DeletePrinter(hPrinter)) r = true;
		ClosePrinter(hPrinter);
	}

	char msg[1024] = {0};
	cout << "  Result : " << ErrorCodeToStringA(msg, ::GetLastError()) << endl;

	return r;
}
#endif