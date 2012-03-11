// main.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "main.h"

#define MAX_LOADSTRING 100

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.
_STRING registryKey;
_STRING clientID;
_STRING clientPWD;
_STRING prnPath;
CAloFileMonitor monitor;
HWND hMain;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
BOOL				InitInstance(HINSTANCE);
BOOL				InitDialogMain(HWND);
INT_PTR CALLBACK	WndDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	InputClientID(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	if(LoadString(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) return FALSE;
	if(LoadString(hInst, IDC_DOKSURI, szWindowClass, MAX_LOADSTRING) == 0) return FALSE;
	registryKey = _T("Software\\");		registryKey.append(szWindowClass);		registryKey.append(_T("\\POD"));

	// ���α׷� �ߺ��� üũ�մϴ�.
	_STRING instanceName=szTitle;
	instanceName.append(szWindowClass);

	SingleInstance si(instanceName.c_str());
	if(si.IsExist()) {
		// ���α׷��� ã�� ����
		HWND hApp = ::FindWindow(NULL, szTitle);
		if(hApp) {
			::ShowWindow(hApp, SW_SHOWNORMAL);
			::ShowWindow(hApp, SW_RESTORE);
		//	::SetFocus(hApp);
			::SetForegroundWindow(hApp);
		//	::SetActiveWindow(hApp); 
		}
		return FALSE;
	}

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance (hInstance))
	{
		return FALSE;
	}
	return TRUE;
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   if(DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), HWND_DESKTOP, WndDlgProc) == 0) return FALSE;
   return TRUE;
}

//
//  �Լ�: InitDialogMain(HWND)
//
//  ����: �� â�� ������� �����մϴ�.
//
BOOL InitDialogMain(HWND hDlgWnd)
{
	hMain = hDlgWnd;

	// ������ Ÿ��Ʋ ����
	if(SetWindowText(hDlgWnd, szTitle) == 0) return FALSE;
	// ������ ����
	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DOKSURI));	// icon �����´�.
	if(hIcon == NULL) return FALSE;
	SendMessage(hDlgWnd, WM_SETICON, TRUE, (LPARAM)hIcon);				// icon ����

	// ������ ��ġ ����
	return SetWindowPosition(hDlgWnd);
}

BOOL checkApplicationCondition(HWND hWnd)
{
	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	RegistryManager rMan;
	rMan.open(registryKey,KEY_READ | KEY_WRITE);
	// ��ID
	clientID = rMan.readByString(_T("clientID"), _T("no exist"));
	if(clientID.compare(_T("no exist")) == 0) {	// ���� ��� �ȵǾ� ������
		int res = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_INPUT_ID), hWnd, InputClientID);
		if(res == IDOK) {
			rMan.writeByString(_T("clientID"), clientID);
			rMan.writeByString(_T("clientPWD"), clientPWD);
		}
		else return FALSE;
	}
	else clientPWD = rMan.readByString(_T("clientPWD"), _T("no exist"));
	// ������� PJL ����
	prnPath  = rMan.readByString(_T("prnPath"), _T("no exist"));
	if(prnPath.compare(_T("no exist")) == 0) {	// ���� ��� �ȵǾ� ������
		FileOpenDialogBox fileOpenDlg;
		fileOpenDlg.setWindowHandle(hWnd);
		prnPath = fileOpenDlg.showAndGetFilename();
		if(prnPath.empty()) return FALSE;
		else rMan.writeByString(_T("prnPath"), prnPath);
	}
	return TRUE;
}

void refreshPJLInfo(stringKV pjlMap)
{
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_TIME),				pjlMap[_T("TIME")].substr(1,pjlMap[_T("TIME")].size()-2).c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_DATE),				pjlMap[_T("DATE")].substr(1,pjlMap[_T("DATE")].size()-2).c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_FRONTCOVERPRINT),	pjlMap[_T("FRONTCOVERPRINT")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_BACKCOVERPRINT),	pjlMap[_T("BACKCOVERPRINT")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_SLIPSHEETPRINT),	pjlMap[_T("SLIPSHEETPRINT")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_CHAPTERPRINT),		pjlMap[_T("CHAPTERPRINT")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_DATAMODE),			pjlMap[_T("DATAMODE")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_APLPAGELAYOUT),	pjlMap[_T("APLPAGELAYOUT")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_DUPLEX),			pjlMap[_T("DUPLEX")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_RESOLUTION),		pjlMap[_T("RESOLUTION")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_EDGETOEDGE),		pjlMap[_T("EDGETOEDGE")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_STAPLE),			pjlMap[_T("STAPLE")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_PUNCH),			pjlMap[_T("PUNCH")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_PAPER),			pjlMap[_T("PAPER")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_COPIES),			pjlMap[_T("COPIES")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_QTY),				pjlMap[_T("QTY")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_AUTOTRAYCHANGE),	pjlMap[_T("AUTOTRAYCHANGE")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_LANGUAGE),			pjlMap[_T("LANGUAGE")].c_str());
	SetWindowText(GetDlgItem(hMain, IDC_EDIT_PJL_JOBNAME),			pjlMap[_T("JOBNAME")].substr(1,pjlMap[_T("JOBNAME")].size()-2).c_str());
	
	ShowWindow(hMain, SW_SHOWNORMAL);
	ShowWindow(hMain, SW_RESTORE);
	SetForegroundWindow(hMain);
}

