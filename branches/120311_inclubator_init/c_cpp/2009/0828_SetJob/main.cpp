#include <Windows.h>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

const DWORD BeginJob = 0;
const DWORD MaxJob = 999;
const DWORD PollingTime = 100;	// milisecond

bool JobToPaused(HANDLE hPrinter)
{
	assert(hPrinter != NULL);	// HANDLE must be accuired
	
	DWORD needed, returned;
	::EnumJobs(hPrinter, BeginJob, MaxJob, 1, NULL, 0, &needed, &returned);
	//LPBYTE pJob = new BYTE[needed];
	JOB_INFO_1 pJobInfo[MaxJob+1];

	if( 0 != ::EnumJobs(hPrinter, BeginJob, MaxJob, 1, (LPBYTE)pJobInfo, needed, &needed, &returned) ) {
		DWORD numsJ = returned;
		//JOB_INFO_1 *pJobInfo = (JOB_INFO_1 *)pJob;
		register DWORD i = 0;
		while(numsJ) {
			JOB_INFO_1 info = pJobInfo[i];
			if( (info.Status != JOB_CONTROL_PAUSE) /*&& (i != 0)*/) {
				::SetJob(hPrinter, info.JobId, 0, NULL, JOB_CONTROL_PAUSE);
			}
			--numsJ;
			i++;
		}
	}
	//delete []pJob;
	return true;
}

int main()
{
	// 프린터 핸들 열기
	HANDLE hPrt = NULL;
	if(0 == ::OpenPrinter(L"RICOH Aficio MP C4500 RPCS", &hPrt, NULL)) {
		return ::GetLastError();
	}
	
	while(1) {
		JobToPaused(hPrt);
		Sleep(PollingTime);
	}
	
	::CloseHandle(hPrt);
	return 0;
}