#include <windows.h>
#include <cstdio>
#include <clocale>
#include <string>
#include <iostream>
#include "..\..\..\..\work\WinApps\commonLib\Util\GetLastError.h"
#pragma warning(disable : 4996)

const DWORD BeginJob = 0;
const DWORD MaxJob = 999;
TCHAR prtName[] = TEXT("uPrint MP 5000B RPCS");
#define	INVALID_JOB_ID	-1

using namespace std;

void EnumPrt();
wstring GetResolve(DWORD code);
string GetLogFilename();
void SetPrt();

int main()
{
	//  ������ : �ѱ���
	::setlocale(LC_ALL, "Korean");

	//::freopen(GetLogFilename().c_str(), "a+", stdout);
	//EnumPrt();
	//::fclose(stdout);
	SetPrt();
	return 0;
}

void EnumPrt()
{
	HANDLE hPrinter;
	DWORD dwCode = ::OpenPrinter(prtName, &hPrinter, NULL);

	DWORD needed, returned;
	BOOL fnRes = ::EnumJobs(hPrinter, BeginJob, MaxJob, 1, NULL, 0, &needed, &returned);
	LPBYTE pJob = new BYTE[needed];
	DWORD ret = INVALID_JOB_ID;

	fnRes = ::EnumJobs(hPrinter, BeginJob, MaxJob, 1, (LPBYTE)pJob, needed, &needed, &returned);
	if( 0 == fnRes ) printf("enum �Ұ�\n");

	JOB_INFO_1 *pJobInfo = (JOB_INFO_1 *)pJob;
	if( returned > 0 ) {
		register DWORD i = 0;
		while ( i < returned ) {
			JOB_INFO_1 info = pJobInfo[i];
			wprintf(L"%d %s\n%s\n", info.JobId, info.pDocument,
				GetResolve(info.Status).c_str());
			++i;
		}
	}
	else {
		printf("������ �۾��� �����ϴ�.\n");
	}
}

wstring GetResolve(DWORD c)
{
	wstring r;

	if(c & JOB_STATUS_PAUSED)	r.append(L" JOB_STATUS_PAUSED\n");
	if(c & JOB_STATUS_ERROR)	r.append(L" JOB_STATUS_ERROR\n");
	if(c & JOB_STATUS_DELETING)	r.append(L" JOB_STATUS_DELETING\n");
	if(c & JOB_STATUS_SPOOLING)	r.append(L" JOB_STATUS_SPOOLING\n");
	if(c & JOB_STATUS_PRINTING)	r.append(L" JOB_STATUS_PRINTING\n");
	if(c & JOB_STATUS_OFFLINE)	r.append(L" JOB_STATUS_OFFLINE\n");
	if(c & JOB_STATUS_PAPEROUT)	r.append(L" JOB_STATUS_PAPEROUT\n");
	if(c & JOB_STATUS_PRINTED)	r.append(L" JOB_STATUS_PRINTED\n");
	if(c & JOB_STATUS_DELETED)	r.append(L" JOB_STATUS_DELETED\n");
	if(c & JOB_STATUS_BLOCKED_DEVQ)	r.append(L" JOB_STATUS_BLOCKED_DEVQ\n");
	if(c & JOB_STATUS_USER_INTERVENTION)	r.append(L" JOB_STATUS_USER_INTERVENTION\n");
	if(c & JOB_STATUS_RESTART)	r.append(L" JOB_STATUS_RESTART\n");
	if(c & JOB_STATUS_COMPLETE)	r.append(L" JOB_STATUS_COMPLETE\n");
	if(c & JOB_STATUS_RETAINED)	r.append(L" JOB_STATUS_RETAINED\n");
	if(c & JOB_STATUS_RENDERING_LOCALLY)	r.append(L" JOB_STATUS_RENDERING_LOCALLY\n");
	if(r.empty()) r.assign(L" NO_STATUS");
	return r;
}

string GetLogFilename()
{
	// ���� ��¥
	SYSTEMTIME st;
	::GetLocalTime(&st);

	// �����̸� �ۼ�
	char filename[MAX_PATH];
	wsprintfA(filename, "prtEnum_%4d%02d%02d_%02d%02d%02d.txt",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	return filename;
}

void SetPrt()
{
	HANDLE hPrinter;
	DWORD dwCode = ::OpenPrinter(prtName, &hPrinter, NULL);
	if( dwCode != 0 ) {
		// �Ҵ� ũ�� ����
		DWORD dwNeeded = 0, dwAllocated = 0;
		DWORD level = 2;
		::GetPrinter(hPrinter, level, NULL, NULL, &dwNeeded);

		// �޸� �Ҵ�
		LPBYTE pPrt = new BYTE[dwNeeded];
		if( NULL == pPrt) {
			::ClosePrinter(hPrinter);
			return;
		}
		else {
			dwAllocated = dwNeeded;
			dwNeeded = 0;
		}

		// �Ҵ�
		::GetPrinter(hPrinter, level, pPrt, dwAllocated, &dwNeeded);
		PRINTER_INFO_2 *pPrtInfo = (PRINTER_INFO_2 *)pPrt;
		PRINTER_INFO_2 info = pPrtInfo[0];

		// ����
		::SetPrinter(hPrinter, 0, NULL, PRINTER_CONTROL_PAUSE);
		cout << "Status : " << info.Status << endl;
		delete [] pPrt;

		// ������ �ݱ�
		::ClosePrinter(hPrinter);
	}
	else {
		cout << Error::GetLastErrorA() << "code :" << ::GetLastError() << endl;
	}
	return;
}
