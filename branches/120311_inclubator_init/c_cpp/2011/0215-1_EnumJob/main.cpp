#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

void PrintEachJobs();
void DisplayJob2(const LPJOB_INFO_2 pJob2);

int main()
{
	PrintEachJobs();
	return 0;
}

void PrintEachJobs()
{
	HANDLE hPrinter;
	BOOL bRes;

	bRes = ::OpenPrinter(TEXT("KONICA MINOLTA C360SeriesPCL"), &hPrinter, NULL);
	if(bRes) {
		DWORD firstJob, noJobs, level, cbBuf, cbNeeded, cReturned;
		LPBYTE pJob;

		firstJob = 0;
		noJobs = INFINITE;	// no limit (warning : much of print jobs)
		level = 2;
		pJob = NULL;
		cbBuf = 0;

		bRes = ::EnumJobs(hPrinter, firstJob, noJobs, level, pJob, cbBuf, &cbNeeded, &cReturned);
		if( (!bRes) && (ERROR_INSUFFICIENT_BUFFER==::GetLastError()) && (cbNeeded>0) ) {
			HANDLE hHeap = ::GetProcessHeap();
			DWORD dwFlags = HEAP_NO_SERIALIZE;

			pJob = (LPBYTE)::HeapAlloc(hHeap, dwFlags, cbNeeded);
			if(NULL==pJob) return;
			cbBuf = cbNeeded;

			bRes = ::EnumJobs(hPrinter, firstJob, noJobs, level, pJob, cbBuf, &cbNeeded, &cReturned);
			if(bRes) {
				LPJOB_INFO_2 pJob2 = (LPJOB_INFO_2)&pJob[0];

				DWORD i;
				for(i=0; i<cReturned; ++i) {
					DisplayJob2(pJob2);
					++pJob2;
				}
			}
			::HeapFree(hHeap, dwFlags, (LPVOID)pJob);
		}
	}
	
}

void DisplayJob2(const LPJOB_INFO_2 pJob2)
{
	if(NULL==pJob2) return;

	// Job Info
	printf("JobId : %d\n", pJob2->JobId);
	_tprintf(TEXT("PrinterName : %s\n"), pJob2->pPrinterName);
	_tprintf(TEXT("MachineName : %s\n"), pJob2->pMachineName);
	_tprintf(TEXT("UserName : %s\n"), pJob2->pUserName);
	_tprintf(TEXT("Document : %s\n"), pJob2->pDocument);
	_tprintf(TEXT("NotifyName : %s\n"), pJob2->pNotifyName);
	_tprintf(TEXT("Datatype : %s\n"), pJob2->pDatatype);
	_tprintf(TEXT("PrintProcessor : %s\n"), pJob2->pPrintProcessor);
	_tprintf(TEXT("Parameters : %s\n"), pJob2->pParameters);
	_tprintf(TEXT("DriverName : %s\n"), pJob2->pDriverName);

	_tprintf(TEXT("Status : %d (%s)\n"), pJob2->Status, pJob2->pStatus);

	printf("Priority : %d\n", pJob2->Priority);
	printf("Position : %d\n", pJob2->Position);
	printf("StartTime : %d\n", pJob2->StartTime);
	printf("UntilTime : %d\n", pJob2->UntilTime);
	printf("Size : %d\n", pJob2->Size);
	printf("PagesPrinted/TotalPages : %d/%d\n", pJob2->PagesPrinted, pJob2->TotalPages);

	// Device Mode
	PDEVMODE pDevMode = pJob2->pDevMode;
	_tprintf(TEXT("DevMode.DeviceName : %s\n"), pDevMode->dmDeviceName);
	printf("DevMode.SpecVersion : %d\n", pDevMode->dmSpecVersion);
	printf("DevMode.DriverVersion : %d\n", pDevMode->dmDriverVersion);
	printf("DevMode.Size : %d\n", pDevMode->dmSize);
	printf("DevMode.DriverExtra : %d\n", pDevMode->dmDriverExtra);
	printf("DevMode.Fields : %d\n", pDevMode->dmFields);

	printf("DevMode.Orientation : %d\n", pDevMode->dmOrientation);
	printf("DevMode.PaperSize : %d\n", pDevMode->dmPaperSize);
	printf("DevMode.PaperLength : %d\n", pDevMode->dmPaperLength);
	printf("DevMode.PaperWidth : %d\n", pDevMode->dmPaperWidth);
	printf("DevMode.Scale : %d\n", pDevMode->dmScale);
	printf("DevMode.Copies : %d\n", pDevMode->dmCopies);
	printf("DevMode.DefaultSource : %d\n", pDevMode->dmDefaultSource);
	printf("DevMode.PrintQuality : %d\n", pDevMode->dmPrintQuality);

	printf("DevMode.Color : %d\n", pDevMode->dmColor);
	printf("DevMode.Duplex : %d\n", pDevMode->dmDuplex);
	printf("DevMode.YResolution : %d\n", pDevMode->dmYResolution);
	printf("DevMode.TTOption : %d\n", pDevMode->dmTTOption);
	printf("DevMode.Collate : %d\n", pDevMode->dmCollate);
	_tprintf(TEXT("DevMode.FormName : %s\n"), pDevMode->dmFormName);
	printf("DevMode.LogPixels : %d\n", pDevMode->dmLogPixels);
	printf("DevMode.BitsPerPel : %d\n", pDevMode->dmBitsPerPel);
	printf("DevMode.PelsWidth : %d\n", pDevMode->dmPelsWidth);
	printf("DevMode.PelsHeight : %d\n", pDevMode->dmPelsHeight);

	printf("\n\n");
}