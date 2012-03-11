#pragma once
#include "HttpRequest.h"

#define	VALID_PASSED		51
#define	VALID_NO_ID			52
#define	VALID_INCORRECT_PWD	53
#define	VALID_PARSING_ERR	54

class UserInfoValidator
{
public:
	UserInfoValidator(void);
	~UserInfoValidator(void);
	int isValid(_STRING id, _STRING password);
	_STRING resolveMsg(int code);
	void setDebugMode(bool isDebug)	{	debugMode	= isDebug;	}
private:
	int resolveText(std::string text);
	bool debugMode;
};
