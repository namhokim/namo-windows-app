#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>		// for _tcscat_s, _tcschr, _tcslen
#include <Wtsapi32.h>	// for WTSEnumerateSessions, WTSQuerySessionInformationA
#include <Ws2def.h>		// for AF_INET
#include <stdio.h>		// for fprint, stdout, stderr
#include <stdlib.h>		// for exit
#include <string>		// for std::string
#include <sstream>		// for std::stringstream
#include "json/json.h"

#pragma comment(lib, "Wtsapi32.lib")

const int ArgNone	= 1;
const int ArgTwo	= 3;
const char* JSON_NO_DATA = "{number:0,data:null}";

int ShowCurrentConnectedUser();
void GetDetailInfo(DWORD SessionId, Json::Value& item_out);
void GetAddressInfo(PWTS_CLIENT_ADDRESS address, std::string& address_str);
int SendMessageToSesstionID(DWORD SessionId, LPCWSTR message);
int GetMessageTitle(LPWSTR title);

namespace convert {
	std::wstring UTF82W(const std::string& utf8);
	std::string W2UTF8(const std::wstring& utf16le);
}

int wmain(int argc, wchar_t *argv[])
{
	switch(argc) {
		case ArgTwo:
			{
				DWORD sessionId = _wtoi(argv[1]);
				std::wstring msg( argv[2] );
				return SendMessageToSesstionID(sessionId, msg.c_str());
			}
		case ArgNone:
		default:
			return ShowCurrentConnectedUser();
	}

	return EXIT_SUCCESS;
};

int ShowCurrentConnectedUser()
{
	PWTS_SESSION_INFOW pSes;
	DWORD dwCount, i, c;
	BOOL bRes;
	Json::Value data(Json::arrayValue);

	// WTSEnumerateSessions function
	// refs. http://msdn.microsoft.com/en-us/library/aa383833(v=vs.85).aspx
	bRes = WTSEnumerateSessionsW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSes, &dwCount);
	if(0==bRes) {	// if error
		fprintf(stdout, JSON_NO_DATA);
		return EXIT_FAILURE;
	}	

	for(i=0,c=0; i<dwCount; ++i)
	{
		PWTS_SESSION_INFOW s = &pSes[i];
		if( WTSActive == (s->State) ) {
			Json::Value item;
			item["rdp"] = convert::W2UTF8(s->pWinStationName);
			item["SessionId"] = Json::Int(s->SessionId);
			GetDetailInfo(s->SessionId, item);
			data.append(item);
			c++;
		}
	}
	WTSFreeMemory(pSes);

	Json::Value root;
	root["number"] = (unsigned int)c;
	root["data"] = data;

	Json::FastWriter w;
	printf(w.write(root).c_str());
	return EXIT_SUCCESS;
}

void GetDetailInfo(DWORD SessionId, Json::Value& item_out)
{
	LPWSTR pBuffer;
	DWORD dwBytesReturned;
	BOOL bRes;

	// host
	// WTSQuerySessionInformation function
	// refs. http://msdn.microsoft.com/en-us/library/aa383838(v=vs.85).aspx
	bRes = WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, SessionId,
		WTSClientName, &pBuffer, &dwBytesReturned);
	if(bRes && dwBytesReturned>0) {
		std::string h(convert::W2UTF8(pBuffer));
		if(h.size()) item_out["host"] = h;
		else item_out["host"] = "-";
		WTSFreeMemory(pBuffer);
	}

	// ip
	bRes = WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, SessionId,
		WTSClientAddress, &pBuffer, &dwBytesReturned);
	if(bRes && dwBytesReturned>0) {
		PWTS_CLIENT_ADDRESS addr = (PWTS_CLIENT_ADDRESS)pBuffer;
		std::string address_str;
		GetAddressInfo(addr, address_str);
		item_out["address"] = address_str;
		WTSFreeMemory(pBuffer);
	}
}

void GetAddressInfo(PWTS_CLIENT_ADDRESS address, std::string& address_str)
{
	std::stringstream ss;
	PBYTE ad = address->Address;
	switch(address->AddressFamily) {
		case AF_INET6:
			address_str.assign("IPv6");
			break;
		// below only Windows Server 2003, Windows XP, Windows 2000 Server, and Windows 2000 Professional
		case AF_INET:
			ss << short(ad[2]) << "." << short(ad[3]) << "." << short(ad[4]) << "." << short(ad[5]);
			address_str.assign(ss.str());
			break;
		case AF_IPX:
			address_str.assign("IPX");
			break;
		case AF_NETBIOS:
			address_str.assign("NETBIOS");
			break;
		case AF_UNSPEC:
		default:
			address_str.assign("-");
			break;
	}
}

