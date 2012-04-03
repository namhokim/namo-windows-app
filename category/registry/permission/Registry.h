#ifndef _REGISTRY_PERMISION__
#define _REGISTRY_PERMISION__

#include <Windows.h>

void OpenTest(HKEY hKey, LPCTSTR lsSubkey);
void CreateTest(HKEY hKey, LPCTSTR lsSubkey);
void DeleteTest(HKEY hKey, LPCTSTR lsSubkey);

#endif