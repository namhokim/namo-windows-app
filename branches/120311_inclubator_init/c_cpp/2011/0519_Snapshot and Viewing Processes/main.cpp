#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>

//  Forward declarations:
BOOL GetProcessList( );
BOOL IsStillAutoupdate(LPCTSTR szExeFile);

int main( void )
{
	//GetProcessList( );
	printf("%d\n", IsStillAutoupdate(_T("Snapshot and Viewing Processes2.exe")));
	return 0;
}

BOOL GetProcessList( )
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		OutputDebugString( TEXT("CreateToolhelp32Snapshot (of processes)") );
		return( FALSE );
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		OutputDebugString( TEXT("Process32First") );// show cause of failure
		CloseHandle( hProcessSnap );				// clean the snapshot object
		return( FALSE );
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		_tprintf( TEXT("%s\n"), pe32.szExeFile );

	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );
	return( TRUE );
}

BOOL IsStillAutoupdate(LPCTSTR szExeFile)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	BOOL bWasFound = FALSE;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE ) {
		OutputDebugString( TEXT("CreateToolhelp32Snapshot (of processes)") );
		return( FALSE );
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) ) {
		OutputDebugString( TEXT("Process32First") );// show cause of failure
		CloseHandle( hProcessSnap );				// clean the snapshot object
		return( FALSE );
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do {
		if( lstrcmp(szExeFile, pe32.szExeFile) == 0 ) {
			bWasFound = TRUE;
			break;
		}
	} while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );
	return( bWasFound );
}