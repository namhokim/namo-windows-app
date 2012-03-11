#include "SendMail.h"

namespace namo {
SendMail::SendMail(void)
:IsSetConfigFile(FALSE)
{
	CoInitialize(S_OK);
	IsSetConfigFile = config.isValidFile(config.getINIFileName());
}

SendMail::SendMail(_STRING configFileName)
:config(configFileName),IsSetConfigFile(TRUE)
{
	CoInitialize(S_OK);
	IsSetConfigFile = config.setINIFileName(configFileName);
}

SendMail::~SendMail(void)
{
	CoUninitialize();
}

_STRING SendMail::getHostname()
{
	return config.getSMTPHostName();
}

BOOL SendMail::setINIFileName(_STRING filename)
{
	return config.setINIFileName(filename);
}

bool SendMail::sendMail(_STRING subject, _STRING content, _STRING attachFilePath)
{
	if(IsSetConfigFile == FALSE) return false;

	CMimeMessage msg;

	msg.SetSubject(subject.c_str());
	msg.AddText(content.c_str());
	if(attachFilePath.length() > 0) msg.AttachFile(attachFilePath.c_str());

	SendMail::addSender(msg);
	SendMail::addRecipients(msg);

	if(config.getSMTPNeedAuthorization()) {
		return connectAndSend(msg, config.getSMTPHostName().c_str(), 
			CT2CA(config.getSMTPID().c_str()), CT2CA(config.getSMTPPassword().c_str()));
	}
	else return connectAndSend(msg, config.getSMTPHostName().c_str());
}

bool SendMail::connectAndSend(CMimeMessage& _message, _STRING host, PCCH id, PCCH password)
{
	if(host.size() == 0) return false;

	CSMTPConnection connection;
	BOOL isConnect = FALSE;
	isConnect = connection.Connect(host.c_str(), id, password);

	BOOL isSuccess = FALSE;
	if(isConnect) isSuccess = connection.SendMessage(_message);
	setLastMessage(connection.getLastMessage());
	if(isConnect) connection.Disconnect();

	return (isSuccess && isConnect);
}

std::string SendMail::getLastMessage()
{
	return SendMail::lastMessage;
}

void SendMail::setLastMessage(std::string msg)
{
	SendMail::lastMessage = msg;
}

void SendMail::addSender(CMimeMessage& msg)
{
	_STRING senders(config.getSMTPSender());
	std::vector<_STRING> displayNameAndEmail;

	Tokenize(senders, displayNameAndEmail, _T("\"<>;"));
	switch(displayNameAndEmail.size()) {
		case 2:
			msg.SetSender(displayNameAndEmail[1].c_str());
			break;
		case 3:	// 표시할 이름과 이메일 주소사이에 white space가 들어있을 경우가 있다
			msg.SetSender(displayNameAndEmail[2].c_str());
			break;
		default:
			msg.SetSender(displayNameAndEmail[0].c_str());
	}
	msg.SetSenderName(displayNameAndEmail[0].c_str());
}

void SendMail::addRecipients(CMimeMessage& msg)
{
	_STRING recipients(config.getSMTPRecipient());
	std::vector<_STRING> account;

	Tokenize(recipients, account, _T(";"));
	size_t accountCount = account.size();
	for(size_t i=0; i<accountCount; i++) {
		std::vector<_STRING> displayNameAndEmail;

		Tokenize(account[i], displayNameAndEmail, _T("\"<>"));
		switch(displayNameAndEmail.size()) {
			case 2:
				msg.AddRecipient(displayNameAndEmail[1].c_str(),displayNameAndEmail[0].c_str());
				break;
			case 3:	// 표시할 이름과 이메일 주소사이에 white space가 들어있을 경우가 있다
				msg.AddRecipient(displayNameAndEmail[2].c_str(),displayNameAndEmail[0].c_str());
				break;
			default:
				msg.AddRecipient(displayNameAndEmail[0].c_str(),displayNameAndEmail[0].c_str());
		}
	}
}

_STRING SendMail::getTitleNoticeDeviceCounter()
{
	return _T("Counter Information Notification");
}

_STRING SendMail::getContentNoticeDeviceCounter()
{
	return _T("/////WISDOM lite/////\n\n")
		_T("[Counter Information Notification]\n")
		_T("----------------------------------------------------------------\n\n")
		_T("Counter information export results.\n")
		_T("Registered Group: -\n\n")
		_T("Please check the attached file for the results.\n\n")
		_T("----------------------------------------------------------------\n");
}

_STRING SendMail::getTitleSMTPTestNotification()
{
	return _T("SMTP Test Notification");
}

_STRING SendMail::getContentSMTPTestNotification()
{
	return _T("/////WISDOM lite/////\n\n")
		_T("[SMTP Test Notification]\n")
		_T("----------------------------------------------------------------\n\n")
		_T("SMTP Test succeeded. This email confirms that the performed SMTP test was successful.\n\n")
		_T("----------------------------------------------------------------\n");
}

void SendMail::Tokenize(const _STRING& str, std::vector<_STRING>& tokens, const _STRING& delimiters)
	{
		// 맨 첫 글자가 구분자인 경우 무시
		_STRING::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// 구분자가 아닌 첫 글자를 찾는다
		_STRING::size_type pos     = str.find_first_of(delimiters, lastPos);

		while (_STRING::npos != pos || _STRING::npos != lastPos)
		{
			// token을 찾았으니 vector에 추가한다
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// 구분자를 뛰어넘는다.  "not_of"에 주의하라
			lastPos = str.find_first_not_of(delimiters, pos);
			// 다음 구분자가 아닌 글자를 찾는다
			pos = str.find_first_of(delimiters, lastPos);
		}
	}
} // namespace namo