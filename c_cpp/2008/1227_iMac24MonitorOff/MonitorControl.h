#pragma once
#include <windows.h>

DWORD WINAPI tfMonitorCtrl(LPVOID lpParam);

class MonitorControl
{
public:
	MonitorControl(void);
	~MonitorControl(void);
	bool setMode(DWORD width, DWORD height, DWORD dwFlag=CDS_FULLSCREEN);
	const int IntervalTime;
private:
	DEVMODE deviceMode;
};
