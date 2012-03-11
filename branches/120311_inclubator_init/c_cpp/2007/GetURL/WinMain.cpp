#include "resource.h"
#include "WinMain.h"
#include "Analyzer.h"
#include "FileMan.h"
#include "DataMan.h"

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInst;
HWND hDlgMain, hListRes, hListData, hProgress, hAddrEdit, hEditKeyword, hEditLevel, hEditNum;
HWND hComboExt, hCheckDom, hCheckGoogle, hEditGoogle;
HICON hIcon;
HANDLE hThread, hTreadProg;
char fileName[MAX_PATH];
bool FileOpened=false;			// ���� ���ȴ��� ǥ�� flag
int Keyword = -1;
char addr[(MAX_PATH+MAX_PATH)];
bool LinkState;
int CurrLv, CurrItem;
BOOL InetConnect = TRUE, DiskState = TRUE;

///// WinMain /////////////////////////////////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
					 LPSTR lpszCmdParam, int nCmdshow)
{
	g_hInst = hInstance;

	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON_INIT));	// icon �����´�.
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_MAIN), HWND_DESKTOP, MainDlgProc);

	return 0;
}
///////////////////////////////////////////////////// WinMain /////


///// Threads /////////////////////////////////////////////////////
DWORD WINAPI ThreadFuncProg(LPVOID temp)
{
	int i;

	i=0;
	while(1) {
		SendMessage(hProgress, PBM_SETPOS, i, 0);
		if(i>100) i = i % 100;
		i++;
		Sleep(500);
	}
	return 0;
}

int GetMaxItemNum()
{
	char str_tmp[MAX_PATH];

	if(GetWindowText(hEditNum, str_tmp, MAX_PATH)<1) return 50;
	else {
		return atoi(str_tmp);
	}
}

BOOL PreCheck()
{
	int idx;

	idx = ListView_GetItemCount(hListRes);
	if(idx > GetMaxItemNum()) return FALSE;	// ���� ��ϵ� �������� ���� �������� ������
	else return TRUE;
}
DWORD WINAPI ThreadFunc(LPVOID temp)
{
	DWORD ThreadID;
	int type, LimitLv;
	char str_tmp[MAX_PATH];

	if(GetWindowText(hEditLevel, str_tmp, MAX_PATH)<1) LimitLv = 1;
	else LimitLv = atoi(str_tmp);

	hTreadProg = CreateThread(NULL, 0, ThreadFuncProg, NULL, 0, &ThreadID);
	LinkState = false;
	type = DetKeyword();

	// ���� 0
	switch(type)
	{
	case KW_ERR:
		break;
	case KW_IMG:	// �̹��� : .jpg, .gif	��� : <img src>
		GetTagInfo2(addr, "img", "src");
		break;
	case KW_DOC:	// ���� : .doc, .hwp, txt, pdf
		break;
	case KW_MUS:	// �������� : .mp3, .wma, mid ��� : <embed src>
		GetTagInfo2(addr, "embed", "src");
		break;
	case KW_SLI:	// �����̵� : .ppt
		break;
	case KW_COM:	// �������� : .zip, .rar
		GetTagInfo2(addr, "embed", "src");
		break;
	}
	if(!PreCheck()) {	// �������� �˻�
		TerminateThread(hTreadProg, THREAD_TERMINATE);
		ShowWindow(hProgress, SW_HIDE);
		
		return 0;
	}
	GetTagInfo2(addr, "a", "href");

	// level 1 �̻�
	LinkState = true;
	CurrLv = 0;

	GetTagInfo2(addr, "a", "href");
	GetTagInfo2(addr, "frame", "src");

	for(int i=0; i<LimitLv; i++) {
		int idx;
		char lv[MAX_PATH];

		// ���� ����ؾ��� ������ ��ȣ get
		idx = ListView_GetItemCount(hListData);
		CurrLv = i+1;	// ������ Data View�� ���ԵǴ� url�� ������ ���纸�� 1 ����.
		for(int ii=0; ii < idx; ii++) {	// Ž�� N
			
			ListView_GetItemText(hListData, ii, 1, lv, MAX_PATH);	// ������ ���Ѵ�.
			if(atoi(lv) == i) {	// ���� �� ���󰡾��ϴ� �����ϰ� ���� ��
				// url�� ���� �ڷ� ����
				LinkState = false;
				ListView_GetItemText(hListData, ii, 0, addr, (MAX_PATH+MAX_PATH));	// ������ ���Ѵ�.
//				MessageBox(hDlgMain, addr, "�˸�", MB_OK);
				if(!PreCheck()) {	// �������� �˻�
					TerminateThread(hTreadProg, THREAD_TERMINATE);
					ShowWindow(hProgress, SW_HIDE);
					
					return 0;
				}
				switch(type)
				{
				case KW_ERR:
					break;
				case KW_IMG:	// �̹��� : .jpg, .gif	��� : <img src>
					GetTagInfo2(addr, "img", "src");
					break;
				case KW_DOC:	// ���� : .doc, .hwp, txt, pdf
					break;
				case KW_MUS:	// �������� : .mp3, .wma, mid ��� : <embed src>
					GetTagInfo2(addr, "embed", "src");
					break;
				case KW_SLI:	// �����̵� : .ppt
					break;
				case KW_COM:	// �������� : .zip, .rar
					GetTagInfo2(addr, "embed", "src");
					break;
				}
				if(!PreCheck()) {	// �������� �˻�
					TerminateThread(hTreadProg, THREAD_TERMINATE);
					ShowWindow(hProgress, SW_HIDE);
					
					return 0;
				}
				GetTagInfo2(addr, "a", "href");

				// �� ���� ��ũ�� Data View�� �߰� ����(lv�� �ϳ� �����ؼ�)
				LinkState = true;
				GetTagInfo2(addr, "a", "href");
				GetTagInfo2(addr, "frame", "src");
			}
		}
	}

	TerminateThread(hTreadProg, THREAD_TERMINATE);
	ShowWindow(hProgress, SW_HIDE);

	return 0;
}
///////////////////////////////////////////////////// Threads /////

