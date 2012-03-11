// MekanderV.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "MekanderV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "WinInet.lib")

// ������ ���� ���α׷� ��ü�Դϴ�.

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC�� �ʱ�ȭ�մϴ�. �ʱ�ȭ���� ���� ��� ������ �μ��մϴ�.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���� �ڵ带 �ʿ信 ���� �����մϴ�.
		_tprintf(_T("�ɰ��� ����: MFC�� �ʱ�ȭ���� ���߽��ϴ�.\n"));
		nRetCode = 1;
	}
	else
	{
		if (!AfxSocketInit())
		{
			AfxMessageBox(_T("Windows ������ �ʱ�ȭ���� ���߽��ϴ�."));
			return FALSE;
		}

		// TODO: ���� ���α׷��� ������ ���⿡�� �ڵ��մϴ�.
		if(argc < 2) {
			Usage();
			MekanderV( _T("http://10.16.11.152:8080/m_total_usage/") );
		} else {
			MekanderV(argv[1]);
		}
	}

	return nRetCode;
}

VOID Usage()
{
	printf("MekanderV.exe [URL]\n");
}

//#include <StrSafe.h>
#ifdef UNICODE
#define	hostname hostnameA
#define	path	pathA
#else
#define	hostname hostnameW
#define	path	pathW
#endif
VOID MekanderV(LPCTSTR url)
{
	// URL���� IP�� ��Ʈ��ȣ�� �޾ƿ�
	TCHAR hostnameW[INTERNET_MAX_HOST_NAME_LENGTH + 1] = {0};
	TCHAR pathW[INTERNET_MAX_PATH_LENGTH + 1] = {0};
	URL_COMPONENTS uc;
	ZeroMemory(&uc, sizeof(uc));
	uc.dwStructSize = sizeof(uc);
	uc.lpszHostName = hostnameW;
	uc.lpszUrlPath = pathW;
	uc.dwHostNameLength = INTERNET_MAX_HOST_NAME_LENGTH;
	uc.dwUrlPathLength = INTERNET_MAX_PATH_LENGTH;
	InternetCrackUrl(url, lstrlen(url), ICU_DECODE, &uc);

	// host��
#ifdef UNICODE
	CHAR hostnameA[INTERNET_MAX_HOST_NAME_LENGTH + 1] = {0};
	CHAR pathA[INTERNET_MAX_PATH_LENGTH + 1] = {0};
	wsprintfA(hostnameA, "%S", hostnameW);
	wsprintfA(pathA, "%S", pathW);
#endif

	for(int i=0; i<100000; ) {
		//MekanderInet(i, url);
		if(MekanderSocket(i, hostname, uc.nPort, path)) {
			++i;
		}
		if(i==100000) {
			i = 0;
		}
		Sleep(2);
	}
}

LPCTSTR AGENT_NAME = _T("Mekander V");
LPCSTR	HTTP_GET_REQ = "GET %s HTTP/1.1\r\nAccept: image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-ms-application, application/x-ms-xbap, application/vnd.ms-xpsdocument, application/xaml+xml, */*\r\nAccept-Language: ko\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.04506.648; .NET CLR 3.5.21022; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; sindo_AA001F3C6960C0)\r\nAccept-Encoding: gzip, deflate\r\nHost: %s:%u\r\nConnection: Keep-Alive\r\n\r\n";

// �ڵ����� HINTERNET�� �ݰ� ���� Ŭ����
class InternetTrigger
{
private:
	HINTERNET& m_hInternet;
public:
	InternetTrigger(HANDLE& hInternet):m_hInternet(hInternet) {};
	~InternetTrigger() {
		CleanUp();
	}
	VOID CleanUp() {
		if(NULL!=m_hInternet) {
			::InternetCloseHandle(m_hInternet);
			m_hInternet = NULL;
		}
	}
}; /* class InternetTrigger */

