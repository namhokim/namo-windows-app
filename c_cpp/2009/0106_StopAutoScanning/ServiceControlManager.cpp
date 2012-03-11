#include "ServiceControlManager.h"

ServiceControlManager::ServiceControlManager(void)
: hScm(NULL), hService(NULL)
{
	hScm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
}

ServiceControlManager::~ServiceControlManager(void)
{
	close();
	if(hScm != NULL) CloseServiceHandle(hScm);
}


bool ServiceControlManager::open(_STRING serviceName, DWORD dwDesiredAccess)
{
	if(!checkSCMOpened()) return false;

	if(hService != NULL) close();
	
	hService = OpenService(hScm,serviceName.c_str(),dwDesiredAccess);
	if(hService) return true;
	else return false;
}

void ServiceControlManager::close()
{
	if(hService != NULL) {
		CloseServiceHandle(hService);
		hService = NULL;
	}
}

bool ServiceControlManager::install(_STRING serviceName, _STRING exeFilePath, _STRING displayName, _STRING description)
{
	if(!checkSCMOpened()) return false;

	// �������� ���� üũ
	hService = CreateService(hScm,serviceName.c_str(),displayName.c_str(),
		SERVICE_PAUSE_CONTINUE | SERVICE_CHANGE_CONFIG,
		SERVICE_WIN32_OWN_PROCESS,SERVICE_DEMAND_START,SERVICE_ERROR_IGNORE,exeFilePath.c_str(),
		NULL,NULL,NULL,NULL,NULL);
	return changeSeviceDesc(description);
}

bool ServiceControlManager::uninstall(_STRING serviceName)
{
	if(open(serviceName)) {
		SERVICE_STATUS ss;
		QueryServiceStatus(hService, &ss);				// ���� ���� ���� Ȯ��
		while (ss.dwCurrentState != SERVICE_STOPPED) {	// �������̸� ������Ų��.
			ControlService(hService,SERVICE_CONTROL_STOP,&ss);
			Sleep(2000);
		}
		if(DeleteService(hService)) return true;		// ���� ����
	}
	return false;
}

bool ServiceControlManager::changeSeviceDesc(_STRING description)
{
	if(!checkServiceOpened()) return false;
	
	// ������ ���
	_TCHAR buffer[1024];
	wsprintf(buffer, _T("%s"), description.c_str());

	SERVICE_DESCRIPTION lpDes;
	lpDes.lpDescription = buffer;
	if(ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &lpDes) != 0) return true;
	else {
		namo::StringUtil su;
		lastErrorMsg = _T("Service desc�� ������ �� �����ϴ�.(");
		lastErrorMsg.append(su.int_to_string(GetLastError()));
		lastErrorMsg.append(_T(")"));
		return false;
	}
}

bool ServiceControlManager::setStartType(DWORD type)
{
	if(!checkServiceOpened()) return false;

	if(ChangeServiceConfig(hService, SERVICE_NO_CHANGE, type,
		SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL) != 0) return true;
	else {
		namo::StringUtil su;
		lastErrorMsg = _T("���������� ������ �� �����ϴ�.(");
		lastErrorMsg.append(su.int_to_string(GetLastError()));
		lastErrorMsg.append(_T(")"));
		return false;
	}

	return false;
}

DWORD ServiceControlManager::getCurrentServiceStatus()
{
	if(!checkServiceOpened()) return SERVICE_GET_INFO_ERROR;	// ServiceControlManager.h�� ����(WinSvc.h�� ����)

	SERVICE_STATUS ss;
	QueryServiceStatus(hService, &ss);
	return ss.dwCurrentState;
}

_STRING ServiceControlManager::resolveServiceStatusCode(DWORD code)
{
	switch(code)
	{
	case SERVICE_STOPPED:			return TEXT("������");
	case SERVICE_START_PENDING:		return TEXT("�����ϴ� ��");
	case SERVICE_STOP_PENDING:		return TEXT("�����ϴ� ��");
	case SERVICE_RUNNING:			return TEXT("���۵�");
	case SERVICE_CONTINUE_PENDING:	return TEXT("������ϴ� ��");
	case SERVICE_PAUSE_PENDING:		return TEXT("�Ͻ� �����ϴ� ��");
	case SERVICE_PAUSED:			return TEXT("�Ͻ� ������");
	case SERVICE_GET_INFO_ERROR:
	default:
									return TEXT("����");
	}
}


bool ServiceControlManager::startService()
{
	if(!checkServiceOpened()) return false;

	if(StartService(hService, 0, NULL) != 0) return true;
	else {
		_TCHAR errcode[15];
		wsprintf(errcode, TEXT("%d"), GetLastError());
		lastErrorMsg = errcode;
		return false;
	}
}

bool ServiceControlManager::stopService()
{
	if(!checkServiceOpened()) return false;

	SERVICE_STATUS ss;
	if(ControlService(hService, SERVICE_CONTROL_STOP, &ss) != 0) return true;
	else {
		_TCHAR errcode[15];
		wsprintf(errcode, TEXT("%d"), GetLastError());
		lastErrorMsg = errcode;
		return false;
	}
}


bool ServiceControlManager::checkSCMOpened()
{
	if(!hScm) {
		lastErrorMsg = _T("SCM�� ������ �ʾҽ��ϴ�.");
		return false;
	}
	else return true;
}

bool ServiceControlManager::checkServiceOpened()
{
	if(!hService) {
		lastErrorMsg = _T("Service�� ������ �ʾҽ��ϴ�.");
		return false;
	}
	else return true;
}