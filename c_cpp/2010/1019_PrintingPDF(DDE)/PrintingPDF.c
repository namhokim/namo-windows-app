#include "PrintingPDF.h"

#include <windows.h>
#include <ddeml.h>
#include <dde.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <shellapi.h>
#include <malloc.h>

#pragma warning(disable: 4996)

#define ACROBAT_EXE	"acrobat.exe"
#define ACROBAT		"acroview"
#define EVAL_TOPIC	"control"
#define RESULT_ITEM	"Result"
#define ERROR_ITEM	"Error"

/* Timeouts & delays */
#define CONNECT_DELAY			500			/* ms */
#define TRANSACTION_TIMEOUT		5000		/* ms */
#define MAX_INPUT_IDLE_WAIT     INFINITE	/* ms */


// Static Variations
static DWORD DDEsession;
static HCONV DDEconversation;
int outstanding_requests;

// Function Declare
int StartServer(LPCTSTR prog);
char *FindAcrobatModule();
void error(LPCTSTR fmt, ...);
void CloseConversation ();
int OpenConversation(LPCTSTR topic_name);
int __cdecl ExecuteCommand(LPCTSTR command, ...);
HDDEDATA CALLBACK Callback (UINT, UINT, HCONV, HSZ, HSZ, HDDEDATA, DWORD, DWORD);
HSZ DDEString(LPCTSTR str);
void FreeString(HSZ hsz);

// Function Defines
int PDFPrint(char *szFilename)
{
	int bRes = RETURN_SUCCESS;
	if(RETURN_FAILED==OpenConversation(EVAL_TOPIC)) return RETURN_FAILED;
	if(RETURN_FAILED==ExecuteCommand("[DocOpen(\"%s\")]", szFilename)) bRes = RETURN_FAILED;
	if(RETURN_FAILED==ExecuteCommand("[DocOpen(\"%s\")]", szFilename)) bRes = RETURN_FAILED;		/* acrord32 ver8 bug */
	if(RETURN_FAILED==ExecuteCommand("[FilePrintSilent(\"%s\")]", szFilename)) bRes = RETURN_FAILED;
	if(RETURN_FAILED==ExecuteCommand("[DocClose(\"%s\")]", szFilename)) bRes = RETURN_FAILED;
	if(RETURN_FAILED==ExecuteCommand("[AppExit]", szFilename)) bRes = RETURN_FAILED;

	return bRes;
}

int StartServer(LPCTSTR prog)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	/* To be sure ... */
	if (! prog) return TRUE;

	ZeroMemory (&si, sizeof (si));
	si.cb = sizeof (si);
	if (!CreateProcess (NULL, (LPTSTR)prog, NULL, NULL, FALSE, 0,
		NULL, NULL, &si, &pi))
	{
		error("Could not start process %s", prog);
		return RETURN_FAILED;
	}

	/* Wait for the process to enter an idle state */
	WaitForInputIdle (pi.hProcess, MAX_INPUT_IDLE_WAIT);

	/* Close the handles */
	CloseHandle (pi.hThread);
	CloseHandle (pi.hProcess);
	return RETURN_SUCCESS;
}

