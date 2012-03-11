#pragma once

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

//
// Purpose: 
//   Get directory which located execute file
//
// Parameters:
//   Output : directory name
// 
// Return value:
//   ERROR_SUCCESS(0), ERROR_INSUFFICIENT_BUFFER(122)
//
DWORD GetExeDirectory(LPTSTR szDir);


//
// Purpose: 
//   Get filename which located execute file
//
// Parameters:
//   Output : filename
// 
// Return value:
//   ERROR_SUCCESS(0), ERROR_INSUFFICIENT_BUFFER(122)
//
DWORD GetExeFilename(LPTSTR szFilename);
