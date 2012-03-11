#include "HttpClient.h"
#include <iostream>

using namespace std;

int main()
{
	// JSESSIONID=BEE42772E28AB5ED4076F5909D05D6A3.tomcat5A
	namo::HttpClient f("My works login - session");
	cout << "first : " << f.requestGET("http://eipap.sindoh.com/MyWorks2/") << endl;
	cout << f.getStatusCode() << endl;
	Sleep(5000);

	//namo::HttpClient l("My works login - alpha");
	// JSESSIONID=5BD1428264B782529FE349B72F5A80D5.tomcat5B
	f.addHeader("Accept-Language", " ko");
	f.addHeader("Content-Type", " application/x-www-form-urlencoded");
	f.addHeader("Accept-Encoding", " gzip, deflate");
	f.addHeader("Connection", " Keep-Alive");
	f.addContent("forwardURL", "");
	f.addContent("currentLangCode", "ko");
	f.addContent("userId", "namhokim");
	f.addContent("password", "inroot");
	f.addContent("loginSubmit", "%EC%A0%9C%EC%B6%9C");
	cout << "login : " << f.requestPOST("http://eipap.sindoh.com/MyWorks2/UserLoginController.do") << endl;
	cout << f.getStatusCode() << endl;
	Sleep(5000);

	// JSESSIONID=5BD1428264B782529FE349B72F5A80D5.tomcat5B
//	namo::HttpClient c("SMS agent - alpha");
	f.clearHeader();
	f.addHeader("Referer" , " http://eipap.sindoh.com/MyWorks2/jsp/SmsController.do?command=SimpleSmsComposeForm&messaging=Y&personId=0&external=Y");
	f.addHeader("Accept-Language", " ko");
	f.addHeader("Content-Type", " application/x-www-form-urlencoded");
	f.addHeader("Accept-Encoding", " gzip, deflate");
	f.addHeader("Connection", " Keep-Alive");
	f.clearContent();
	f.addContent("command", "SimpleSmsComposePost");
	f.addContent("senderPersonId", "145988");
	f.addContent("receiverPersonId", "0");
	f.addContent("senderPersonName", "%EA%B9%80%EB%82%A8%ED%98%B8");
	f.addContent("callbackPhoneNo", "010-3191-8593");
	f.addContent("toType", "1");
	f.addContent("receiverPersonName", "%EB%A1%9C%EB%B2%84%ED%8A%B8");
	f.addContent("receiverPhoneNo", "010-3191-8593");
	f.addContent("message", "%EB%A1%9C%EB%%EC%84%9C%EB%B2%84+%EC%A0%90%EA%B2%80+%EB%B6%80%ED%83%81%EB%93%9C%EB%A6%BD%EB%8B%88%EB%8B%A4.");
	f.addContent("submit", "%EB%B3%B4%EB%82%B4%EA%B8%B0");
	cout << f.requestPOST("http://eipap.sindoh.com/MyWorks2/jsp/SmsController.do") << endl;
	cout << f.getStatusCode() << endl;
	Sleep(5000);

	return 0;
}
//JSESSIONID=1CC06951C35C532F18188AAE55ACB383.tomcat5B