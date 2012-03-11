#include <windows.h>
#include <wininet.h>	// for internet connection
#include <stdio.h>
#include <Shellapi.h>	// for shell API funcs.
#include "resource.h"
#include "file_parser.h"
#include "dnseq.h"
#include "file_handler.h"	// edit <-> file

static bool FileOpened=false;			// ���� ���ȴ��� ǥ�� flag
static bool FileDownloading=false;		// �ٿ�ε� �� ���θ� ǥ�� flag
char fileName[MAX_PATH+MAX_PATH];	// url list file full path
static char fileName_tmp[MAX_PATH+MAX_PATH];	// url list file full path for backup
extern char target_dir[MAX_PATH];	// �ٿ�ε� �� ���
extern long total_url;	// �ٿ�ε� ������ url����
extern long fail_url;	// �ٿ�ε� ������ url����
#define	UI_KOR	0	// ���: �ѱ���
#define	UI_ENG	1	// ���: ����
static short int UI_Sel=UI_KOR;	// ��� ���� ����
static bool IsExtended=false;	// Ȯ�� �Է� �������� ���θ� ��Ÿ���� flag
// thread
extern HINTERNET hInternet, hURL;
extern HANDLE hDownloadedFile;

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
HWND hDlgMain;
HMENU hMenu=NULL;
HICON hIcon;
HWND hDnBtn;
HWND hEdit;		// ����Ʈ â �ڵ�.
HFONT hFontEdit;

///// WinMain /////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpszCmdParam, int nCmdshow)
{
	g_hInst=hInstance;
	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON_INIT));	// icon �����´�.

	// ��ȭ���ڸ� ����
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);
	return 0;
}
///////////////////////////////////////////////////// WinMain /////


///// Threads /////////////////////////////////////////////////////
static char ExecuteBtnCaption[20]="�ٿ�ε� ����(&D)";

