#include "phyAddr.h"

#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>		// for _tcscat_s, _tcschr, _tcslen
#include <Iphlpapi.h>	// Requirements: Windows 2000 Professional

#pragma comment(lib, "Iphlpapi.lib")

#define NO_INTERFACE (-1)

//#define USE_REGISTRY_CACHE		// 필요에 따라 주석 추가(사용시 UAC 확인)
//#define USE_REGISTRY_CACHE_DATE	// 활성화시 캐시 작성날짜도 기록

#define REGISTRY_CACHE_HKEY		HKEY_LOCAL_MACHINE
#define REGISTRY_CACHE_SUB_KEY	TEXT("SOFTWARE\\COMPANY\\PRODUCT")
#define	REGISTRY_CACHE_DATA		TEXT("phyAddr")
#define	REGISTRY_CACHE_DATETIME	TEXT("phyAddr_written")

namespace Network
{
	const size_t buf_size = 5;

	// 함수 전방 선언
	DWORD GetBestInterfaceIdx();
	BOOL SetMacAddressByIndex(const DWORD dwIf, std::vector<BYTE>& out);
	BOOL GetMacAddressByRegistry(std::vector<BYTE>& out);
	BOOL SetRegistryByMacAddress(const std::vector<BYTE>& out);
	BOOL SetRegistryByWrittenDatetime(HKEY hkey);

	// 외부 노출된 함수 구현
	bool GetBestInterfaceAddress(std::vector<BYTE>& out)
	{
#ifdef USE_REGISTRY_CACHE
		BOOL bSuccess = GetMacAddressByRegistry(out);
		if(bSuccess) return true;
#endif
		DWORD dwIf = Network::GetBestInterfaceIdx();
		if(NO_INTERFACE==dwIf) {
			return false;
		}

		if(Network::SetMacAddressByIndex(dwIf, out)) {
			return true;
		} else {
			return false;
		}
	}


	// 전방 선언된 함수 구현
	DWORD GetBestInterfaceIdx()
	{
		IPAddr in;
		DWORD dwIf;
		ZeroMemory(&in, sizeof(IPAddr));	// IPv4 : 0.0.0.0

		// GetBestInterface function
		// http://msdn.microsoft.com/en-us/library/windows/desktop/aa365920(v=vs.85).aspx
		DWORD dw = ::GetBestInterface(in, &dwIf);

		return ( (NO_ERROR==dw) ? dwIf : NO_INTERFACE);
	}

	BOOL SetMacAddressByIndex(const DWORD dwIf, std::vector<BYTE>& out)
	{
		// GetAdaptersInfo function
		// refs. http://msdn.microsoft.com/en-us/library/windows/desktop/aa365917(v=vs.85).aspx

		// 동적 할당 크기 구함(1차 시도)
		ULONG ulOutBufLen = 0;	// ERROR_BUFFER_OVERFLOW 유도
		if (::GetAdaptersInfo(NULL, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
			PIP_ADAPTER_INFO pAdapterInfo;

			std::vector<BYTE> alloc(ulOutBufLen);
			pAdapterInfo = (PIP_ADAPTER_INFO)&alloc[0];

			// 실제 값을 받아옴(2차 시도)
			if (::GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_SUCCESS) {
				PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
				while (pAdapter) {
					if(dwIf == (pAdapter->Index)) {	// interface index matched
						for (UINT i=0; i<pAdapter->AddressLength; ++i) {
							out.push_back(pAdapter->Address[i]);
						}
#ifdef USE_REGISTRY_CACHE
						// 레지스트리에 기록한다.
						SetRegistryByMacAddress(out);
#endif
						return TRUE;
					}
					pAdapter = pAdapter->Next;
				} // while (pAdapter)
			}
		}
		return FALSE;
	}

#ifdef USE_REGISTRY_CACHE

	HKEY hKey = REGISTRY_CACHE_HKEY;
	LPCTSTR lsSubkey = REGISTRY_CACHE_SUB_KEY;
	LPCTSTR lpValueName = REGISTRY_CACHE_DATA;

	BOOL GetMacAddressByRegistry(std::vector<BYTE>& out)
	{
		LONG lRet;
		HKEY hkResult;
		lRet = RegOpenKeyEx(hKey, lsSubkey, 0, KEY_QUERY_VALUE | KEY_WOW64_32KEY, &hkResult);
		if(ERROR_SUCCESS!=lRet) return FALSE;

		DWORD type, cbData = 0;
		lRet = RegQueryValueEx(hkResult, lpValueName, NULL, &type, NULL, &cbData);
		if(ERROR_SUCCESS==lRet && REG_BINARY==type && cbData>0) {	// 데이터가 존재한다면
			out.clear();
			out.resize(cbData);
			lRet = RegQueryValueEx(hkResult, lpValueName, NULL, NULL, &out[0], &cbData);
		} else {
			lRet = !ERROR_SUCCESS;
		}
		RegCloseKey(hkResult);

		return (ERROR_SUCCESS==lRet ? TRUE : FALSE);
	}

	BOOL SetRegistryByMacAddress(const std::vector<BYTE>& out)
	{
		LONG lRet;
		HKEY hkResult;
		DWORD dwDisposition;

		if(0==out.size()) return FALSE;

		// 키 없으면 생성
		lRet = RegCreateKeyEx(hKey, lsSubkey, 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_WRITE | KEY_QUERY_VALUE | KEY_WOW64_32KEY , NULL, &hkResult, &dwDisposition);
		// Windows Vista 이후는 ERROR_ACCESS_DENIED 가능성 존재
		if(ERROR_SUCCESS!=lRet) return FALSE;

		// 존재시
		DWORD type, cbData = 0;
		lRet = RegQueryValueEx(hkResult, lpValueName, NULL, &type, NULL, &cbData);
		if(ERROR_SUCCESS==lRet && REG_BINARY!=type) {	// 존재하나 타입이 다르면
			RegDeleteValue(hkResult, lpValueName);		// 제거
		}
		
		// 기록
		lRet = RegSetValueEx(hkResult, lpValueName, 0, REG_BINARY, &out[0], out.size());
#ifdef USE_REGISTRY_CACHE_DATE	// 날짜 기록 활성화시
		SetRegistryByWrittenDatetime(hkResult);	// 날짜도 기록
#endif
		RegCloseKey(hkResult);

		return (ERROR_SUCCESS==lRet ? TRUE : FALSE);
	}

	BOOL SetRegistryByWrittenDatetime(HKEY hkey)
	{
		LONG lRet;
		SYSTEMTIME st;
		TCHAR szBuf[256] = {0};
		LPCTSTR lpValueName = REGISTRY_CACHE_DATETIME;

		if(NULL==hkey) return FALSE;	// 파라메터 체크
		
		// 지역시간
		GetLocalTime(&st);
		
		// 쓸 문자열 작성
		GetDateFormat(LOCALE_USER_DEFAULT, DATE_LONGDATE,
			&st, NULL, szBuf, _countof(szBuf));
		_tcscat_s(szBuf, _countof(szBuf), TEXT(" "));	// 공백
		GetTimeFormat(LOCALE_USER_DEFAULT, 0,
			&st, NULL, _tcschr(szBuf, TEXT('\0')), (int)(_countof(szBuf) - _tcslen(szBuf)));

		// 기록
		lRet = RegSetValueEx(hkey, lpValueName, 0, REG_SZ,
			reinterpret_cast<LPBYTE>(szBuf), _tcslen(szBuf)*sizeof(TCHAR));
		return lRet;
	}
#endif /* USE_REGISTRY_CACHE */

} // end of namespace Network
