#pragma once
#include "StringUtil.h"
#include <windows.h>
#define SERVICE_GET_INFO_ERROR	0x00000008

class ServiceControlManager
{
public:
	ServiceControlManager(void);
	~ServiceControlManager(void);
	bool open(_STRING serviceName, DWORD dwDesiredAccess=SERVICE_ALL_ACCESS);	// ���� ���� ���� ���簡 ����(���� �̸��� ������ �� false�� ��ȯ��)
	void close();
	// ��ġ ����
	bool install(_STRING serviceName, _STRING exeFilePath, _STRING displayName, _STRING description);
	bool uninstall(_STRING serviceName);
	// ���� ����
	bool changeSeviceDesc(_STRING description);
	bool setStartType(DWORD type=SERVICE_AUTO_START);
	// ���� Ȯ��
	bool isServiceStopped()		{	return (getCurrentServiceStatus() == SERVICE_STOPPED);	}
	bool isServiceRunning()		{	return (getCurrentServiceStatus() == SERVICE_RUNNING);	}
	DWORD getCurrentServiceStatus();
	_STRING resolveServiceStatusCode(DWORD code);
	// �����޽��� ��������
	_STRING getLastErrorMsg()	{	return lastErrorMsg;	}
	// ���� ���� ���
	bool startService();	// ���� ��ȣ�� ������ ������� ����(isServiceRunning()�� Ȯ�� �ʿ�)
	bool stopService();		// ���� ��ȣ�� ������ ������� ����(isServiceStopped()�� Ȯ�� �ʿ�)
private:
	SC_HANDLE hScm, hService;
	_STRING lastErrorMsg;
	bool checkSCMOpened();
	bool checkServiceOpened();
};
