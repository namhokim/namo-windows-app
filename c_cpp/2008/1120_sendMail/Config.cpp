#include "Config.h"
const int StringBufferSize=1024;

namespace namo {
// public member
Config::Config(_STRING INIFilename)
: currentDirectory(_T("")), iniFileName(_T(""))
{
	iniFileName = INIFilename;
	Config::setCurrentDirectory();
}

Config::Config(void)
: currentDirectory(_T("")), iniFileName(_T("config.ini"))	// singleton string
{
	Config::setCurrentDirectory();
}

Config::~Config(void)
{
}

_STRING Config::getINIFileName() {
	return iniFileName;
}

_STRING Config::getCurrentDirectory() {
	return currentDirectory;
}

_STRING Config::getConfigFileName() {
	return (currentDirectory + _T("\\") + iniFileName);
}

BOOL Config::setINIFileName(_STRING INIFilename)
{	
	iniFileName = INIFilename;
	return isValidFile(INIFilename);
}

BOOL Config::isValidFile(_STRING fileName)
{
	return (GetFileAttributes(fileName.c_str()) != INVALID_FILE_ATTRIBUTES);	// if no file
}

_STRING Config::getSMTPHostName()
{
	return getStringValue(_T("SMTP"),_T("host"),_T("localhost"));
}

_STRING Config::getSMTPID()
{
	return getStringValue(_T("SMTP"),_T("id"),_T(""));
}

_STRING Config::getSMTPPassword()
{
	return getStringValue(_T("SMTP"),_T("password"),_T(""));
}

_STRING Config::getSMTPTimeout()
{
	return getStringValue(_T("SMTP"),_T("timeout"),_T("10000"));
}

BOOL Config::getSMTPNeedAuthorization()
{
	return (getStringValue(_T("SMTP"),_T("authorization"),_T("no")) == _T("yes"));
}

_STRING Config::getSMTPSender()
{
	return getStringValue(_T("SMTP"),_T("sender"),_T(""));
}

_STRING Config::getSMTPRecipient()
{
	return getStringValue(_T("SMTP"),_T("recipient"),_T(""));
}

BOOL Config::setSMTPHostName(_STRING value)
{
	return setStringValue(_T("SMTP"),_T("host"),value);
}

BOOL Config::setSMTPID(_STRING value)
{
	return setStringValue(_T("SMTP"),_T("id"),value);
}

BOOL Config::setSMTPPassword(_STRING value)
{
	return setStringValue(_T("SMTP"),_T("password"),value);
}

BOOL Config::setSMTPTimeout(_STRING value)
{
	return setStringValue(_T("SMTP"),_T("timeout"),value);
}

BOOL Config::setSMTPNeedAuthorization(BOOL isAuth)
{
	if(isAuth) return setStringValue(_T("SMTP"),_T("authorization"),_T("yes"));
	else return setStringValue(_T("SMTP"),_T("authorization"),_T("no"));
}

BOOL Config::setSMTPSender(_STRING displayName, _STRING emailAddress)
{
	return setStringValue(_T("SMTP"),_T("sender"),Config::withMailFormat(displayName,emailAddress));
}

BOOL Config::setSMTPRecipient(_STRING displayName, _STRING emailAddress)
{
	return setStringValue(_T("SMTP"),_T("recipient"),Config::withMailFormat(displayName,emailAddress));
}

BOOL Config::setSMTPRecipientAdd(_STRING displayName, _STRING emailAddress)
{
	_STRING beforeRecipient(Config::getSMTPRecipient());
	_STRING RecipientFormat(beforeRecipient + Config::withMailFormat(displayName,emailAddress));
	return setStringValue(_T("SMTP"),_T("recipient"),RecipientFormat);
}

_STRING Config::getMDBPathAndFileName()
{
	return getStringValue(_T("DB"), _T("mdbfile"), _T(""));
}
UINT Config::getStatusPollingInterval()
{
	return getIntegerValue(_T("POLLINGINTERVAL"), _T("statusPollingInterval"), -1);
}
// end of public member

// private member
void Config::setCurrentDirectory()
{
	TCHAR currentPath[MAX_PATH];
	_TGETCWD(currentPath,MAX_PATH);
	currentDirectory = currentPath;
}

_STRING Config::getStringValue(_STRING section, _STRING key, _STRING defaultValue)
{
	TCHAR tempString[StringBufferSize];
	GetPrivateProfileString(section.c_str(),key.c_str(),defaultValue.c_str(),
		tempString,StringBufferSize,Config::getConfigFileName().c_str());
	return tempString;
}

BOOL Config::setStringValue(_STRING section, _STRING key, _STRING value)
{
	return WritePrivateProfileString(section.c_str(),key.c_str(),value.c_str(),
		Config::getConfigFileName().c_str());
}

_STRING Config::withMailFormat(_STRING displayName, _STRING emailAddress)
{
	return _T("\"") + displayName + _T("\"<") + emailAddress + _T(">;");
}
UINT Config::getIntegerValue(_STRING section, _STRING key, UINT defaultValue)
{
	return GetPrivateProfileInt(section.c_str(), key.c_str(), defaultValue, Config::getConfigFileName().c_str());
}

BOOL Config::setIntegerValue(_STRING section, _STRING key, UINT value)
{
	TCHAR tempString[StringBufferSize];
	wsprintf(tempString, _T("%d"), value);
	_STRING setStringValue(tempString);
	return Config::setStringValue(section, key, setStringValue);
}
// end of private member

} // namespace namo