#define RESTORE	0
#define BACKUP	1
//#define MAX_REGISTRY_NAME	512

// ���ǻ���! : swName�� ������Ʈ���� ������ S/W �̸��̴�.
//			   ����ϴ� ���α׷��� �̸��� ��� �ٸ��� ������ �ؾ� �߸��� ��ġ�� ��Ÿ���� �ʴ´�.
BOOL WindowPosition(HWND hWnd, LPCTSTR swName, int actionCode);		// �������� ��ġ�� ����(����/���)

BOOL RestoreWindowPosition(HWND hWnd, LPCTSTR swName);	// ������ ���� -> ������ ����
BOOL BackupWindowPosition(HWND hWnd, LPCTSTR swName);		// ���� ������ ���� -> ���� ���

// ��� : ȭ�鿵���� â������ ���Ͽ� ��ü ��ġ�� ��ȯ�Ѵ�(�ð��� ��� ������ȯ)
BOOL GetAlternatePosition(HWND hWnd, RECT &getRect);

// ��� : ȭ�鿵���� â������ ����Ͽ� �ð��� ����� ���Ѵ�.
BOOL GetVisuallyCentered(RECT desktop, RECT currWnd, RECT &infoVC);	// �ð������� �����ġ ���ϱ�


// ���� ��������
// 1. ���ɻ� : ������Ʈ�� �б⸦ �ѹ��� �ϰ� (������; ��) ����)���� ����� split�ϴ� ���� ����.
// 2. �ػ� ����(��->��) ��, ������ ȯ�溸�� ����� �ػ󵵰� ���� ��, ������ ������ �Ұ��� ����.
//	BOOL CheckValidPosition(hWnd, rt);