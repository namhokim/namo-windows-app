#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Wtsapi32.h>	// for WTSEnumerateSessions, WTSQuerySessionInformationA
#include <Ws2def.h>		// for AF_INET
#include <stdio.h>		// for fprint, stdout, stderr
#include <stdlib.h>		// for exit
#include <string>		// for std::string
#include <vector>		// for convert
#include <sstream>		// for std::stringstream
#include "json/json.h"

#pragma comment(lib, "Wtsapi32.lib")

const char* JSON_NO_DATA = "{number:0,data:null}";

void GetDetailInfo(DWORD SessionId, Json::Value& item_out);
void GetAddressInfo(PWTS_CLIENT_ADDRESS address, std::string& address_str);

namespace convert {
	std::string W2UTF8(const std::wstring& utf16le);
}

int main(int argc, char **argv)
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
};

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
		std::string h(convert::W2UTF8(std::wstring(pBuffer)));
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
	std::string W2UTF8(const std::wstring& utf16le)
	{	
		int needBuf = WideCharToMultiByte(CP_UTF8, 0, utf16le.c_str(), (int)utf16le.size(), NULL, 0, NULL, NULL);
		if(needBuf > 0) {
			std::vector<BYTE> buf(needBuf+1);
			WideCharToMultiByte(CP_UTF8, 0, utf16le.c_str(), (int)utf16le.size(), (LPSTR)&buf[0], (int)buf.size(), NULL, NULL);
			std::string out(buf.begin(), buf.end());
			return out;
		}
		else {
			return "";
		}
	}
} // namespace convert