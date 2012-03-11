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

	// SCM을 연다
	hScm=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
	if (hScm==NULL) {
		cerr << "SCM을 열 수 없습니다.\n";
		return;
	}

	// 등록할 서비스 파일이 있는지 조사해 보고 경로를 구한다.
	GetCurrentDirectory(MAX_PATH,SrvPath);
	wcscat_s(SrvPath, L"\\");
	wcscat_s(SrvPath, EXENAME);
	if (GetFileAttributes(SrvPath) == INVALID_FILE_ATTRIBUTES) {
		CloseServiceHandle(hScm);
		cerr << "같은 디렉토리에 서비스 파일이 없습니다.\n";
		return;
	}

	// 서비스를 등록한다.
	hSrv=CreateService(hScm,SRVNAME,DISPNAME,SERVICE_PAUSE_CONTINUE | SERVICE_CHANGE_CONFIG,
		SERVICE_WIN32_OWN_PROCESS,SERVICE_DEMAND_START,SERVICE_ERROR_IGNORE,SrvPath,
		NULL,NULL,NULL,NULL,NULL);
	if (hSrv==NULL) {
		cerr << "설치하지 못했습니다.\n";
	} else {
		// 설명을 등록한다.
		lpDes.lpDescription=DESCNAME;
		ChangeServiceConfig2(hSrv, SERVICE_CONFIG_DESCRIPTION, &lpDes);

		cerr << "설치했습니다.\n";
		CloseServiceHandle(hSrv);
	}

	CloseServiceHandle(hScm);
}

// 서비스를 제거한다.
void UnInstallService()
{
	SC_HANDLE hScm, hSrv;
	SERVICE_STATUS ss;

	// SCM을 연다
	hScm=OpenSCManager(NULL,NULL,SC_MANAGER_CREATE_SERVICE);
	if (hScm==NULL) {
		cerr << "SCM을 열 수 없습니다.\n";
		return;
	}

	// 서비스의 핸들을 구한다.
	hSrv=OpenService(hScm,SRVNAME,SERVICE_ALL_ACCESS);
	if (hSrv==NULL) {
		CloseServiceHandle(hScm);
		cerr << "서비스가 설치되어 있지 않습니다.\n";
		return;
	}

	// 실행중이면 중지시킨다.
	QueryServiceStatus(hSrv,&ss);
	if (ss.dwCurrentState != SERVICE_STOPPED) {
		ControlService(hSrv,SERVICE_CONTROL_STOP,&ss);
		Sleep(2000);
	}

	// 서비스 제거
	if (DeleteService(hSrv)) {
		cerr << "서비스를 제거했습니다.\n";
	} else {
		cerr << "서비스를 제거하지 못했습니다.\n";
	}
	CloseServiceHandle(hSrv);
	CloseServiceHandle(hScm);
}