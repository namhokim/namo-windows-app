#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Wtsapi32.h>	// for WTSEnumerateSessions, WTSQuerySessionInformationA
#include <Ws2def.h>		// for AF_INET
#include <stdio.h>		// for fprint, stdout, stderr
#include <stdlib.h>		// for exit
#include <string>		// for std::string
#include <sstream>		// for std::stringstream
#include "json/json.h"

#pragma comment(lib, "Wtsapi32.lib")

const int ArgNone	= 1;
const char* JSON_NO_DATA = "{number:0,data:null}";

int ShowCurrentConnectedUser();
void GetDetailInfo(DWORD SessionId, Json::Value& item_out);
void GetAddressInfo(PWTS_CLIENT_ADDRESS address, std::string& address_str);

namespace convert {
	std::string W2UTF8(const std::wstring& utf16le);
}

int main(int argc, char **argv)
{
	switch(argc) {
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

namespace convert {
	const wchar_t Pow2_6 = 64;
	const wchar_t Pow2_12 = 4096;

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