void FileChanged()
{
	Sleep(1000);
//	MessageBox(NULL, _T("���Ϻ���"), _T("�˸�"), MB_OK);
	namo::StringUtil su;
	// PJL������ �����ͼ� ȭ�鿡 �ѷ��ش�.

	PJLReader reader;
	if(reader.open(prnPath)) {
		reader.fillPJLinfo();
		refreshPJLInfo(reader.getKVMap());
	}
}

_STRING GetTextEditString(HWND hWnd, int ID)
{
	const int BufferSize = 1024;
	_TCHAR buffer[BufferSize];
	GetWindowText(GetDlgItem(hWnd, ID), buffer, BufferSize);
	return buffer;
}
#include <commctrl.h>
_STRING GetDateAndTime(HWND hWnd)
{
	_STRING resDateAndTime(_T(""));
	_TCHAR datetime[256];
	SYSTEMTIME st;
	memset(&st, 0, sizeof(SYSTEMTIME));

	if(DateTime_GetSystemtime(GetDlgItem(hWnd, IDC_DATEPICKER), &st) == GDT_VALID) {
		wsprintf(datetime, _T("%04d-%02d-%02d"), st.wYear, st.wMonth, st.wDay);
		resDateAndTime.append(datetime);
	}
	if(DateTime_GetSystemtime(GetDlgItem(hWnd, IDC_TIMEPICKER), &st) == GDT_VALID) {
		wsprintf(datetime, _T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
		resDateAndTime.append(_T(" "));
		resDateAndTime.append(datetime);
	}
	return resDateAndTime;
}

_STRING GetINFContent(HWND hWnd)
{
	_STRING retString;
	retString.append(_T("USER_ID="));
	retString.append(clientID);
	retString.append(_T(";COLOR_MODE="));
	retString.append(GetTextEditString(hWnd, IDC_EDIT_PJL_DATAMODE));
	retString.append(_T(";PAPER_SIZE="));
	retString.append(GetTextEditString(hWnd, IDC_EDIT_PJL_PAPER));
	retString.append(_T(";COPIES="));
	retString.append(GetTextEditString(hWnd, IDC_EDIT_PJL_COPIES));
	retString.append(_T(";PRINT_PAGES="));
	retString.append(GetTextEditString(hWnd, IDC_EDIT_PJL_QTY));
	retString.append(_T(";ORG_SIDE_NAME="));
	retString.append(GetTextEditString(hWnd, IDC_EDIT_PJL_DUPLEX));
	retString.append(_T(";COVER="));
	retString.append(GetTextEditString(hWnd, IDC_EDIT_PJL_FRONTCOVERPRINT));
	retString.append(_T(";BOOKBINDING="));
	retString.append(GetTextEditString(hWnd, IDC_EDIT_PJL_STAPLE));
	retString.append(_T(";SLIP_SHEET="));
	retString.append(GetTextEditString(hWnd, IDC_EDIT_PJL_SLIPSHEETPRINT));
	retString.append(_T(";DUE_DATE="));
	retString.append(GetDateAndTime(hWnd));
	retString.append(_T(";DOCUMENT_NAME="));
	retString.append(GetTextEditString(hWnd, IDC_EDIT_PJL_JOBNAME));
	retString.append(_T(";PAGE_ROTATE="));
	retString.append(GetTextEditString(hWnd, IDC_EDIT_PJL_APLPAGELAYOUT));	
	return retString;
}
void FileProcessing(HWND hWnd)
{
	// �̸� ����
	FileManager fm;
	_STRING filename = fm.getFilenameByTime(_T(""), clientID);
	_STRING filenamePJL = filename;
	filenamePJL.append(_T(".prn"));
	_STRING filenameINF = filename;
	filenameINF.append(_T(".inf"));

	// pjl ���� ����
	fm.copyFile(prnPath, filenamePJL);
	// inf ���� �ۼ�
	fm.saveToFile(filenameINF, GetINFContent(hWnd));

	// FTP ����
	FTPRequest ftp;
	ftp.open(_T("10.15.1.102"),_T("ftpuser"), _T("podftpuser"));
	if(ftp.put(filenamePJL) && ftp.put(filenameINF)) {
		MessageBox(hWnd, _T("���������� ����"), _T("�˸�"), MB_OK);
	}
	else MessageBox(hWnd, _T("���������� ����"), _T("�˸�"), MB_OK);
	ftp.close();

	// ���ϻ���
	fm.removeFile(filenamePJL);
	//fm.removeFile(filenameINF);
}

HANDLE hEvent = NULL;
DWORD WINAPI tnAnimateProgress(LPVOID lpParam)
{
	HWND hwndPB = (HWND)lpParam;

	ShowWindow(hwndPB, SW_SHOW);
	SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, 65535));
	SendMessage(hwndPB,  PBM_SETPOS, 0, 0);
	while(WaitForSingleObject(hEvent, 0) ==WAIT_TIMEOUT) {
		SendMessage(hwndPB, PBM_STEPIT, 0, 0);
		Sleep(100);
	}
	ShowWindow(hwndPB, SW_HIDE);
	return 0;
}

