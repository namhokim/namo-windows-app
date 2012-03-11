//#include <windows.h>
#include <iostream>
#include "unicode.h"
#include "BuildCSV.h"
#include "SendMail.h"
#include "Config.h"

bool noticeDeviceCounter()
{
	bool isSuccess;

	namo::BuildCSV counterCSV;
	isSuccess = counterCSV.make();

	if(isSuccess) {
		namo::SendMail mail;
		isSuccess = mail.sendMail(mail.getTitleNoticeDeviceCounter(),
			mail.getContentNoticeDeviceCounter(), counterCSV.getFileName());
		counterCSV.remove();
	}
	return isSuccess;
}

bool sendSMTPTestNotification()
{
	namo::SendMail mail;

	return mail.sendMail(mail.getTitleSMTPTestNotification(), mail.getContentSMTPTestNotification());
}

bool sendNotification(_STRING subject, _STRING content, _STRING attachFilePath=_T(""))
{
	namo::SendMail mail;

	return mail.sendMail(subject,content,attachFilePath);
}

void Usage()
{
	_COUT << "usage.\n";
	_COUT << "sendMail [Title] [Content] ([AttachFile])\n";
}

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	wcout.imbue(locale("korean"));

	_STRING title(_T("no title"));
	_STRING content(_T("no content"));
	_STRING attachFilePath(_T(""));
	if(argc == 3) {
		title = argv[1];
		content = argv[2];
	}
	else if(argc == 4) {
		title = argv[1];
		content = argv[2];
		attachFilePath = argv[3];
	}
	else {
		Usage();
		return 0;
	}
	if(sendNotification(title,content,attachFilePath))
		MessageBoxA(NULL, "성공", "알림", MB_OK);	//std::cout << mail.getLastMessage();
	else 
		MessageBoxA(NULL, "실패", "알림", MB_OK);	//std::cout << mail.getLastMessage();
	return 0;
}
