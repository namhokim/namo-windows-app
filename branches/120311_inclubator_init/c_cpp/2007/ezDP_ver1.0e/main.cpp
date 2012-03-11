#include <windows.h>
#include <wininet.h>	// for internet connection
#include <stdio.h>
#include <Shellapi.h>	// for shell API funcs.
#include "resource.h"
#include "file_parser.h"
#include "dnseq.h"
#include "file_handler.h"	// edit <-> file

static bool FileOpened=false;			// 파일 열렸는지 표시 flag
static bool FileDownloading=false;		// 다운로딩 중 여부를 표시 flag
char fileName[MAX_PATH+MAX_PATH];	// url list file full path
static char fileName_tmp[MAX_PATH+MAX_PATH];	// url list file full path for backup
extern char target_dir[MAX_PATH];	// 다운로드 할 경로
extern long total_url;	// 다운로드 성공한 url개수
extern long fail_url;	// 다운로드 실패한 url개수
#define	UI_KOR	0	// 언어: 한국어
#define	UI_ENG	1	// 언어: 영어
static short int UI_Sel=UI_KOR;	// 언어 선택 여부
static bool IsExtended=false;	// 확장 입력 상태인지 여부를 나타내는 flag
// thread
extern HINTERNET hInternet, hURL;
extern HANDLE hDownloadedFile;

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
HWND hDlgMain;
HMENU hMenu=NULL;
HICON hIcon;
HWND hDnBtn;
HWND hEdit;		// 에디트 창 핸들.
HFONT hFontEdit;

///// WinMain /////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpszCmdParam, int nCmdshow)
{
	g_hInst=hInstance;
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON_INIT));	// icon 가져온다.

	// 대화상자를 실행
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);
	return 0;
}
///////////////////////////////////////////////////// WinMain /////


///// Threads /////////////////////////////////////////////////////
static char ExecuteBtnCaption[20]="다운로드 시작(&D)";

DWORD WINAPI ThreadFunc(LPVOID temp)
{
	ExecuteDownload(hDlgMain, fileName);
	switch(UI_Sel) {
	case UI_KOR:
		wsprintf(ExecuteBtnCaption, "%s", "다운로드 시작(&D)");
		break;
	case UI_ENG:
		wsprintf(ExecuteBtnCaption, "%s", "&Download Now !!!");
		break;
	}
	return 0;
}

DWORD WINAPI ThreadFunc2(LPVOID temp)	// for tmp download
{
	ExecuteDownload(hDlgMain, fileName_tmp);
	switch(UI_Sel) {
	case UI_KOR:
		wsprintf(ExecuteBtnCaption, "%s", "다운로드 시작(&D)");
		break;
	case UI_ENG:
		wsprintf(ExecuteBtnCaption, "%s", "&Download Now !!!");
		break;
	}
	return 0;
}
///////////////////////////////////////////////////// Threads /////


BOOL CALLBACK AboutDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			EndDialog(hDlg, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

///////////////////////////////////////////////////// URL file handle func /////

void SetPathtoURLlistFilePath()	// url 파일을 열면 파일 연 경로로 저장 받을 경로를 바꾼다.
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];

	// dn path
	_splitpath(fileName, drive, dir, NULL, NULL);	// 현재 연 파일의 경로를 구함
	wsprintf(target_dir, "%s%s", drive, dir);
	SetDlgItemText(hDlgMain, IDC_STATICTA, target_dir);	// static lable change
}

