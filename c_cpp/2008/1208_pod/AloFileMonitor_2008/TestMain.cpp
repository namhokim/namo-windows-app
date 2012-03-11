#include "stdio.h"
#include <iostream>
#include "windows.h"

#include "AloFileMonitor.h"

int gChanged;
const _STRING filename(_T("C:\\temp\\test.prn"));

/* @brief this function will be changed when the file's last-write time is changed */
void FileChanged()
{
	++gChanged;

	std::cout<<"file changed - "<< gChanged << std::endl;
}


void main()
{
	gChanged = 0;
	CAloFileMonitor monitor;

	// set the file
	monitor.SetFileMonitor(filename, FileChanged);

	// start the monitor
	bool startResult = monitor.Start();

	// 3번의 변경을 체크
	while( startResult/* && gChanged < 10*/)
	{		
		::Sleep(100);
	}

	if(gChanged >= 3) std::cout<< ("after 10 times changed, stop the monitor\n");
	bool stopResult = monitor.Stop();

	std::cout<<"Test program has been finished\n";
}