VOID MekanderInet(int id, LPCTSTR url)
{
	//
	HINTERNET hInternet = InternetOpen(AGENT_NAME, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, INTERNET_FLAG_ASYNC);
	if (hInternet == NULL) return;
	//InternetTrigger ic(hInternet);

	//
	DWORD dwTimout = 86400000;
	InternetSetOption(hInternet, INTERNET_OPTION_CONNECT_TIMEOUT, &dwTimout, sizeof(dwTimout));
	InternetSetOption(hInternet, INTERNET_OPTION_RECEIVE_TIMEOUT, &dwTimout, sizeof(dwTimout));

	//
	INTERNET_STATUS_CALLBACK callbackPointer;
	callbackPointer = InternetSetStatusCallback( hInternet, (INTERNET_STATUS_CALLBACK)CallBack );
	if (callbackPointer == INTERNET_INVALID_STATUS_CALLBACK) return;

	PNOTIFY_CONTEXT pContext = (PNOTIFY_CONTEXT)malloc(sizeof(CONTEXT));
	pContext->id = id;
	InternetOpenUrl( hInternet, url, NULL, 0,
			INTERNET_FLAG_RELOAD | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_NO_CACHE_WRITE,
			(DWORD_PTR)pContext);
}

char buf[256] = {0};

BOOL MekanderSocket(int id, LPCSTR host, unsigned short port, LPCSTR object)
{
	// ���� ����
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, 0);
	if(INVALID_SOCKET==s) return FALSE;

	// ����
	SOCKADDR_IN serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = ::htons(port);
	serveraddr.sin_addr.s_addr = inet_addr(host);
	int r = ::connect(s, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(SOCKET_ERROR==r) {
		printf("Cannot Connect : %d\n", id);
		closesocket(s);
		return FALSE;
	} else {
		printf("Connect : %d\n", id);
	}

	// ����ŷ ��ȯ
	// http://www.win32developer.com/tutorial/winsock/winsock_tutorial_3.shtm
	u_long ul = 1;
	if( SOCKET_ERROR==ioctlsocket(s, FIONBIO, &ul) ) return FALSE;

	// ��û
	CHAR request[1024] = {0};
	size_t len = wsprintfA(request, HTTP_GET_REQ, object, host, port);
	::send(s, request, len, 0);

	// �ޱ�
	::recv(s, buf, 1, 0);
	printf("Receive wait : %d\n", id);
	return TRUE;
}

VOID CALLBACK CallBack(
		__in HINTERNET hInternet,
		__in DWORD_PTR dwContext,
		__in DWORD dwInternetStatus,
		__in_bcount(dwStatusInformationLength) LPVOID lpvStatusInformation,
		__in DWORD dwStatusInformationLength
		)
{
	PNOTIFY_CONTEXT pContext = (PNOTIFY_CONTEXT)dwContext;

	switch(dwInternetStatus)
	{
		case INTERNET_STATUS_HANDLE_CREATED:
			printf("INTERNET_STATUS_HANDLE_CREATED - %d\n", pContext->id);
			pContext->hFile = (HINTERNET)(((INTERNET_ASYNC_RESULT*)lpvStatusInformation)->dwResult);

			ZeroMemory(&(pContext->lpIB), sizeof(INTERNET_BUFFERS));
			pContext->lpIB.dwStructSize = sizeof(INTERNET_BUFFERS);
			pContext->lpIB.lpvBuffer = pContext->buf;
			pContext->lpIB.dwBufferLength = 1024;

			pContext->bNotRead = TRUE;

			break;
		case INTERNET_STATUS_REQUEST_COMPLETE:
			if(pContext->bNotRead) {
				printf("InternetReadFileEx - %d\n", pContext->id);
				InternetReadFileEx(pContext->hFile, &(pContext->lpIB), IRF_ASYNC, dwContext);
			} else {
				printf("INTERNET_STATUS_REQUEST_COMPLETE - %d\n", pContext->id);
				InternetCloseHandle(pContext->hFile);
				free(pContext);
				//InternetCloseHandle(hInternet);
			}
			break;
		case INTERNET_STATUS_HANDLE_CLOSING:
			printf("INTERNET_STATUS_HANDLE_CLOSING - %d\n", pContext->id);
			//free(pContext);
			break;
		case INTERNET_STATUS_CONNECTION_CLOSED:
			printf("INTERNET_STATUS_CONNECTION_CLOSED - %d\n", pContext->id);
			break;
		default:
			printf("dwInternetStatus - %d : %u\n", pContext->id, dwInternetStatus);
			break;
	}
}