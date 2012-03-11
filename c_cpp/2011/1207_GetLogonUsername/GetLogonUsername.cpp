#include "GetLogonUsername.h"
#include <windows.h>
#include <Lmcons.h>	// for UNLEN
#include <Sddl.h>	// for ConvertSidToStringSid

bool GetLogonUsernameA (
    char * lpBuffer,
    unsigned long* pcbBuffer
    )
{
	return FALSE;
}

#include <tchar.h>	// for _tprintf

BOOL CALLBACK EnumWindowStationProc(
  __in  LPTSTR lpszWindowStation,
  __in  LPARAM lParam
)
{
	_tprintf(_T("%s\n"), lpszWindowStation);
	return TRUE;
}

// How to detect if someone logged in or out?
//  http://www.codeguru.com/forum/showthread.php?t=366043
// +
// http://read.pudn.com/downloads87/sourcecode/windows/network/335717/vcn%E8%BF%9C%E7%A8%8B%E6%8E%A7%E5%88%B6%E6%BA%90%E4%BB%A3%E7%A0%81/Copy%20of%20winvnc/vncService.cpp__.htm
BOOL GetTextualSid(   
				   PSID pSid,            // binary SID   
				   LPTSTR TextualSid,    // buffer for Textual representation of SID   
				   LPDWORD lpdwBufferLen // required/provided TextualSid buffersize   
				   );

bool GetLogonUsernameW (
    wchar_t * lpBuffer,
	unsigned long* pcbBuffer
	)
{
	HDESK hDesk = OpenInputDesktop(0, FALSE, DESKTOP_READOBJECTS);
	if (hDesk) {
		SID sid;
		DWORD dwSidLength = 0;
		BOOL bSuccess = GetUserObjectInformation(hDesk, UOI_USER_SID, &sid, sizeof(SID), &dwSidLength);
		if (!bSuccess && ERROR_INSUFFICIENT_BUFFER == GetLastError()) {
			SID *psid = (SID*)malloc(dwSidLength);
			if (psid) {
				bSuccess = GetUserObjectInformation(hDesk, UOI_USER_SID, psid, dwSidLength, &dwSidLength);
				if(bSuccess) {
					// SID -> name
					LPTSTR StringSid = NULL;
					bSuccess = ConvertSidToStringSid(psid, &StringSid);
					LocalFree(StringSid);
				}
				/*TCHAR sidbuffer[1024];  
				ULONG bufflen=1024; 
				GetTextualSid(psid, sidbuffer, &bufflen);*/


				free(psid);
			}
		}
		if (bSuccess) {
			if (dwSidLength) {
				// a user has loged on
			}
			else {
				// no user
			}
		}
		CloseDesktop(hDesk);
	}

//	LPTSTR lpszWinSta = _T("WinSta0");
//	LPTSTR lpszDesktop = _T("Default");
//	//EnumWindowStations(EnumWindowStationProc, 0);
//
//	// 현재 스테이션 백업
//	HWINSTA bWinSta = GetProcessWindowStation();
//	if(0==bWinSta) return false;
//
//	
//	HWINSTA hObj = OpenWindowStation(lpszWinSta, FALSE, WINSTA_ALL_ACCESS);
//	if(NULL==hObj) {
//		// 현재 스테이션 복원
//		SetProcessWindowStation(bWinSta);
//		CloseWindowStation(bWinSta);
//
//		return false;
//	}
//
//	BOOL bRes = SetProcessWindowStation(hObj);
//
//	// 데스크탑 획득
//	HDESK hDeskDef = ::OpenDesktop(
//					lpszDesktop,		// The name of the desktop to be opened
//					0,
//					FALSE,				// processes created by this process will inherit the handle
//					DESKTOP_READOBJECTS	// The access to the desktop.
//					);
//	if(NULL==hDeskDef) {
//		// 현재 스테이션 복원
//		SetProcessWindowStation(bWinSta);
//		CloseWindowStation(bWinSta);
//
//		return false;
//	}
//
//	DWORD nLengthNeeded = 0;
//	bRes = GetUserObjectInformation(hDeskDef, UOI_USER_SID, NULL, 0, &nLengthNeeded);
//	if( (0==bRes&&ERROR_INSUFFICIENT_BUFFER!=::GetLastError()) || 0==nLengthNeeded) {
//NG:
//		// 데스크탑 닫기
//		::CloseDesktop(hDeskDef);
//
//		// 현재 스테이션 복원
//		SetProcessWindowStation(bWinSta);
//		CloseWindowStation(bWinSta);
//
//		return false;
//	}
//
//	// 동적할당
//	LPBYTE bufSID = (LPBYTE)malloc(nLengthNeeded);
//	if(NULL==bufSID) {
//		goto NG;
//	}
//	DWORD nLength = nLengthNeeded;
//	nLengthNeeded = 0;
//	bRes = GetUserObjectInformation(hDeskDef, UOI_USER_SID, bufSID, nLength, &nLengthNeeded);
//
//	// SID 구조체
//	PISID pSID = (PISID)bufSID;
//
//	// 동적해제
//	free(bufSID);
//
//	//GetUserObjectInformation(hObj, UOI_USER_SID, pvInfo, nLength, &nLengthNeeded);
//
//	// 현재 스테이션 복원
//	SetProcessWindowStation(bWinSta);

	return FALSE;
}