///// Keywords Tip Dlg ////////////////////////////////////////////
BOOL CALLBACK KeywordDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
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
///////////////////////////////////////////////////// Keywords /////

BOOL CALLBACK MainDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	DWORD ThreadID;
	LVCOLUMN COL;

	switch(iMessage)
	{
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		SendMessage(hDlg, WM_SETICON, TRUE, (LPARAM)hIcon);		// icon ����
		hProgress = GetDlgItem(hDlg,IDC_PROGRESS_V);	// ���α׷��� ������ �ڵ� ������.
		hAddrEdit = GetDlgItem(hDlg,IDC_EDIT_ADDR);		// �ּ� ����Ʈ ������ �ڵ� ������.
		hEditKeyword = GetDlgItem(hDlg,IDC_EDIT_KW);	// Ű���� ����Ʈ ������ �ڵ� ������.
		hEditLevel = GetDlgItem(hDlg,IDC_EDIT_LV);	// �˻� ���� ����Ʈ ������ �ڵ� ������.
		hEditNum = GetDlgItem(hDlg,IDC_EDIT_NUM);	// �˻� ���� ����Ʈ ������ �ڵ� ������.
		hComboExt = GetDlgItem(hDlg,IDC_COMBO_EXT);	// �˻� Ȯ���� �޺� ������ �ڵ� ������.
		hCheckDom = GetDlgItem(hDlg,IDC_CHECK_DOMAIN);	// ������ üũ ������ �ڵ� ������.
		hCheckGoogle = GetDlgItem(hDlg,IDC_CHECK_GOOGLE);	// ���� üũ ������ �ڵ� ������.
		hEditGoogle = GetDlgItem(hDlg,IDC_EDIT_GOOGLE);	// ���� ����Ʈ ������ �ڵ� ������.

		SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0,100));	// ���� ����
		hListRes = GetDlgItem(hDlg,IDC_LIST_RES);
		hListData = GetDlgItem(hDlg,IDC_LIST_DATA);
		// ����Ʈ �� ������ ����
		//	��� �߰�
		COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;
		COL.fmt=LVCFMT_LEFT;
		COL.cx=300;
		COL.pszText="URL";			// ù��° ���
		COL.iSubItem=0;
		SendMessage(hListRes, LVM_INSERTCOLUMN, 0,(LPARAM)&COL);
		COL.pszText="Next URL";			// ù��° ���
		SendMessage(hListData, LVM_INSERTCOLUMN, 0,(LPARAM)&COL);

		COL.cx=150;
		COL.pszText="From";		// �ι�° ���
		COL.iSubItem=1;
		SendMessage(hListRes, LVM_INSERTCOLUMN, 1,(LPARAM)&COL);
		COL.pszText="Level";		// �ι�° ���
		SendMessage(hListData, LVM_INSERTCOLUMN, 1,(LPARAM)&COL);

		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg,0);
			return TRUE;
		case ID_FILE_EXIT:		// �޴� ����
			EndDialog(hDlg, 0);
			return TRUE;
		case IDC_BUTTON_GO:
			char str_keyword[MAX_PATH];

			// �˻����� �̿� OR �� �̿� ����
			if(SendMessage(hCheckGoogle, BM_GETCHECK, 0, 0)==BST_CHECKED) {
				if(GetWindowText(hEditGoogle, str_keyword, (MAX_PATH+MAX_PATH))>0) {
//					MessageBox(hDlg, str_keyword, "�˸�", MB_OK);
					for(int i=0; i<lstrlen(str_keyword)-1; i++) {
						if(str_keyword[i]==' ') str_keyword[i]='+';
					}
					wsprintf(addr, "http://www.google.com/search?hl=en&lr=&ie=UTF-8&oe=UTF-8&q=%s", str_keyword);
//					MessageBox(hDlg, addr, "�˸�", MB_OK);

					// working...
					// ����Ʈ �� ����
					ListView_DeleteAllItems(hListRes);
					ListView_DeleteAllItems(hListData);
					// ������ ����
					ShowWindow(hProgress, SW_SHOW);
					// �����带 ���� �ϳ� ������!!!
					hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);
					// �±״� <p class=g><a href= �� ��θ� ã�� ������...
				}
				else {
					MessageBox(hDlg, "Ű���带 �Է��ϼ���!", "�˸�", MB_OK);
					SetFocus(hEditGoogle);
				}
			}
			else {
				if(GetWindowText(hAddrEdit, addr, (MAX_PATH+MAX_PATH))>0) {
					// Go��ư�� ������ �� ������ �����
					
						if(IsAbsUrl(addr)==FALSE) {
							MessageBox(hDlg, "���� �ּ�(http����)�� ���ּ���.", "�˸�", MB_OK);
							SetFocus(hAddrEdit);
							SendMessage(hAddrEdit, EM_SETSEL, 0, -1);
						}
						else {	// �����ּҷ� addr�� �޾� �Դ�.
							if(GetWindowText(hEditKeyword, str_keyword, MAX_PATH)>0) {
								// ����Ʈ �� ����
								ListView_DeleteAllItems(hListRes);
								ListView_DeleteAllItems(hListData);
								// ������ ����
								ShowWindow(hProgress, SW_SHOW);
								hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);
								}
							else {
								MessageBox(hDlg, "Ű����(���� 'Ű���嵵��' ����)�� �Է��ϼ���!", "�˸�", MB_OK);
								SetFocus(hEditKeyword);
								SendMessage(hEditKeyword, EM_SETSEL, 0, -1);
							}
						}
				}
				else {
					MessageBox(hDlg, "�ּҸ� �Է��ϼ���!", "�˸�", MB_OK);
					SetFocus(hAddrEdit);
				}
			}
			break;
		case ID_FILESAVE:	// ����
			SaveFilefromListView(FALSE);
			break;
		case ID_FILE_SAVEAS:	// ����
			SaveFilefromListView(TRUE);
			break;
		case IDC_BUTTON_KEYWORD:	// Ű���� ��.
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_KEYWORD), hDlg, KeywordDlgProc);
			break;
		case IDC_EDIT_ADDR:	// �ּ� ����Ʈ ������
			switch (HIWORD(wParam)) {
			case EN_SETFOCUS:
				SendMessage(hAddrEdit, EM_SETSEL, 0, -1);
				break;
			}
			break;
		case IDC_EDIT_KW:	// Ű���� ����Ʈ ������
			switch (HIWORD(wParam)) {
			case EN_SETFOCUS:
				SendMessage(hEditKeyword, EM_SETSEL, 0, -1);
				break;
			}
			break;
		// ���ο� case �߰� ��ġ
			
		}
		return FALSE;
	}
	return FALSE;
}

