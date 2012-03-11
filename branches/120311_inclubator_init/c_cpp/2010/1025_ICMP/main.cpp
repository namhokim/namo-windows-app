//#include <winsock2.h>
//#include <iphlpapi.h>
//#include <icmpapi.h>
//#include <stdio.h>
//
//#pragma comment(lib, "iphlpapi.lib")
//#pragma comment(lib, "ws2_32.lib")
//
//LPSTR GetFormattedMessage(LPSTR pMessage, ...);

#include "IPRange.h"
#include <iostream>
#include <process.h>
#include <Windows.h>
//#include <string>

using namespace std;

struct ip_list
{
	ip_list()
		:isSuccess(false)
	{
		::memset(ip, 0, (12+3+1));
	}

	char ip[12+3+1];
	bool isSuccess;
};

void Searching();
unsigned int __stdcall tfSearchDevices( void* );

int __cdecl main(int argc, char **argv)  {

	if(argc>1) {
		bool b = IsICMP_OK(argv[1]);
		if(b) printf("> %s is alive...\n", argv[1]);
		else printf("> %s cannot reached...\n", argv[1]);
	} else {
		printf("Usage. ICMP.exe [IP Address]\n");
	}

	/*while(1) {
		Searching();
		Sleep(10000);
	}*/
	return 0;
}

void Searching()
{
	ip_list lst[254];
	IPRange range("10.16.11.1", "255.255.255.0", true);
	string aIP("255.255.255.255");	// 메모리 재할당을 막기위해 IP의 문자열을 초기화
	
	int i=0;
	HANDLE hThread[254];
	unsigned int threadID[254];

	while( !aIP.assign(range.next()).empty() ) {
		strcpy_s(lst[i].ip, (12+3+1), aIP.c_str());

		hThread[i] = reinterpret_cast<HANDLE>(
						_beginthreadex( NULL, // security
						0,					// stack_size
						&tfSearchDevices,	// start_address
						&lst[i],			// arglist
						0,					// initflag : running(0)
						&threadID[i]		// thread identifier
						) );
		if(0 == hThread[i]) {   // on an error
			break;
		}

		i++;
	}

	//Sleep(10000);
	DWORD dw2 = 0;
	DWORD dw = ::WaitForMultipleObjects(MAXIMUM_WAIT_OBJECTS, &hThread[0], TRUE, INFINITE);	// 0~63
	dw = ::WaitForMultipleObjects(MAXIMUM_WAIT_OBJECTS, &hThread[64], TRUE, INFINITE);		// 64~127
	dw = ::WaitForMultipleObjects(MAXIMUM_WAIT_OBJECTS, &hThread[128], TRUE, INFINITE);		// 128~191
	dw = ::WaitForMultipleObjects(MAXIMUM_WAIT_OBJECTS-2, &hThread[192], TRUE, INFINITE);		// 192~253
	if(dw==WAIT_FAILED) {
		dw2 = ::GetLastError();
	}
	for(int j=0; j<254; ++j) {
		::CloseHandle(hThread[j]);
	}
}

unsigned int __stdcall tfSearchDevices( void* someArgs )
{
	ip_list* lst = static_cast<ip_list *>(someArgs);
	(lst->isSuccess) = IsICMP_OK(lst->ip);

	return 0;
}
