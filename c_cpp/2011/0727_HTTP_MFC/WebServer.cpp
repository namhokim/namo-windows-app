#include "StdAfx.h"
#include "WebServer.h"
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

namespace Time
{
	void Get(string& out) {
		SYSTEMTIME st;
		::GetLocalTime(&st);
		
		stringstream ss;
		ss.imbue(std::locale::classic());
		ss << setfill('0')
			<< setw(4) << st.wYear << '-'
			<< setw(2) << st.wMonth << '-' << setw(2) << st.wDay << ' '
			<< setw(2) << st.wHour << ':' << setw(2) << st.wMinute << ':' << setw(2) << st.wSecond << '.'
			<< setw(3) << st.wMilliseconds;
		out = ss.str();
	}

	void Get(wstring& out) {
		SYSTEMTIME st;
		::GetLocalTime(&st);

		wstringstream wss;
		wss.imbue(std::locale::classic());
		wss << setfill(L'0')
			<< setw(4) << st.wYear << L'-'
			<< setw(2) << st.wMonth << L'-' << setw(2) << st.wDay << L' ' 
			<< setw(2) << st.wHour << L':' << setw(2) << st.wMinute << L':' << setw(2) << st.wSecond << L'.'
			<< setw(3) << st.wMilliseconds;
		out = wss.str();
	}
}

namespace DB
{
	UserInfoDB _db;

	const UserInfoDB* GetDB() {
		return (&_db);
	}

	// 전역상수(DB에 있다고 가정)
	const DWORD dwDefaultQuota	= 10;	//INFINITE;
	const DWORD dwLoginTime		= 30;	// sec
	const DWORD dwDispLogoTime	= 1900;	// ms
	const DWORD dwDispExceedQuataScreen = 5000;	// ms

	void SelectFromUserInfoWhere(LPUSER_INFO lpUserInfo)
	{
		UserInfoDB::iterator pos;
		pos = _db.find(lpUserInfo->card_number);
		if(pos!=_db.end()) {	// 존재
			lpUserInfo->dwQuota = pos->second.dwQuota;
			lpUserInfo->dwUsed = pos->second.dwUsed;
			// 일일쿼터와 당일남은월간쿼터를 비교해서 제한이 먼저 걸리는 타입을 반환
			// 동일할 경우는 TYPE_LIMIT_BY_MONTHLY_QUOTA 를 반환
			lpUserInfo->dwLimitType = TYPE_LIMIT_BY_DAILY_QUOTA;

			// update touch time
			Time::Get(pos->second.touch_time);
		} else {				// 새로 추가
			lpUserInfo->dwQuota = dwDefaultQuota;
			lpUserInfo->dwUsed = 0;
			lpUserInfo->dwLimitType = TYPE_LIMIT_BY_DAILY_QUOTA;
			Time::Get(lpUserInfo->touch_time);
			
			_db.insert(UserInfoDB::value_type(lpUserInfo->card_number, (*lpUserInfo)));
		}
	}

	void UpdateUserInfoWhere(LPUSER_INFO lpUserInfo)
	{
		UserInfoDB::iterator pos;
		pos = _db.find(lpUserInfo->card_number);
		if(pos!=_db.end()) {	// 존재
			pos->second.dwQuota	= lpUserInfo->dwQuota;
			pos->second.dwUsed	= lpUserInfo->dwUsed;

			// update touch time
			Time::Get(pos->second.touch_time);
		} else {				// 새로 추가
			// NG!
		}
	}
}

namespace WebServer
{
	HWND	hWnd = NULL;
	LPCTSTR	szAgent = _T("FREE4 Terminal");

	BOOL GetUserInfo(LPUSER_INFO lpUserInfo)
	{
		CInternetSession session(szAgent);
		CHttpConnection* pServer = NULL;
		CHttpFile* pFile = NULL;
		DWORD dwRet;
		BYTE szBuff[1024] = {0};
		vector<BYTE> szContent;

		//try {
			CString strServerName, strObject;
			INTERNET_PORT nPort;

			strServerName = _T("127.0.0.1");
			strObject = _T("/mirus/");
			nPort = 8080;

			pServer = session.GetHttpConnection(strServerName, nPort);
		//	pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject);
		//	pFile->SendRequest();
		//	pFile->QueryInfoStatusCode(dwRet);
		//	if (dwRet == HTTP_STATUS_OK) {
		//		UINT nRead = pFile->Read(szBuff, 1023);
		//		while (nRead > 0)
		//		{
		//			szContent.insert(szContent.end(), &szBuff[0], &szBuff[nRead]);
		//			nRead = pFile->Read(szBuff, 1023);
		//		}
		//		cout << string(szContent.begin(), szContent.end()) << endl;
		//	}

		//} catch (CInternetException* pEx)
		//{
		//	//catch errors from WinInet
		//	TCHAR szMsgErr[1024]={0};
		//	pEx->GetErrorMessage(szMsgErr, 1024);
		//	MessageBox(NULL, szMsgErr, 0, MB_OK);
		//	pEx->Delete();
		//}
		//pFile->Close();
		//pServer->Close();
		session.Close();


		DB::SelectFromUserInfoWhere(lpUserInfo);
		if(hWnd!=NULL) ::SendMessage(hWnd, WM_UPDATE_VIEW, (WPARAM)lpUserInfo->card_number.c_str(), NULL);

		return TRUE;
	}

	BOOL SetUserInfo(CONST LPUSER_INFO lpUserInfo)
	{
		DB::UpdateUserInfoWhere(lpUserInfo);
		if(hWnd!=NULL) ::SendMessage(hWnd, WM_UPDATE_VIEW, (WPARAM)lpUserInfo->card_number.c_str(), NULL);

		return TRUE;
	}

	BOOL GetConfigurationInfo(LPCONF_INFO lpConfInfo)
	{
		lpConfInfo->dwLoginTime		= DB::dwLoginTime;
		lpConfInfo->dwDispLogoTime	= DB::dwDispLogoTime;
		lpConfInfo->dwDispExceedQuataScreen = DB::dwDispExceedQuataScreen;
		return TRUE;
	}
}