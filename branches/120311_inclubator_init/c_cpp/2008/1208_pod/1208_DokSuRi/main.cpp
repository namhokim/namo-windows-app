// main.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "main.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.
_STRING registryKey;
_STRING clientID;
_STRING clientPWD;
_STRING prnPath;
CAloFileMonitor monitor;
HWND hMain;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
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

	// 전역 문자열을 초기화합니다.
	if(LoadString(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING) == 0) return FALSE;
	if(LoadString(hInst, IDC_DOKSURI, szWindowClass, MAX_LOADSTRING) == 0) return FALSE;
	registryKey = _T("Software\\");		registryKey.append(szWindowClass);		registryKey.append(_T("\\POD"));

	// 프로그램 중복을 체크합니다.
	_STRING instanceName=szTitle;
	instanceName.append(szWindowClass);

	SingleInstance si(instanceName.c_str());
	if(si.IsExist()) {
		// 프로그램을 찾고 종료
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

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance))
	{
		return FALSE;
	}
	return TRUE;
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   if(DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), HWND_DESKTOP, WndDlgProc) == 0) return FALSE;
   return TRUE;
}

//
//  함수: InitDialogMain(HWND)
//
//  목적: 주 창의 룩앤필을 설정합니다.
//
BOOL InitDialogMain(HWND hDlgWnd)
{
	hMain = hDlgWnd;

	// 윈도우 타이틀 설정
	if(SetWindowText(hDlgWnd, szTitle) == 0) return FALSE;
	// 아이콘 설정
	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DOKSURI));	// icon 가져온다.
	if(hIcon == NULL) return FALSE;
	SendMessage(hDlgWnd, WM_SETICON, TRUE, (LPARAM)hIcon);				// icon 부착

	// 윈도우 위치 설정
	return SetWindowPosition(hDlgWnd);
}

BOOL checkApplicationCondition(HWND hWnd)
{
	// 전역 문자열을 초기화합니다.
	RegistryManager rMan;
	rMan.open(registryKey,KEY_READ | KEY_WRITE);
	// 고객ID
	clientID = rMan.readByString(_T("clientID"), _T("no exist"));
	if(clientID.compare(_T("no exist")) == 0) {	// 값이 기록 안되어 있으면
		int res = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG_INPUT_ID), hWnd, InputClientID);
		if(res == IDOK) {
			rMan.writeByString(_T("clientID"), clientID);
			rMan.writeByString(_T("clientPWD"), clientPWD);
		}
		else return FALSE;
	}
	else clientPWD = rMan.readByString(_T("clientPWD"), _T("no exist"));
	// 모니터할 PJL 파일
	prnPath  = rMan.readByString(_T("prnPath"), _T("no exist"));
	if(prnPath.compare(_T("no exist")) == 0) {	// 값이 기록 안되어 있으면
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
//	MessageBox(NULL, _T("파일변경"), _T("알림"), MB_OK);
	namo::StringUtil su;
	// PJL정보를 가져와서 화면에 뿌려준다.

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
	// 이름 결정
	FileManager fm;
	_STRING filename = fm.getFilenameByTime(_T(""), clientID);
	_STRING filenamePJL = filename;
	filenamePJL.append(_T(".prn"));
	_STRING filenameINF = filename;
	filenameINF.append(_T(".inf"));

	// pjl 파일 복사
	fm.copyFile(prnPath, filenamePJL);
	// inf 파일 작성
	fm.saveToFile(filenameINF, GetINFContent(hWnd));

	// FTP 전송
	FTPRequest ftp;
	ftp.open(_T("10.15.1.102"),_T("ftpuser"), _T("podftpuser"));
	if(ftp.put(filenamePJL) && ftp.put(filenameINF)) {
		MessageBox(hWnd, _T("성공적으로 전송"), _T("알림"), MB_OK);
	}
	else MessageBox(hWnd, _T("성공적으로 전송"), _T("알림"), MB_OK);
	ftp.close();

	// 파일삭제
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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//	WM_SIZE		- 주 프로그램 창 크기 변화
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
		// 메뉴의 선택 영역을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDOK:
			// 전송을 한다.
			hThread = CreateThread(NULL,0,tnFileProcessing,hWnd,0,&dwThreadId);
			break;
		case IDCANCEL:
			ShowWindow(hWnd, SW_MINIMIZE);
			//ShowWindow(hWnd, SW_HIDE);
			break;
		case IDM_EXIT:	// 메뉴-종료
			monitor.Stop();
			EndDialog(hWnd,0);
			break;
		}
		break;
	case WM_CLOSE:		// X버튼, Alt+F4
		monitor.Stop();
		EndDialog(hWnd,0);
	case WM_DESTROY:
		break;
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
		MessageBox(hDlg, vt.resolveMsg(resCode).c_str(), _T("알림"), MB_OK | MB_ICONEXCLAMATION);
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
		MessageBox(hDlg, _T("내용을 입력하세요"), _T("알림"), MB_OK);
	}
	else {
		if(FormValid(hDlg, textBufferID, textBufferPWD)) EndDialog(hDlg, IDOK);
	}
	EnableWindow(GetDlgItem(hDlg, IDOK), TRUE);
	return 0;
}
// 고객정보 입력 대화 상자의 메시지 처리기입니다.
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
