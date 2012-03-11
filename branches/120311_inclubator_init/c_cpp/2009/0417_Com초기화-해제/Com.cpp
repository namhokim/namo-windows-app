// Com.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <Objbase.h>
#include <iostream>

using namespace std;

class ComObj
{
private:
	bool isNeedUnInit;
public:
	ComObj() : isNeedUnInit(false)
	{
		HRESULT res = ::CoInitialize(NULL);
		switch(res)
		{
		case S_OK:
			cout << "The COM library was initialized successfully on this thread.\n";
			isNeedUnInit = true;
			break;
		case S_FALSE:
			cout << "The COM library is already initialized on this thread.\n";
			break;
		case RPC_E_CHANGED_MODE:
			cout << "A previous call to CoInitializeEx specified the concurrency model for this thread as multithread apartment (MTA). This could also indicate that a change from neutral-threaded apartment to single-threaded apartment has occurred.\n";
			break;
		default:
			cout << "etc : ComObj()\n";
		}
	}
	~ComObj() {
		if(isNeedUnInit) CoUninitialize();
	}
};

void testComObj()
{
	ComObj co;
	ComObj co2;
}

int _tmain(int argc, _TCHAR* argv[])
{
	testComObj();
	return 0;
}