///
BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hThread;
	DWORD ThreadID;
	HDROP hDrop;
	RECT rect;
	int Result;
	char szTmp[100];
	// Menu 관련
	BOOL fSystemMenu;
	UINT uPos;
	HMENU hPopup;

	switch(iMessage)
	{
	case WM_INITMENU:
		if(FileOpened || SendMessage(hEdit, EM_GETMODIFY, 0,0)) {
			ModifyMenu((HMENU)wParam, ID_ACTION_DOWNLOADNOW, MF_BYCOMMAND | MF_STRING | MF_ENABLED, 
				ID_ACTION_DOWNLOADNOW, ExecuteBtnCaption);
		}
		else {
			ModifyMenu((HMENU)wParam, ID_ACTION_DOWNLOADNOW, MF_BYCOMMAND | MF_STRING | MF_GRAYED, 
				ID_ACTION_DOWNLOADNOW, ExecuteBtnCaption);
		}
		return TRUE;
	case WM_INITDIALOG:
		hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1));

		hDlgMain = hDlg;
		SendMessage(hDlg, WM_SETICON, TRUE, (LPARAM)hIcon);		// icon 부착
		GetCurrentDirectory(MAX_PATH, target_dir);	// default 현재 디렉토리 => 다운로딩 폴더
		SetDlgItemText(hDlg, IDC_STATICTA, target_dir);
		switch(UI_Sel) {
		case UI_KOR:
			hDnBtn = FindWindowEx(hDlg,NULL,NULL,"다운로드 시작(&D)");	// 다운로드 버튼의 핸들을 구함.
			break;
		case UI_ENG:
			hDnBtn = FindWindowEx(hDlg,NULL,NULL,"&Download Now");	// 다운로드 버튼의 핸들을 구함.
			break;
		}
		hEdit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
					ES_MULTILINE | ES_WANTRETURN | WS_HSCROLL | WS_VSCROLL |
					ES_AUTOHSCROLL | ES_AUTOVSCROLL,
					10,180,460,150, hDlg, (HMENU)ID_EXT_EDIT, g_hInst, NULL);
		SendMessage(hEdit, EM_LIMITTEXT, (WPARAM)MAXBUFFER, 0);
		hFontEdit = CreateFont(15,0,0,0,0,0,0,0,HANGEUL_CHARSET, 3,2,1,
			VARIABLE_PITCH | FF_ROMAN, "굴림");
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hFontEdit, MAKELPARAM(FALSE,0));
		return TRUE;
	case WM_DROPFILES:
		

		hDrop = (HDROP)wParam;
		DragQueryFile(hDrop,0,fileName,MAX_PATH+MAX_PATH);
		CloseHandle(hDrop);

		FileOpened=true;

		SetDlgItemText(hDlg,IDC_STATICFI,fileName);
		if(hDnBtn != NULL)	EnableWindow(hDnBtn, TRUE);

		LoadFiletoEditBox();
		SetPathtoURLlistFilePath();
		return TRUE;
	case WM_INITMENUPOPUP:
		fSystemMenu = (BOOL)HIWORD(lParam);	// 시스템 메뉴인지 여부
		uPos = (UINT)LOWORD(lParam);	// 톱 메뉴에서 몇 번째 인가?
		hPopup = (HMENU)wParam;	// 메뉴 핸들은?

		if(fSystemMenu)	// 시스템 메뉴이면
			break;
		if(uPos==0) // 두번째 톱메뉴이면(0부터 count)
		{
			if(IsWindowEnabled(hEdit)==FALSE) {
				EnableMenuItem(hPopup, ID_FILESAVE, MF_BYCOMMAND | MF_GRAYED);
				EnableMenuItem(hPopup, ID_FILESAVEAS, MF_BYCOMMAND | MF_GRAYED);
			}
			else {
				EnableMenuItem(hPopup, ID_FILESAVE, MF_BYCOMMAND | MF_ENABLED);
				EnableMenuItem(hPopup, ID_FILESAVEAS, MF_BYCOMMAND | MF_ENABLED);
			}
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			MessageBox(hDlg, "엔터입력", "알림", MB_OK);
			break;
		case ID_FILE_QUIT:	// 메뉴에서 종료 선택
		case IDC_EXIT:		// 종료 버튼 클릭
			Result = ConfirmSave();
			if(Result==IDCANCEL) break;
			else if(Result==IDYES) SaveFilefromEditBox(FALSE);

			EndDialog(hDlg, 0);
			return TRUE;
		case ID_FILENEW:
			Result = ConfirmSave();
			if(Result==IDCANCEL) break;
			else if(Result==IDYES) SaveFilefromEditBox(FALSE);

			fileName[0]=0;
			SetWindowText(hEdit, "");	// 에디터 비움

			FileOpened = false;
			SetDlgItemText(hDlg,IDC_STATICFI,fileName);
			if(hDnBtn != NULL)	EnableWindow(hDnBtn, FALSE);	// 다운 버튼 gray
			EnableWindow(hEdit, TRUE);	// 에디터 사용 가능
			break;
		case ID_FILEOPEN:
			if(FileLoad(fileName)) {
				FileOpened=true;
				SetDlgItemText(hDlg,IDC_STATICFI,fileName);
				if(hDnBtn != NULL)	EnableWindow(hDnBtn, TRUE);	// 다운 버튼 enable
			}
			LoadFiletoEditBox();
			SetPathtoURLlistFilePath();
			// SetWindowText(hDlg, fileName);
			break;
		case ID_FILESAVE:	// 저장
			SaveFilefromEditBox(FALSE);
			break;
		case ID_FILESAVEAS:	// 다른 이름으로 저장
			SaveFilefromEditBox(TRUE);
			break;
		case IDC_DNBTN:
		case ID_ACTION_DOWNLOADNOW:
			if(FileDownloading) {	// 파일 다운로드중이면,
				// The state of the specified object (thread) is signaled
				TerminateThread(hThread, THREAD_TERMINATE);
				// 쓰레드에서 사용되던 핸들을 닫아준다.
				InternetCloseHandle(hURL);
				InternetCloseHandle(hInternet);
				CloseHandle(hDownloadedFile);	// 파일이 열려 있을 경우, 프로그램이 살아있는동안 파일을 지울수없다.
				// 쓰레드가 죽었으니, 핸들을 닫아준다.
				CloseHandle(hThread);
				FileDownloading = false;
				switch(UI_Sel) {
				case UI_KOR:
					wsprintf(ExecuteBtnCaption, "%s", "다운로드 시작(&D)");
					SetDlgItemText(hDlg,IDC_STATUS,"다운로드 취소됨.");
					break;
				case UI_ENG:
					wsprintf(ExecuteBtnCaption, "%s", "&Download Now");
					SetDlgItemText(hDlg,IDC_STATUS,"Download Canceled.");
					break;
				}
				SetDlgItemText(hDlg,IDC_DNBTN,ExecuteBtnCaption);
			}
			else {	// 그외에는
				if(FileOpened) {
					hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);
					FileDownloading = true;
					switch(UI_Sel) {
					case UI_KOR:
						wsprintf(ExecuteBtnCaption, "%s", "다운로드 중지(&D)");
						break;
					case UI_ENG:
						wsprintf(ExecuteBtnCaption, "%s", "&Download Stop");
						break;
					}
					SetDlgItemText(hDlg,IDC_DNBTN,ExecuteBtnCaption);
				}
				else {
					if(SendMessage(hEdit, EM_GETMODIFY, 0,0)==TRUE) {
						SaveTempFile();
					}
					else {
						MessageBox(hDlg,"먼저 URL목록이 있는 텍스트 파일을 여십시요.", "알림", MB_OK);
					}
				}
			}
			break;
		case ID_ACTION_SETDIRECTORYSAVEDFILES:
			if (BrowseFolder(hDlg,target_dir)) SetDlgItemText(hDlg, IDC_STATICTA, target_dir);
			break;
		case ID_ACTION_SETDIRECTORYCF:
			GetCurrentDirectory(MAX_PATH, target_dir);	// default 현재 디렉토리 => 다운로딩 폴더
			SetDlgItemText(hDlg, IDC_STATICTA, target_dir);
			break;
		case ID_HELP_INFO:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG2), hDlg, AboutDlgProc);
			break;
		case ID_USERINTERFACE_KOREAN:
			hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU2));
			SetMenu(hDlg, hMenu);
			UI_Sel = UI_KOR;
			if(FileDownloading) {
				wsprintf(ExecuteBtnCaption, "%s", "다운로드 중지(&D)");
			}
			else {
				wsprintf(ExecuteBtnCaption, "%s", "다운로드 시작(&D)");
			}
			
			SetDlgItemText(hDlg,IDC_DNBTN,ExecuteBtnCaption);
			SetDlgItemText(hDlg,IDC_EXIT,"종료(&X)");
			break;
		case ID_USERINTERFACE_ENGLISH:
			hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			SetMenu(hDlg, hMenu);
			UI_Sel = UI_ENG;
			if(FileDownloading) {
				wsprintf(ExecuteBtnCaption, "%s", "&Download Stop");
			}
			else {
				wsprintf(ExecuteBtnCaption, "%s", "&Download Now");
			}
			SetDlgItemText(hDlg,IDC_DNBTN,ExecuteBtnCaption);
			SetDlgItemText(hDlg,IDC_EXIT,"E&Xit");
			break;
		case IDC_BUTTON_EXT:	// 에디터 확장 버튼 눌렀을 때.
			if(!IsExtended) {	// 비확장상태
				// 윈도우 크기 늘리고
				GetWindowRect(hDlg, &rect);
				MoveWindow(hDlg, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top+160, TRUE);
				
				// edit창 visible = true;
				
				// 저장 메뉴 등장.
				SetDlgItemText(hDlg,IDC_BUTTON_EXT,"▲");
				IsExtended = true;
				SetFocus(hEdit);	// 에디터로 포커싱.
			}
			else {	// 확장상태
				GetWindowRect(hDlg, &rect);
				MoveWindow(hDlg, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top-160, TRUE);
				// 윈도우 크기 원래대로
				// edit창 visible = false;
				// 저장 메뉴 없앰.
				SetDlgItemText(hDlg,IDC_BUTTON_EXT,"▼");
				IsExtended = false;
			}
			break;
		case ID_EXT_EDIT:
			switch(HIWORD(wParam)) {
			case EN_CHANGE:	// 텍스트가 변경시...
				if(hDnBtn != NULL)	EnableWindow(hDnBtn, TRUE);
				break;
			case EN_MAXTEXT:
				MessageBox(hDlg, "입력 문자수를 초과하였습니다.", "에러", MB_OK);
				break;
			}
		}
		return FALSE;
	case WM_USER+10:	// 쓰레드로부터의 메시지
		wsprintf(szTmp,"%d번째 url: %d 바이트 받고 있습니다",total_url+1, lParam);
		SetDlgItemText(hDlg,IDC_STATUS,szTmp);
		break;
	case WM_USER+11:	// 쓰레드로부터 메시지
		wsprintf(szTmp, "다운로드 완료 : %d / %d / %d(성공 / 실패 / 전체)", total_url-fail_url, fail_url, total_url);
		SetDlgItemText(hDlg,IDC_STATUS,szTmp);
		FileDownloading = false;	// 완료 -> 다운중 아님
		switch(UI_Sel) {
		case UI_KOR:
			wsprintf(ExecuteBtnCaption, "%s", "다운로드 시작(&D)");
			break;
		case UI_ENG:
			wsprintf(ExecuteBtnCaption, "%s", "&Download Now");
			break;
		}
		SetDlgItemText(hDlg,IDC_DNBTN,ExecuteBtnCaption);
		break;
	case WM_CLOSE:		// 시스템 메뉴에서 종료 or X버튼 클릭시.
		Result = ConfirmSave();
		if(Result==IDCANCEL) break;
		else if(Result==IDYES) SaveFilefromEditBox(FALSE);

		EndDialog(hDlg, 0);
		return TRUE;
	case WM_DESTROY:
		DeleteObject(hFontEdit);
		CloseHandle(hThread);
		PostQuitMessage(0);
		return 0;
	}
    return FALSE;
}