char *FindAcrobatModule()
{
	char buf[MAX_PATH];
	char path[MAX_PATH];
	char *program;
	HANDLE hFile;
	HINSTANCE hInst;

	program = (char *)malloc(MAX_PATH*sizeof(char));
	if (program == NULL) goto exit_1;

	buf[0] = path[0] = program[0] = '\0';

	if (GetTempPath(sizeof(path), path) != 0) {
		strcpy(buf, path);
	}


	strcat(buf, "dummy.pdf");

	hFile = CreateFile(buf, 
		GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL,
		OPEN_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	CloseHandle(hFile);

	hInst = FindExecutable("dummy.pdf", path, program);

	DeleteFile(buf);

#if 0
	switch ((int)hInst) {
	case SE_ERR_FNF:
		fprintf(stdout, "File not found: %s\n", argv[1]);
		break;
	case SE_ERR_NOASSOC:
		fprintf(stdout, "No association for %s\n", argv[1]);
		break;
	default:
		fprintf(stdout, "File .pdf is opened by %s\n", program);
		break;
	}
#endif

	if (*program == '\0' || (GetFileAttributes(program) == INVALID_FILE_ATTRIBUTES)) {
		program = NULL;
	}

exit_1:
	return program;
}

void error(LPCTSTR fmt, ...)
{
	/*
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputc('\n', stderr);
	*/
	/* Tidy up */
	CloseConversation();
	/*
	exit(1);
	*/
}

void CloseConversation ()
{
	if (DDEconversation) DdeDisconnect(DDEconversation);
	if (DDEsession) DdeUninitialize(DDEsession);
}

int OpenConversation(LPCTSTR topic_name)
{
	UINT ret;
	HSZ service;
	HSZ topic;
	int n;

	ret = DdeInitialize(&DDEsession, Callback, APPCLASS_STANDARD| APPCMD_CLIENTONLY, 0);
	if (ret != DMLERR_NO_ERROR) {
		error("Cannot start DDE");
		return RETURN_FAILED;
	}

	service = DDEString(ACROBAT);
	topic = DDEString(topic_name);
	DDEconversation = DdeConnect(DDEsession, service, topic, 0);

	if (DDEconversation == 0)
	{
		if (StartServer(FindAcrobatModule()) == 0)
		{
			/* Try to connect */
			for (n = 0; n < 5; n++)
			{
				Sleep (CONNECT_DELAY);
				DDEconversation = DdeConnect(DDEsession, service, topic, 0);
				if (DDEconversation) break;
			}
			if (DDEconversation == 0) {
				error("Cannot contact server %s", ACROBAT);
				return RETURN_FAILED;
			}
		}
	}
	FreeString(service);
	FreeString(topic);

	return RETURN_SUCCESS;
}

int __cdecl ExecuteCommand(LPCTSTR command, ...)
{
	HDDEDATA ret;
	va_list args;
	int len;
	char *buffer;

	va_start( args, command );
	len = _vscprintf(command, args) // _vscprintf doesn't count
		+ 1;			// terminating '\0'
	buffer = (char *)malloc( len * sizeof(char) );
	vsprintf( buffer, command, args );

	ret = DdeClientTransaction((LPBYTE)buffer, strlen(buffer) + 1,
		DDEconversation, 0, 0, XTYP_EXECUTE,
		TRANSACTION_TIMEOUT, 0);
	if (ret == 0)
	{
		UINT DDEErr = DdeGetLastError(DDEsession);
		error("Cannot execute command \"%s\" (error %ld)", command, DDEErr);
		return RETURN_FAILED;
	} // if

	return RETURN_SUCCESS;
}

HDDEDATA CALLBACK Callback (UINT uType, UINT uFmt, HCONV hconv, HSZ hsz1, HSZ hsz2,
							HDDEDATA hdata, DWORD dwData1, DWORD dwData2)
{
	if (uType == XTYP_ADVDATA) {
		DWORD len = DdeGetData(hdata, NULL, 0, 0);
		char *buf = (char *)_alloca(len + 1);
		DdeGetData(hdata, (LPBYTE)buf, len + 1, 0);

		if (--outstanding_requests == 0)
			PostQuitMessage(0);

		return (HDDEDATA) DDE_FACK;
	}

	return (HDDEDATA) NULL;
}

HSZ DDEString(LPCTSTR str)
{
	HSZ hsz = DdeCreateStringHandle(DDEsession, str, CP_WINANSI);
	if (hsz == 0)
		error("Cannot create string for %s", str);
	return hsz;
}

void FreeString(HSZ hsz)
{
	DdeFreeStringHandle(DDEsession, hsz);
}
