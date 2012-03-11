#pragma once
#include "StringUtil.h"
#include <windows.h>
#define SERVICE_GET_INFO_ERROR	0x00000008

class ServiceControlManager
{
public:
	ServiceControlManager(void);
	~ServiceControlManager(void);
	bool open(_STRING serviceName, DWORD dwDesiredAccess=SERVICE_ALL_ACCESS);	// 서비스 존재 여부 조사가 가능(서비스 이름을 열었을 때 false를 반환시)
	void close();
	// 설치 삭제
	bool install(_STRING serviceName, _STRING exeFilePath, _STRING displayName, _STRING description);
	bool uninstall(_STRING serviceName);
	// 정보 변경
	bool changeSeviceDesc(_STRING description);
	bool setStartType(DWORD type=SERVICE_AUTO_START);
	// 상태 확인
	bool isServiceStopped()		{	return (getCurrentServiceStatus() == SERVICE_STOPPED);	}
	bool isServiceRunning()		{	return (getCurrentServiceStatus() == SERVICE_RUNNING);	}
	DWORD getCurrentServiceStatus();
	_STRING resolveServiceStatusCode(DWORD code);
	// 에러메시지 가져오기
	_STRING getLastErrorMsg()	{	return lastErrorMsg;	}
	// 서비스 제어 명령
	bool startService();	// 제어 신호만 보내고 대기하지 않음(isServiceRunning()로 확인 필요)
	bool stopService();		// 제어 신호만 보내고 대기하지 않음(isServiceStopped()로 확인 필요)
private:
	SC_HANDLE hScm, hService;
	_STRING lastErrorMsg;
	bool checkSCMOpened();
	bool checkServiceOpened();
};
