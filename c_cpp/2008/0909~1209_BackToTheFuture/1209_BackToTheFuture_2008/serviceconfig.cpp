#include <windows.h>
#include <iostream>
using namespace std;

#define EXENAME L"BackToTheFuture.exe"
#define SRVNAME L"BackToTheFutureService"
#define DISPNAME L"Back To The Future Service"
#define DESCNAME L"Jump Time"

void InstallService()
{
	SC_HANDLE hScm, hSrv;
	WCHAR SrvPath[MAX_PATH];
	SERVICE_DESCRIPTION lpDes;

	// SCM�� ����
	hScm=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
	if (hScm==NULL) {
		cerr << "SCM�� �� �� �����ϴ�.\n";
		return;
	}

	// ����� ���� ������ �ִ��� ������ ���� ��θ� ���Ѵ�.
	GetCurrentDirectory(MAX_PATH,SrvPath);
	wcscat_s(SrvPath, L"\\");
	wcscat_s(SrvPath, EXENAME);
	if (GetFileAttributes(SrvPath) == INVALID_FILE_ATTRIBUTES) {
		CloseServiceHandle(hScm);
		cerr << "���� ���丮�� ���� ������ �����ϴ�.\n";
		return;
	}

	// ���񽺸� ����Ѵ�.
	hSrv=CreateService(hScm,SRVNAME,DISPNAME,SERVICE_PAUSE_CONTINUE | SERVICE_CHANGE_CONFIG,
		SERVICE_WIN32_OWN_PROCESS,SERVICE_DEMAND_START,SERVICE_ERROR_IGNORE,SrvPath,
		NULL,NULL,NULL,NULL,NULL);
	if (hSrv==NULL) {
		cerr << "��ġ���� ���߽��ϴ�.\n";
	} else {
		// ������ ����Ѵ�.
		lpDes.lpDescription=DESCNAME;
		ChangeServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, &lpDes);

		cerr << "��ġ�߽��ϴ�.\n";
		CloseServiceHandle(hSrv);
	}

	CloseServiceHandle(hScm);
}

// ���񽺸� �����Ѵ�.
void UnInstallService()
{
	SC_HANDLE hScm, hSrv;
	SERVICE_STATUS ss;

	// SCM�� ����
	hScm=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
	if (hScm==NULL) {
		cerr << "SCM�� �� �� �����ϴ�.\n";
		return;
	}

	// ������ �ڵ��� ���Ѵ�.
	hSrv=OpenService(hScm,SRVNAME,SERVICE_ALL_ACCESS);
	if (hSrv==NULL) {
		CloseServiceHandle(hScm);
		cerr << "���񽺰� ��ġ�Ǿ� ���� �ʽ��ϴ�.\n";
		return;
	}

	// �������̸� ������Ų��.
	QueryServiceStatus(hSrv,&ss);
	if (ss.dwCurrentState != SERVICE_STOPPED) {
		ControlService(hSrv,SERVICE_CONTROL_STOP,&ss);
		Sleep(2000);
	}

	// ���� ����
	if (DeleteService(hSrv)) {
		cerr << "���񽺸� �����߽��ϴ�.\n";
	} else {
		cerr << "���񽺸� �������� ���߽��ϴ�.\n";
	}
	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);
}