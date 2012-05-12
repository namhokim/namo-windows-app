#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>		// for _tcscat_s, _tcschr, _tcslen
#include <Wtsapi32.h>	// for WTSEnumerateSessions, WTSQuerySessionInformationA
#include <Ws2def.h>		// for AF_INET
#include <stdlib.h>		// for EXIT_SUCCESS, EXIT_FAILURE
#include <sstream>		// for std::stringstream
#include <Strsafe.h>	// for StringCchPrintf
#include "json/json.h"
//#include "whoConnect.h"

#pragma comment(lib, "Wtsapi32.lib")

#define MAX_MESSAGE	1024

///////////////////////////////////////////////////////////////////////////////

const char* JSON_NO_DATA = "{\"number\":0,\"data\":null}";

///////////////////////////////////////////////////////////////////////////////

void GetDetailInfo(DWORD SessionId, Json::Value& item_out);
void GetAddressInfo(PWTS_CLIENT_ADDRESS address, std::string& address_str);
int GetMessageTitle(LPWSTR title);

namespace convert {
	std::wstring UTF82W(const std::string& utf8);
	std::string W2UTF8(const std::wstring& utf16le);
} // end of namespace convert

void ErrorExit(DWORD dwCode, std::string& errorString);

///////////////////////////////////////////////////////////////////////////////

namespace wts {

// inner call GetDetailInfo -> GetAddressInfo
int ShowCurrentConnectedUser(Json::Value& out)
{
	PWTS_SESSION_INFOW pSes;
	DWORD dwCount, i, c;
	BOOL bRes;
	Json::Value data(Json::arrayValue);

	// WTSEnumerateSessions function
	// refs. http://msdn.microsoft.com/en-us/library/aa383833(v=vs.85).aspx
	bRes = WTSEnumerateSessionsW(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSes, &dwCount);
	if(0==bRes) {	// if error
		Json::Reader r;
		r.parse(JSON_NO_DATA, out);
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

	// Set result
	out["number"] = (unsigned int)c;
	out["data"] = data;

	return EXIT_SUCCESS;
}

// inner call : GetMessageTitle
int SendMessageToSesstionID(unsigned int SessionId, const wchar_t* message)
{
	DWORD dwTimeout = 60;	// 60 seconds
	DWORD dwTimeoutIndefinitely = 0;	// 
	DWORD Response = 0;

	WCHAR szTitle[MAX_MESSAGE+1] = {0};
	WCHAR szMessage[MAX_MESSAGE+1] = {0};
	GetMessageTitle(szTitle);
	StringCchPrintfW(szMessage, MAX_MESSAGE, L"[답장은 http://localhost:8282/ 에서 가능합니다]\n\n%s", message);

	// http://msdn.microsoft.com/en-us/library/windows/desktop/aa383842(v=vs.85).aspx
	BOOL bRes = WTSSendMessage(
		WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		szTitle, static_cast<DWORD>(wcslen(szTitle)*sizeof(WCHAR)),
		szMessage, static_cast<DWORD>(wcslen(szMessage)*sizeof(WCHAR)),
		MB_OK, dwTimeoutIndefinitely, &Response, FALSE);

	return (0!=bRes);
}

int DisconnectSession(unsigned int SessionId)
{
	// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/aa383830(v=vs.85).aspx
	return WTSDisconnectSession(WTS_CURRENT_SERVER_HANDLE,SessionId, FALSE);
}

unsigned int WaitForEvent(unsigned int* pStausCode, std::string& errorMsg)
{
	DWORD dwEventFlags;
	BOOL bRes;
	bRes = WTSWaitSystemEvent(WTS_CURRENT_SERVER_HANDLE,
		WTS_EVENT_CONNECT | WTS_EVENT_DISCONNECT, &dwEventFlags);
	(*pStausCode) = dwEventFlags;
	if(bRes) return NO_ERROR;
	else {
		DWORD dw = GetLastError();
		ErrorExit(dw, errorMsg);
		return dw;
	}
}

} // end of namespace wts

///////////////////////////////////////////////////////////////////////////////

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
		if(h.size()) {
			item_out["host"] = h;
		} else {
			WCHAR szName[MAX_COMPUTERNAME_LENGTH+1];
			DWORD dwSize = MAX_COMPUTERNAME_LENGTH;
			GetComputerName(szName, &dwSize);
			item_out["host"] = convert::W2UTF8(szName);
		}
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
			address_str.assign("localhost");
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////

int GetMessageTitle(LPWSTR title)
{
	SYSTEMTIME st;
	LPCWSTR MessageFormat = L"8282 서비스 - %s의 메시지";
	
	TCHAR szTime[256] = {0};

	// 지역시간
	GetLocalTime(&st);

	// 쓸 문자열 작성
	GetDateFormatW(LOCALE_USER_DEFAULT, DATE_LONGDATE,
		&st, NULL, szTime, _countof(szTime));
	wcscat_s(szTime, _countof(szTime), TEXT(" "));	// 공백
	GetTimeFormatW(LOCALE_USER_DEFAULT, 0,
		&st, NULL, wcschr(szTime, TEXT('\0')), (int)(_countof(szTime) - wcslen(szTime)));

	// 메시지 생성
	return StringCchPrintfW(title, MAX_MESSAGE, MessageFormat, szTime);
}

///////////////////////////////////////////////////////////////////////////////
// UNICODE converter : UTF16LE <-> UTF8
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

// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/ms680582(v=vs.85).aspx
void ErrorExit(DWORD dwCode, std::string& errorString)
{
	LPVOID lpMsgBuf;

	FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR) &lpMsgBuf,
        0, NULL );

	errorString.assign(convert::W2UTF8((LPWSTR)lpMsgBuf));

	LocalFree(lpMsgBuf);
}