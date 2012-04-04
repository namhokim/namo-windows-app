#include <stdio.h>
#include <Windows.h>

BOOL IsWindowsVistaOrLater();
BOOL IsWindowsXPOrLater();

int main()
{
	printf("Is Windows Vista or later? => %s\n",
		(IsWindowsVistaOrLater() ? "Yes" : "No") );

	printf("Is Windows XP or later? => %s\n",
		(IsWindowsXPOrLater() ? "Yes" : "No") );

	return 0;
}

BOOL IsWindowsVistaOrLater()
{
	OSVERSIONINFO osvi;
	BOOL bIsWindowsVistaOrLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	// 이 함수는 구조체의 dwOSVersionInfoSize 필드의 값을 잘못 입력했을 때만 실패한다.
	// GetVersionEx function
	// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/ms724451(v=vs.85).aspx
	GetVersionEx(&osvi);

	// OSVERSIONINFOEX structure :: Remarks
	// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/ms724833(v=vs.85).aspx
	bIsWindowsVistaOrLater =
		( (osvi.dwMajorVersion > 6) ||
		( (osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion >= 0) ));

	return bIsWindowsVistaOrLater;
}

BOOL IsWindowsXPOrLater()
{
	OSVERSIONINFO osvi;
	BOOL bIsWindowsXPOrLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	// 이 함수는 구조체의 dwOSVersionInfoSize 필드의 값을 잘못 입력했을 때만 실패한다.
	// GetVersionEx function
	// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/ms724451(v=vs.85).aspx
	GetVersionEx(&osvi);

	// OSVERSIONINFOEX structure :: Remarks
	// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/ms724833(v=vs.85).aspx
	bIsWindowsXPOrLater = 
       ( (osvi.dwMajorVersion > 5) ||
       ( (osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1) ));

	return bIsWindowsXPOrLater;
}