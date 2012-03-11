#include "UserInfoValidator.h"

UserInfoValidator::UserInfoValidator(void)
: debugMode(false)
{
}

UserInfoValidator::~UserInfoValidator(void)
{
}

int UserInfoValidator::isValid(_STRING id, _STRING password)
{
	_STRING reqObject(_T("/POD_Center/login.do?userId="));
	reqObject.append(id);
	reqObject.append(_T("&password="));
	reqObject.append(password);

	std::string resText;
	HttpRequest req(_T("Validator"));
	if(!req.open(_T("10.15.1.102"), 9090)) return CANNOT_CONECT;
	resText = req.requestText(reqObject, _T("POST"));
	req.close();
	return resolveText(resText);
}

int UserInfoValidator::resolveText(std::string text)
{
	if(debugMode) std::cout << text << std::endl;

	if(text.find("<meta http-equiv=\"Robot\" content=\"Frame\">") != std::string::npos) return VALID_PASSED;
	if(text.find("<span id=\"null.errors\">Invalid USER ID</span>") != std::string::npos) return VALID_NO_ID;
	if(text.find("<span id=\"null.errors\">Invalid Password</span>") != std::string::npos) return VALID_INCORRECT_PWD;

	if(text.find("not conected") != std::string::npos) return CANNOT_CONECT;
	if(text.find("request error") != std::string::npos) return CANNOT_REQUEST;
	if(text.find("InternetQueryDataAvailable Error") != std::string::npos) return ERROR_QUERYDATA;
	if(text.find("InternetReadFile Error") != std::string::npos) return ERROR_READFILE;
	return VALID_PARSING_ERR;
}

_STRING UserInfoValidator::resolveMsg(int code)
{
	switch(code) {
	case VALID_PASSED:
		return _T("OK");
	case VALID_NO_ID:
		return _T("No ID");
	case VALID_INCORRECT_PWD:
		return _T("Password incorrect");
	case VALID_PARSING_ERR:
		return _T("Program Error");
	case CANNOT_CONECT:
	default:
		return _T("Connect Error");
	case CANNOT_REQUEST:
		return _T("Request Error");
	case ERROR_QUERYDATA:
		return _T("InternetQueryDataAvailable Error");
	case ERROR_READFILE:
		return _T("InternetReadFile Error");

	}
}