DWORD WINAPI ThreadFunc(LPVOID temp)
{
	ExecuteDownload(hDlgMain, fileName);
	switch(UI_Sel) {
	case UI_KOR:
		wsprintf(ExecuteBtnCaption, "%s", "�ٿ�ε� ����(&D)");
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
		wsprintf(ExecuteBtnCaption, "%s", "�ٿ�ε� ����(&D)");
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

void SetPathtoURLlistFilePath()	// url ������ ���� ���� �� ��η� ���� ���� ��θ� �ٲ۴�.
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];

	// dn path
	_splitpath(fileName, drive, dir, NULL, NULL);	// ���� �� ������ ��θ� ����
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
	// Menu ����
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
		SendMessage(hDlg, WM_SETICON, TRUE, (LPARAM)hIcon);		// icon ����
		GetCurrentDirectory(MAX_PATH, target_dir);	// default ���� ���丮 => �ٿ�ε� ����
		SetDlgItemText(hDlg, IDC_STATICTA, target_dir);
		switch(UI_Sel) {
		case UI_KOR:
			hDnBtn = FindWindowEx(hDlg,NULL,NULL,"�ٿ�ε� ����(&D)");	// �ٿ�ε� ��ư�� �ڵ��� ����.
			break;
		case UI_ENG:
			hDnBtn = FindWindowEx(hDlg,NULL,NULL,"&Download Now");	// �ٿ�ε� ��ư�� �ڵ��� ����.
			break;
		}
		hEdit = CreateWindow("edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
					ES_MULTILINE | ES_WANTRETURN | WS_HSCROLL | WS_VSCROLL |
					ES_AUTOHSCROLL | ES_AUTOVSCROLL,
					10,180,460,150, hDlg, (HMENU)ID_EXT_EDIT, g_hInst, NULL);
		SendMessage(hEdit, EM_LIMITTEXT, (WPARAM)MAXBUFFER, 0);
		hFontEdit = CreateFont(15,0,0,0,0,0,0,0,HANGEUL_CHARSET, 3,2,1,
			VARIABLE_PITCH | FF_ROMAN, "����");
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
		fSystemMenu = (BOOL)HIWORD(lParam);	// �ý��� �޴����� ����
		uPos = (UINT)LOWORD(lParam);	// �� �޴����� �� ��° �ΰ�?
		hPopup = (HMENU)wParam;	// �޴� �ڵ���?

		if(fSystemMenu)	// �ý��� �޴��̸�
			break;
		if(uPos==0) // �ι�° ��޴��̸�(0���� count)
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
			MessageBox(hDlg, "�����Է�", "�˸�", MB_OK);
			break;
		case ID_FILE_QUIT:	// �޴����� ���� ����
		case IDC_EXIT:		// ���� ��ư Ŭ��
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
			SetWindowText(hEdit, "");	// ������ ���

			FileOpened = false;
			SetDlgItemText(hDlg,IDC_STATICFI,fileName);
			if(hDnBtn != NULL)	EnableWindow(hDnBtn, FALSE);	// �ٿ� ��ư gray
			EnableWindow(hEdit, TRUE);	// ������ ��� ����
			break;
		case ID_FILEOPEN:
			if(FileLoad(fileName)) {
				FileOpened=true;
				SetDlgItemText(hDlg,IDC_STATICFI,fileName);
				if(hDnBtn != NULL)	EnableWindow(hDnBtn, TRUE);	// �ٿ� ��ư enable
			}
			LoadFiletoEditBox();
			SetPathtoURLlistFilePath();
			// SetWindowText(hDlg, fileName);
			break;
		case ID_FILESAVE:	// ����
			SaveFilefromEditBox(FALSE);
			break;
		case ID_FILESAVEAS:	// �ٸ� �̸����� ����
			SaveFilefromEditBox(TRUE);
			break;
		case IDC_DNBTN:
		case ID_ACTION_DOWNLOADNOW:
			if(FileDownloading) {	// ���� �ٿ�ε����̸�,
				// The state of the specified object (thread) is signaled
				TerminateThread(hThread, THREAD_TERMINATE);
				// �����忡�� ���Ǵ� �ڵ��� �ݾ��ش�.
				InternetCloseHandle(hURL);
				InternetCloseHandle(hInternet);
				CloseHandle(hDownloadedFile);	// ������ ���� ���� ���, ���α׷��� ����ִµ��� ������ ���������.
				// �����尡 �׾�����, �ڵ��� �ݾ��ش�.
				CloseHandle(hThread);
				FileDownloading = false;
				switch(UI_Sel) {
				case UI_KOR:
					wsprintf(ExecuteBtnCaption, "%s", "�ٿ�ε� ����(&D)");
					SetDlgItemText(hDlg,IDC_STATUS,"�ٿ�ε� ��ҵ�.");
					break;
				case UI_ENG:
					wsprintf(ExecuteBtnCaption, "%s", "&Download Now");
					SetDlgItemText(hDlg,IDC_STATUS,"Download Canceled.");
					break;
				}
				SetDlgItemText(hDlg,IDC_DNBTN,ExecuteBtnCaption);
			}
			else {	// �׿ܿ���
				if(FileOpened) {
					hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);
					FileDownloading = true;
					switch(UI_Sel) {
					case UI_KOR:
						wsprintf(ExecuteBtnCaption, "%s", "�ٿ�ε� ����(&D)");
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
						MessageBox(hDlg,"���� URL����� �ִ� �ؽ�Ʈ ������ ���ʽÿ�.", "�˸�", MB_OK);
					}
				}
			}
			break;
		case ID_ACTION_SETDIRECTORYSAVEDFILES:
			if (BrowseFolder(hDlg,target_dir)) SetDlgItemText(hDlg, IDC_STATICTA, target_dir);
			break;
		case ID_ACTION_SETDIRECTORYCF:
			GetCurrentDirectory(MAX_PATH, target_dir);	// default ���� ���丮 => �ٿ�ε� ����
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
				wsprintf(ExecuteBtnCaption, "%s", "�ٿ�ε� ����(&D)");
			}
			else {
				wsprintf(ExecuteBtnCaption, "%s", "�ٿ�ε� ����(&D)");
			}
			
			SetDlgItemText(hDlg,IDC_DNBTN,ExecuteBtnCaption);
			SetDlgItemText(hDlg,IDC_EXIT,"����(&X)");
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
		case IDC_BUTTON_EXT:	// ������ Ȯ�� ��ư ������ ��.
			if(!IsExtended) {	// ��Ȯ�����
				// ������ ũ�� �ø���
				GetWindowRect(hDlg, &rect);
				MoveWindow(hDlg, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top+160, TRUE);
				
				// editâ visible = true;
				
				// ���� �޴� ����.
				SetDlgItemText(hDlg,IDC_BUTTON_EXT,"��");
				IsExtended = true;
				SetFocus(hEdit);	// �����ͷ� ��Ŀ��.
			}
			else {	// Ȯ�����
				GetWindowRect(hDlg, &rect);
				MoveWindow(hDlg, rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top-160, TRUE);
				// ������ ũ�� �������
				// editâ visible = false;
				// ���� �޴� ����.
				SetDlgItemText(hDlg,IDC_BUTTON_EXT,"��");
				IsExtended = false;
			}
			break;
		case ID_EXT_EDIT:
			switch(HIWORD(wParam)) {
			case EN_CHANGE:	// �ؽ�Ʈ�� �����...
				if(hDnBtn != NULL)	EnableWindow(hDnBtn, TRUE);
				break;
			case EN_MAXTEXT:
				MessageBox(hDlg, "�Է� ���ڼ��� �ʰ��Ͽ����ϴ�.", "����", MB_OK);
				break;
			}
		}
		return FALSE;
	case WM_USER+10:	// ������κ����� �޽���
		wsprintf(szTmp,"%d��° url: %d ����Ʈ �ް� �ֽ��ϴ�",total_url+1, lParam);
		SetDlgItemText(hDlg,IDC_STATUS,szTmp);
		break;
	case WM_USER+11:	// ������κ��� �޽���
		wsprintf(szTmp, "�ٿ�ε� �Ϸ� : %d / %d / %d(���� / ���� / ��ü)", total_url-fail_url, fail_url, total_url);
		SetDlgItemText(hDlg,IDC_STATUS,szTmp);
		FileDownloading = false;	// �Ϸ� -> �ٿ��� �ƴ�
		switch(UI_Sel) {
		case UI_KOR:
			wsprintf(ExecuteBtnCaption, "%s", "�ٿ�ε� ����(&D)");
			break;
		case UI_ENG:
			wsprintf(ExecuteBtnCaption, "%s", "&Download Now");
			break;
		}
		SetDlgItemText(hDlg,IDC_DNBTN,ExecuteBtnCaption);
		break;
	case WM_CLOSE:		// �ý��� �޴����� ���� or X��ư Ŭ����.
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