DWORD WINAPI tnFileProcessing(LPVOID lpParam)
{
	HANDLE hThread;
	DWORD dwThreadId;

	HWND hDlg = (HWND)lpParam;
	EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
	HWND hwndPB = GetDlgItem(hDlg, IDC_PROGRESS_TRANSFER);
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	
	ResetEvent(hEvent);
	hThread = CreateThread(NULL,0,tnAnimateProgress,hwndPB,0,&dwThreadId);
	FileProcessing(hDlg);
	SetEvent(hEvent);
	CloseHandle(hEvent);
	EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
	
	return 0;
}

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����: �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//	WM_SIZE		- �� ���α׷� â ũ�� ��ȭ
//
//
INT_PTR CALLBACK WndDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HANDLE hThread;
	DWORD dwThreadId;
	int wmId, wmEvent;

	switch (message)
	{
	case WM_INITDIALOG:
		InitDialogMain(hWnd);
		if(!checkApplicationCondition(hWnd)) EndDialog(hWnd,0);
		monitor.SetFileMonitor(prnPath, FileChanged);
		monitor.Start();
		break;
	case WM_SIZE:
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴��� ���� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDOK:
			// ������ �Ѵ�.
			hThread = CreateThread(NULL,0,tnFileProcessing,hWnd,0,&dwThreadId);
			break;
		case IDCANCEL:
			ShowWindow(hWnd, SW_MINIMIZE);
			//ShowWindow(hWnd, SW_HIDE);
			break;
		case IDM_EXIT:	// �޴�-����
			monitor.Stop();
			EndDialog(hWnd,0);
			break;
		}
		break;
	case WM_CLOSE:		// X��ư, Alt+F4
		monitor.Stop();
		EndDialog(hWnd,0);
	case WM_DESTROY:
		break;
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

bool FormValid(HWND hDlg, _STRING id, _STRING pwd)
{
	UserInfoValidator vt;
	int resCode = vt.isValid(id, pwd);
	if(resCode == VALID_PASSED) return true;
	else {
		MessageBox(hDlg, vt.resolveMsg(resCode).c_str(), _T("�˸�"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}
}

DWORD WINAPI thFormValid(LPVOID lpParam)
{
	HWND hDlg = (HWND)lpParam;
	EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);

	_TCHAR textBufferID[1024], textBufferPWD[1024];
	GetWindowText(GetDlgItem(hDlg, IDC_EDIT_CLIENT_ID),textBufferID,1024);
	GetWindowText(GetDlgItem(hDlg, IDC_EDIT_CLIENT_PWD),textBufferPWD,1024);

	clientID = textBufferID;
	clientPWD = textBufferPWD;
	if(clientID.empty() || clientPWD.empty()) {
		MessageBox(hDlg, _T("������ �Է��ϼ���"), _T("�˸�"), MB_OK);
	}
	else {
		if(FormValid(hDlg, textBufferID, textBufferPWD)) EndDialog(hDlg, IDOK);
	}
	EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
	return 0;
}
// ������ �Է� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK InputClientID(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HANDLE hThread;
	DWORD dwThreadId;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetFocus(GetDlgItem(hDlg, IDC_EDIT_CLIENT_ID));
		return (INT_PTR)FALSE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			hThread = CreateThread(NULL,0,thFormValid,hDlg,0,&dwThreadId);
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
