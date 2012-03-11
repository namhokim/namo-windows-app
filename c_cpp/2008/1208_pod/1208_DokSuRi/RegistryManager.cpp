#include "RegistryManager.h"

RegistryManager::RegistryManager(void)
:m_key(NULL)
{
}

RegistryManager::~RegistryManager(void)
{
	close();
}

void RegistryManager::open(_STRING lpSubKey, REGSAM samDesired, HKEY hKey)
{
	RegCreateKeyEx(hKey, lpSubKey.c_str(),0,NULL,
			REG_OPTION_NON_VOLATILE, samDesired,NULL,&m_key,&m_dwDisp);
}

_STRING RegistryManager::readByString(_STRING valueName, _STRING defaultValue, DWORD lpType)
{
	const int bufferSize = 1024;
	_TCHAR bufferString[bufferSize];
	DWORD bufferStringSize = bufferSize;
	
	if (RegQueryValueEx(m_key, valueName.c_str(), 0, &lpType,(LPBYTE)bufferString, &bufferStringSize) !=ERROR_SUCCESS) {
		return defaultValue;
	}
	else return bufferString;
}

bool RegistryManager::writeByString(_STRING valueName, _STRING data)
{
	LONG retCode = RegSetValueEx(m_key, valueName.c_str(), 0, REG_SZ, (LPBYTE)data.c_str(), data.size()*sizeof(_TCHAR));
	if(retCode == ERROR_SUCCESS) return true;
	else {

		return false;
	}
}