BOOL GetTextualSid(   
				   PSID pSid,            // binary SID   
				   LPTSTR TextualSid,    // buffer for Textual representation of SID   
				   LPDWORD lpdwBufferLen // required/provided TextualSid buffersize   
				   )   
{   
	PSID_IDENTIFIER_AUTHORITY psia;   
	DWORD dwSubAuthorities;   
	DWORD dwSidRev=SID_REVISION;   
	DWORD dwCounter;   
	DWORD dwSidSize;   

	// Validate the binary SID.   

	if(!IsValidSid(pSid)) return FALSE;   

	// Get the identifier authority value from the SID.   

	psia = GetSidIdentifierAuthority(pSid);   

	// Get the number of subauthorities in the SID.   

	dwSubAuthorities = *GetSidSubAuthorityCount(pSid);   

	// Compute the buffer length.   
	// S-SID_REVISION- + IdentifierAuthority- + subauthorities- + NULL   

	dwSidSize=(15 + 12 + (12 * dwSubAuthorities) + 1) * sizeof(TCHAR);   

	// Check input buffer length.   
	// If too small, indicate the proper size and set last error.   

	if (*lpdwBufferLen < dwSidSize)   
	{   
		*lpdwBufferLen = dwSidSize;   
		SetLastError(ERROR_INSUFFICIENT_BUFFER);   
		return FALSE;   
	}   

	// Add 'S' prefix and revision number to the string.   

	dwSidSize=wsprintf(TextualSid, TEXT("S-%lu-"), dwSidRev );   

	// Add SID identifier authority to the string.   

	if ( (psia->Value[0] != 0) || (psia->Value[1] != 0) )   
	{   
		dwSidSize+=wsprintf(TextualSid + lstrlen(TextualSid),   
			TEXT("0x%02hx%02hx%02hx%02hx%02hx%02hx"),   
			(USHORT)psia->Value[0],   
			(USHORT)psia->Value[1],   
			(USHORT)psia->Value[2],   
			(USHORT)psia->Value[3],   
			(USHORT)psia->Value[4],   
			(USHORT)psia->Value[5]);   
	}   
	else   
	{   
		dwSidSize+=wsprintf(TextualSid + lstrlen(TextualSid),   
			TEXT("%lu"),   
			(ULONG)(psia->Value[5]      )   +   
			(ULONG)(psia->Value[4] <  8)   +   
			(ULONG)(psia->Value[3] < 16)   +   
			(ULONG)(psia->Value[2] < 24)   );   
	}   

	// Add SID subauthorities to the string.   
	//   
	for (dwCounter=0 ; dwCounter < dwSubAuthorities ; dwCounter++)   
	{   
		dwSidSize+=wsprintf(TextualSid + dwSidSize, TEXT("-%lu"),   
			*GetSidSubAuthority(pSid, dwCounter) );   
	}   
	return TRUE;   
}   