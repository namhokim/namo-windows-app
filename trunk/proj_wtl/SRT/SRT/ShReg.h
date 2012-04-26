#define SHLM HKEY_LOCAL_MACHINE
#define SHCU HKEY_CURRENT_USER
#define SHCR HKEY_CLASSES_ROOT

UINT SHRegReadInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault);

// SHRegReadString (5) : ����Ʈ ������ ���� + Ű���� ���� ��� ���� ������ ����
BOOL SHRegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPTSTR lpRet, DWORD nSize);
// SHRegReadString (6) : ����Ʈ ������ ����  + Ű���� ���� ��� ���� ������ // (���� ��� ����) // strcpy �κ�

BOOL SHRegReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, 
   LPTSTR lpRet, DWORD nSize);
BOOL SHRegWriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, UINT nData);
BOOL SHRegWriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);
// ������Ʈ���� Ư�� Ű(���丮)�� �ִ��� Ȯ���Ѵ�.
// HKEY keyDomain : Ű������
//		(��. HKEY_CLASSES_ROOT, HKEY_CURRENT_USER , HKEY_LOCAL_MACHINE, HKEY_USERS)
// LPCTSTR subKey : Ű������ �Ʒ��� ���� Ű(ã�� Ű�� �����Ѵ�)
// LPCTSTR checkKey : ã�� Ű(���� Ű)
// �ۼ���: 2008.1.13
// �ۼ���: �賲ȣ
BOOL IsExistKey(HKEY keyDomain, LPCTSTR subKey, LPCTSTR checkKey);
BOOL IsExistKey(HKEY keyDomain, LPCTSTR subKey);
