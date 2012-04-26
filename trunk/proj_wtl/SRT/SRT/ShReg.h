#define SHLM HKEY_LOCAL_MACHINE
#define SHCU HKEY_CURRENT_USER
#define SHCR HKEY_CLASSES_ROOT

UINT SHRegReadInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault);

// SHRegReadString (5) : 디폴트 값없는 버전 + 키값이 없을 경우 새로 만들지 않음
BOOL SHRegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPTSTR lpRet, DWORD nSize);
// SHRegReadString (6) : 디폴트 값존재 버전  + 키값이 없을 경우 새로 만들음 // (위험 요소 내재) // strcpy 부분

BOOL SHRegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, 
   LPTSTR lpRet, DWORD nSize);
BOOL SHRegWriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, UINT nData);
BOOL SHRegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);
// 레지스트리에 특정 키(디렉토리)가 있는지 확인한다.
// HKEY keyDomain : 키도메인
//		(예. HKEY_CLASSES_ROOT, HKEY_CURRENT_USER , HKEY_LOCAL_MACHINE, HKEY_USERS)
// LPCTSTR subKey : 키도메인 아래의 서브 키(찾을 키는 제외한다)
// LPCTSTR checkKey : 찾을 키(서브 키)
// 작성일: 2008.1.13
// 작성자: 김남호
BOOL IsExistKey(HKEY keyDomain, LPCTSTR subKey, LPCTSTR checkKey);
BOOL IsExistKey(HKEY keyDomain, LPCTSTR subKey);
