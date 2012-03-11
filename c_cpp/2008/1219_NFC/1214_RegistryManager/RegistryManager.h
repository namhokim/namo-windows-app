#pragma once
#include "StringUtil.h"
#include <windows.h>

class RegistryManager
{
public:
	RegistryManager(void);
	~RegistryManager(void);
	void open(_STRING lpSubKey, REGSAM samDesired=KEY_READ, HKEY hKey=HKEY_CURRENT_USER);
	_STRING readByString(_STRING valueName, _STRING defaultValue, DWORD lpType=REG_SZ);
	bool writeByString(_STRING valueName, _STRING data);
	void close()				{	if(m_key != NULL) RegCloseKey(m_key);	}
private:
	HKEY m_key;
	DWORD m_dwDisp;
};
