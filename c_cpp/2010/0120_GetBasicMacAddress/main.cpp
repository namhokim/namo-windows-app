#include "NetworkUtil.h"
#include <string>
#include <Windows.h>
#include <ctime>
#include "Ryeol/RyeolHttpClient.h"

#define KST_OFFSET +9
using namespace Ryeol;

std::wstring string2wstring(std::string str) {
	std::wstring wstr(str.length(),L' ');
	copy(str.begin(),str.end(),wstr.begin());
	return wstr;
}

int main()
{
	time_t timer = ::time(NULL);;

	std::string mac = ::GetBasicMacAddress("");
	MessageBoxA(NULL, mac.c_str(), "Mac 林家", MB_OK);

	// Gen UUID
	//CHttpClient         objHttpReq ;
	//CHttpResponse *     pobjHttpRes = NULL ;

	//try {
	//	// Initialize the User Agent
	//	objHttpReq.SetInternet (_T ("My User Agent v1.0")) ;

	//	// Specifies whether to use UTF-8 encoding. (This uses ANSI encoding)
	//	// Default is FALSE
	//	//objHttpReq.SetUseUtf8 (FALSE) ;

	//	// Specifies a code page for ANSI strings. (This uses Korean)
	//	// Default is CP_ACP
	//	//objHttpReq.SetAnsiCodePage (949) ;

	//	// Add user's custom HTTP headers
	//	//objHttpReq.AddHeader (_T ("Ryeol-Magic"), _T ("My Magic Header")) ;

	//	// Add user's parameters
	//	//objHttpReq.AddParam (_T ("where"), _T ("nexearch")) ;

	//	// Send a request
	//	std::string uri = "http://pass.sindoh.com/ws/uuid/gen/mac";
	//	uri.append(mac);
	//	pobjHttpRes = objHttpReq.RequestGet(string2wstring(uri).c_str()) ;

	//	// Place codes to handle the returned CHttpResponse object.
	//	
	//	// Reads the length of the stream
	//	DWORD       dwContSize ;
	//	// If the length is not specified
	//	if ( !pobjHttpRes->GetContentLength (dwContSize) )
	//		dwContSize = 0 ;

	//	const DWORD     cbBuff = 1024 * 10 ;
	//	BYTE            byBuff[cbBuff] ;
	//	DWORD           dwRead ;
	//	size_t          cbTotal = 0 ;

	//	// Reads the data stream returned by the HTTP server.
	//	while ( dwRead = pobjHttpRes->ReadContent (byBuff, cbBuff - 1) ) {
	//		cbTotal += dwRead ;

	//		byBuff[dwRead] = '\0' ;
	//		//printf ("%s", reinterpret_cast<LPCSTR> (byBuff)) ;
	//		MessageBoxA(NULL, reinterpret_cast<LPCSTR> (byBuff), "Mac 林家", MB_OK);
	//	}

	//} catch (httpclientexception e) {
	//	// Place exception handling codes here.
	//}
	return 0;
}