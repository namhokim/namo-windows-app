#include <windows.h>
#include <string>
#include <iostream>
using namespace std;
#include "Service.h"
#include "BackToTheFuture.h"
#include "ServiceConfig.h"

const int defaultDuration = 10;


void Usage()
{
	cout << "Usage) Service Install / Uninstall" << endl
		<< "   BackToTheFuture -install" << endl
		<< "   BackToTheFuture -uninstall" << endl;
}

//BackToTheFuture(atoi(argv[1]),defaultDuration);
int main(int argc, char* argv[])
{
	if(argc == 1) {
		SERVICE_TABLE_ENTRY serviceTableEntry[]={
			{L"BackToTheFuture",(LPSERVICE_MAIN_FUNCTION)BackToTheFutureMain},
			{NULL,NULL}
		};

		StartServiceCtrlDispatcher(serviceTableEntry);
	}
	else if(argc == 2) {
		if(_strcmpi(argv[1],"-install")==0) InstallService();
		else if(_strcmpi(argv[1],"-uninstall")==0) UnInstallService();
		else Usage();
	}
	else Usage();
	return 0;
}
