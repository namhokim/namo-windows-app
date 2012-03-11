#pragma once
#include <windows.h>
#include "unicode.h"

namespace namo {
class Config
{
public:
	Config(void);
	Config(_STRING INIFilename);
	~Config(void);
	_STRING getINIFileName();
	_STRING getCurrentDirectory();
	_STRING getConfigFileName();
	BOOL setINIFileName(_STRING INIFilename);
	BOOL isValidFile(_STRING fileName);
	_STRING getSMTPHostName();
	_STRING getSMTPID();
	_STRING getSMTPPassword();
	_STRING getSMTPTimeout();
	_STRING getSMTPSender();
	_STRING getSMTPRecipient();
	BOOL getSMTPNeedAuthorization();
	BOOL setSMTPHostName(_STRING value);
	BOOL setSMTPID(_STRING value);
	BOOL setSMTPPassword(_STRING value);
	BOOL setSMTPTimeout(_STRING value);
	BOOL setSMTPNeedAuthorization(BOOL isAuth);
	BOOL setSMTPSender(_STRING displayName, _STRING emailAddress);
	BOOL setSMTPRecipient(_STRING displayName, _STRING emailAddress);
	BOOL setSMTPRecipientAdd(_STRING displayName, _STRING emailAddress);
	_STRING getMDBPathAndFileName();
	UINT getStatusPollingInterval();
private:
	_STRING currentDirectory;
	_STRING iniFileName;
	void setCurrentDirectory();	// 현재 디렉토리 경로 구하기
	_STRING	getStringValue(_STRING section, _STRING key, _STRING defaultValue);
	BOOL	setStringValue(_STRING section, _STRING key, _STRING value);
	_STRING withMailFormat(_STRING displayName, _STRING emailAddress);
	UINT getIntegerValue(_STRING section, _STRING key, UINT defaultValue);	// value: 문자가 포함되면 defaultValue 반환, 숫자+문자가 포함되면 문자전 숫자만 반환된다 // defaultValue가 음수일 경우는 0이 반환된다 // section&key를 찾을 수 없으면 0이 반환
	BOOL setIntegerValue(_STRING section, _STRING key, UINT value);
}; // class Config
} // namespace namo