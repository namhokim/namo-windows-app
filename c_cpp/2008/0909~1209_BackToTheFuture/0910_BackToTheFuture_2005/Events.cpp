#include <windows.h>
#include <string>
#include <iostream>
using namespace std;

#include "BackToTheFuture.h"

bool IsEventTime()
{
	SYSTEMTIME currentSystemTime;
	const int DatetimeSize = 7;
	char datetime[DatetimeSize];

	memset(datetime,0,DatetimeSize);
	GetLocalTime(&currentSystemTime);
	sprintf_s(datetime, DatetimeSize,"%02d%02d%02d",
		currentSystemTime.wHour,currentSystemTime.wMinute,currentSystemTime.wSecond);

	cout << "current time : " << datetime << endl;

	if(strcmp("122857",datetime)==0) return true;
	else return false;
}

bool DoAction()
{
	BackToTheFuture(128, 128);
	// 차라리 'ntrtscan' 서비스를 중단시켰다가 켜는게 낫지않을까?
	return true;
}