int SendMessageToSesstionID(DWORD SessionId, LPCWSTR message)
{
	DWORD dwTimeout = 60;	// 60 seconds
	DWORD dwTimeoutIndefinitely = 0;	// 
	DWORD Response = 0;

	WCHAR szTitle[1024] = {0};
	WCHAR szMessage[1024] = {0};
	GetMessageTitle(szTitle);
	wsprintfW(szMessage, message);

	// http://msdn.microsoft.com/en-us/library/windows/desktop/aa383842(v=vs.85).aspx
	BOOL bRes = WTSSendMessage(
		WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		szTitle, static_cast<DWORD>(wcslen(szTitle)*sizeof(WCHAR)),
		szMessage, static_cast<DWORD>(wcslen(szMessage)*sizeof(WCHAR)),
		MB_OK, dwTimeoutIndefinitely, &Response, FALSE);

	return (0!=bRes);
}

int GetMessageTitle(LPWSTR title)
{
	SYSTEMTIME st;
	LPCWSTR MessageFormat = L"8282 ���� - %s�� �޽���";
	
	WCHAR szTime[256] = {0};

	// �����ð�
	GetLocalTime(&st);

	// �� ���ڿ� �ۼ�
	GetDateFormatW(LOCALE_USER_DEFAULT, DATE_LONGDATE,
		&st, NULL, szTime, _countof(szTime));
	_tcscat_s(szTime, _countof(szTime), TEXT(" "));	// ����
	GetTimeFormatW(LOCALE_USER_DEFAULT, 0,
		&st, NULL, _tcschr(szTime, TEXT('\0')), (int)(_countof(szTime) - _tcslen(szTime)));

	// �޽��� ����
	return wsprintfW(title, MessageFormat, szTime);
}

namespace convert {
	const wchar_t Pow2_6 = 64;
	const wchar_t Pow2_12 = 4096;

	std::wstring UTF82W(const std::string& utf8)
	{
		std::wstring out;

		// refs. http://snowdeer.egloos.com/2401298
		size_t size = utf8.size();
		try {
			out.reserve(size);		// for prevent re-allocation
			for(size_t i=0; i<size; ++i) {
				wchar_t ch;
				short sh = (unsigned char)utf8[i];
				if (sh < 0x80) {
					ch = sh;
				} else if (sh  < 0xE0) {
					short sh_next = (unsigned char)utf8.at(i+1);	// can be exception
					ch = ((sh & 0x1F) << 6) | (sh_next & 0x3F);
					i = i + 1;
				} else {
					short sh_next = (unsigned char)utf8[i+1];
					short sh_next_next = (unsigned char)utf8.at(i+2);	// can be exception
					ch = ((sh & 0x0F) << 12) | ((sh_next & 0x3F) << 6) | sh_next_next & 0x3F;
					i = i + 2;
				}
				out.push_back(ch);
			}
		} catch(std::out_of_range) {
			out.clear();
		}
		return out;
	}

	std::string W2UTF8(const std::wstring& utf16le)
	{	
		std::string out;

		// refs. http://support.microsoft.com/kb/601368/ko
		size_t size = utf16le.size();
		out.reserve(size);
		for(size_t i=0; i<size; ++i) {
			wchar_t uc = utf16le[i];

			if(uc <= 0x7f) {
				out.push_back((char)uc);
			} else if (uc <= 0x7ff) {
				char utf8[3];
				utf8[0] = (char) 0xc0 + uc / Pow2_6;
				utf8[1] = (char) 0x80 + uc % Pow2_6;
				utf8[2] = (char) '\0';
				out.append(utf8);
			} else if (uc <= 0xffff) {
				unsigned char utf8[4];
				utf8[0] = (unsigned char) 0xe0 + uc / Pow2_12;
				utf8[1] = (unsigned char) 0x80 + uc / Pow2_6 % Pow2_6;
				utf8[2] = (unsigned char) 0x80 + uc % Pow2_6;
				utf8[3] = (char) '\0';
				out.append((char*)utf8);
			}
		}
		return out;
	}
} // namespace convert