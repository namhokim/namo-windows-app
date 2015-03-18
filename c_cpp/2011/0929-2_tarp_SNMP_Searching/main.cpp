#include <cstdio>
#include <cstdlib>
#include "SearchingModule.h"

using namespace std;

bool ParsingAndPut(char* line, LIST_OF_DEVICES& dev);

void Usage()
{
	printf("Usage:\n > tarp [-t|-c] 10.16.11.1/24 10.15.11.1/16\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	// Check Args
	if(argc < 2) Usage();

	// Args Parsing
	LISTS_OF_DEVICE list;
	bool wfTP, wfCC;
	wfTP = wfCC = false;
	for(int i=1; i<argc; ++i) {
		if (wfTP || wfCC) {
			int v = atoi(argv[i]);
			if(0!=v && wfTP) Properties::SetSNMPTimeout(v);
			if(0!=v && wfCC) Properties::SetConcurrentConnect(v);
			wfTP = wfCC = false;
		}
		if('-'==argv[i][0]) {
			switch(tolower(argv[i][1])) {
				case 't':	// -t
					wfTP = true;
					break;
				case 'c':	// -c
					wfCC = true;
					break;
				default:
					wfTP = wfCC = false;
			}
		} else {
			LIST_OF_DEVICES aL;
			if(ParsingAndPut(argv[i], aL)) {
				list.push_back(aL);
			}
		}
	}

	// Execute
	GetChangeDevices(list);

	return 0;
}

inline bool IsValidChar(char ch)
{
	switch (ch) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '.':
		case '/':
			return true;
	}
	return false;
}

#include <WinSock2.h>
// [0-9./] 만 허용
bool ParsingAndPut(char* line, LIST_OF_DEVICES& dev)
{
	// 허용문자 검사
	size_t len = strlen(line);
	for(size_t i=0; i< len; i++) {
		if(!IsValidChar(line[i])) {
			printf(" ERROR: Invalid Parameter Character > %c\n", line[i]);
			return false;
		}
	}

	// '/'을 기준으로 문자열을 나눔
	const char* subnet = strchr(line, '/');
	if(NULL==subnet) return false;	// line에 '/'가 포함되어 있지 않다.
	len = strlen(subnet);
	if(len < 2) return false;

	// 문자열 분리
	line[subnet - line] = '\0';
	subnet++;

	// 서브넷: 문자열 -> 정수형
	int iSubnet = atoi(subnet);
	if(iSubnet>32 || iSubnet < 0) return false;
	unsigned long ulSubnet = 0;
	for(int i=0; i<iSubnet; i++) {
		ulSubnet = ulSubnet | (0x80000000 >> i);
	}

	struct in_addr tmp_in_addr;
	tmp_in_addr.s_addr = ntohl(ulSubnet);	// Little Endian -> Big Endian(TCP/IP network byte order)

	// Set
	dev.ip = line;
	dev.subnet = inet_ntoa(tmp_in_addr);
	
	return true;
}