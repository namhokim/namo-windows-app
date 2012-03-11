#pragma once
#include <atlsmtpconnection.h>
#include "unicode.h"
#include "Config.h"
#include <algorithm>
#include <vector>

namespace namo {
class SendMail
{
public:
	SendMail(void);
	SendMail(_STRING configFileName);
	~SendMail(void);
	_STRING getHostname();
	BOOL setINIFileName(_STRING filename);
	bool sendMail(_STRING subject, _STRING content, _STRING attachFilePath=_T(""));
	bool connectAndSend(CMimeMessage& _message, _STRING host, PCCH id = NULL, PCCH password = NULL);
	std::string getLastMessage();
	_STRING getTitleNoticeDeviceCounter();
	_STRING getContentNoticeDeviceCounter();
	_STRING getTitleSMTPTestNotification();
	_STRING getContentSMTPTestNotification();
private:
	BOOL IsSetConfigFile;
	Config config;
	std::string lastMessage;
	void setLastMessage(std::string msg);
	void Tokenize(const _STRING& str, std::vector<_STRING>& tokens, const _STRING& delimiters = _T(" "));
	void addSender(CMimeMessage& msg);
	void addRecipients(CMimeMessage& msg);
}; // class SendMail
} // namespace namo
