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
bool FileOpened=false;			// 파일 열렸는지 표시 flag
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

	hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON_INIT));	// icon 가져온다.
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
	if(idx > GetMaxItemNum()) return FALSE;	// 현재 등록된 아이템이 제한 개수보다 많으면
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

	// 레벨 0
	switch(type)
	{
	case KW_ERR:
		break;
	case KW_IMG:	// 이미지 : .jpg, .gif	방법 : <img src>
		GetTagInfo2(addr, "img", "src");
		break;
	case KW_DOC:	// 문서 : .doc, .hwp, txt, pdf
		break;
	case KW_MUS:	// 음악파일 : .mp3, .wma, mid 방법 : <embed src>
		GetTagInfo2(addr, "embed", "src");
		break;
	case KW_SLI:	// 슬라이드 : .ppt
		break;
	case KW_COM:	// 압축파일 : .zip, .rar
		GetTagInfo2(addr, "embed", "src");
		break;
	}
	if(!PreCheck()) {	// 개수제한 검사
		TerminateThread(hTreadProg, THREAD_TERMINATE);
		ShowWindow(hProgress, SW_HIDE);
		
		return 0;
	}
	GetTagInfo2(addr, "a", "href");

	// level 1 이상
	LinkState = true;
	CurrLv = 0;

	GetTagInfo2(addr, "a", "href");
	GetTagInfo2(addr, "frame", "src");

	for(int i=0; i<LimitLv; i++) {
		int idx;
		char lv[MAX_PATH];

		// 새로 등록해야할 아이템 번호 get
		idx = ListView_GetItemCount(hListData);
		CurrLv = i+1;	// 다음에 Data View에 삽입되는 url의 레벨은 현재보다 1 많다.
		for(int ii=0; ii < idx; ii++) {	// 탐색 N
			
			ListView_GetItemText(hListData, ii, 1, lv, MAX_PATH);	// 레벨을 구한다.
			if(atoi(lv) == i) {	// 현재 또 따라가야하는 레벨하고 같을 때
				// url을 빼서 자료 추출
				LinkState = false;
				ListView_GetItemText(hListData, ii, 0, addr, (MAX_PATH+MAX_PATH));	// 레벨을 구한다.
//				MessageBox(hDlgMain, addr, "알림", MB_OK);
				if(!PreCheck()) {	// 개수제한 검사
					TerminateThread(hTreadProg, THREAD_TERMINATE);
					ShowWindow(hProgress, SW_HIDE);
					
					return 0;
				}
				switch(type)
				{
				case KW_ERR:
					break;
				case KW_IMG:	// 이미지 : .jpg, .gif	방법 : <img src>
					GetTagInfo2(addr, "img", "src");
					break;
				case KW_DOC:	// 문서 : .doc, .hwp, txt, pdf
					break;
				case KW_MUS:	// 음악파일 : .mp3, .wma, mid 방법 : <embed src>
					GetTagInfo2(addr, "embed", "src");
					break;
				case KW_SLI:	// 슬라이드 : .ppt
					break;
				case KW_COM:	// 압축파일 : .zip, .rar
					GetTagInfo2(addr, "embed", "src");
					break;
				}
				if(!PreCheck()) {	// 개수제한 검사
					TerminateThread(hTreadProg, THREAD_TERMINATE);
					ShowWindow(hProgress, SW_HIDE);
					
					return 0;
				}
				GetTagInfo2(addr, "a", "href");

				// 그 곳의 링크를 Data View에 추가 삽입(lv은 하나 증가해서)
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
		SendMessage(hDlg, WM_SETICON, TRUE, (LPARAM)hIcon);		// icon 부착
		hProgress = GetDlgItem(hDlg,IDC_PROGRESS_V);	// 프로그래스 윈도우 핸들 가져옴.
		hAddrEdit = GetDlgItem(hDlg,IDC_EDIT_ADDR);		// 주소 에디트 윈도우 핸들 가져옴.
		hEditKeyword = GetDlgItem(hDlg,IDC_EDIT_KW);	// 키워드 에디트 윈도우 핸들 가져옴.
		hEditLevel = GetDlgItem(hDlg,IDC_EDIT_LV);	// 검색 레벨 에디트 윈도우 핸들 가져옴.
		hEditNum = GetDlgItem(hDlg,IDC_EDIT_NUM);	// 검색 개수 에디트 윈도우 핸들 가져옴.
		hComboExt = GetDlgItem(hDlg,IDC_COMBO_EXT);	// 검색 확장자 콤보 윈도우 핸들 가져옴.
		hCheckDom = GetDlgItem(hDlg,IDC_CHECK_DOMAIN);	// 도메인 체크 윈도우 핸들 가져옴.
		hCheckGoogle = GetDlgItem(hDlg,IDC_CHECK_GOOGLE);	// 구글 체크 윈도우 핸들 가져옴.
		hEditGoogle = GetDlgItem(hDlg,IDC_EDIT_GOOGLE);	// 구글 에디트 윈도우 핸들 가져옴.

		SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0,100));	// 범위 지정
		hListRes = GetDlgItem(hDlg,IDC_LIST_RES);
		hListData = GetDlgItem(hDlg,IDC_LIST_DATA);
		// 리스트 뷰 윈도우 설정
		//	헤더 추가
		COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;
		COL.fmt=LVCFMT_LEFT;
		COL.cx=300;
		COL.pszText="URL";			// 첫번째 헤더
		COL.iSubItem=0;
		SendMessage(hListRes, LVM_INSERTCOLUMN, 0,(LPARAM)&COL);
		COL.pszText="Next URL";			// 첫번째 헤더
		SendMessage(hListData, LVM_INSERTCOLUMN, 0,(LPARAM)&COL);

		COL.cx=150;
		COL.pszText="From";		// 두번째 헤더
		COL.iSubItem=1;
		SendMessage(hListRes, LVM_INSERTCOLUMN, 1,(LPARAM)&COL);
		COL.pszText="Level";		// 두번째 헤더
		SendMessage(hListData, LVM_INSERTCOLUMN, 1,(LPARAM)&COL);

		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg,0);
			return TRUE;
		case ID_FILE_EXIT:		// 메뉴 선택
			EndDialog(hDlg, 0);
			return TRUE;
		case IDC_BUTTON_GO:
			char str_keyword[MAX_PATH];

			// 검색엔진 이용 OR 안 이용 결정
			if(SendMessage(hCheckGoogle, BM_GETCHECK, 0, 0)==BST_CHECKED) {
				if(GetWindowText(hEditGoogle, str_keyword, (MAX_PATH+MAX_PATH))>0) {
//					MessageBox(hDlg, str_keyword, "알림", MB_OK);
					for(int i=0; i<lstrlen(str_keyword)-1; i++) {
						if(str_keyword[i]==' ') str_keyword[i]='+';
					}
					wsprintf(addr, "http://www.google.com/search?hl=en&lr=&ie=UTF-8&oe=UTF-8&q=%s", str_keyword);
//					MessageBox(hDlg, addr, "알림", MB_OK);

					// working...
					// 리스트 뷰 삭제
					ListView_DeleteAllItems(hListRes);
					ListView_DeleteAllItems(hListData);
					// 쓰레드 수행
					ShowWindow(hProgress, SW_SHOW);
					// 쓰레드를 새로 하나 만들자!!!
					hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);
					// 태그는 <p class=g><a href= 의 경로를 찾는 것으로...
				}
				else {
					MessageBox(hDlg, "키워드를 입력하세요!", "알림", MB_OK);
					SetFocus(hEditGoogle);
				}
			}
			else {
				if(GetWindowText(hAddrEdit, addr, (MAX_PATH+MAX_PATH))>0) {
					// Go버튼을 눌렀을 때 할일을 기술함
					
						if(IsAbsUrl(addr)==FALSE) {
							MessageBox(hDlg, "절대 주소(http포함)로 써주세요.", "알림", MB_OK);
							SetFocus(hAddrEdit);
							SendMessage(hAddrEdit, EM_SETSEL, 0, -1);
						}
						else {	// 절대주소로 addr에 받아 왔다.
							if(GetWindowText(hEditKeyword, str_keyword, MAX_PATH)>0) {
								// 리스트 뷰 삭제
								ListView_DeleteAllItems(hListRes);
								ListView_DeleteAllItems(hListData);
								// 쓰레드 수행
								ShowWindow(hProgress, SW_SHOW);
								hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);
								}
							else {
								MessageBox(hDlg, "키워드(옆의 '키워드도움말' 참조)를 입력하세요!", "알림", MB_OK);
								SetFocus(hEditKeyword);
								SendMessage(hEditKeyword, EM_SETSEL, 0, -1);
							}
						}
				}
				else {
					MessageBox(hDlg, "주소를 입력하세요!", "알림", MB_OK);
					SetFocus(hAddrEdit);
				}
			}
			break;
		case ID_FILESAVE:	// 저장
			SaveFilefromListView(FALSE);
			break;
		case ID_FILE_SAVEAS:	// 저장
			SaveFilefromListView(TRUE);
			break;
		case IDC_BUTTON_KEYWORD:	// 키워드 팁.
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG_KEYWORD), hDlg, KeywordDlgProc);
			break;
		case IDC_EDIT_ADDR:	// 주소 에디트 윈도우
			switch (HIWORD(wParam)) {
			case EN_SETFOCUS:
				SendMessage(hAddrEdit, EM_SETSEL, 0, -1);
				break;
			}
			break;
		case IDC_EDIT_KW:	// 키워드 에디트 윈도우
			switch (HIWORD(wParam)) {
			case EN_SETFOCUS:
				SendMessage(hEditKeyword, EM_SETSEL, 0, -1);
				break;
			}
			break;
		// 새로운 case 추가 위치
			
		}
		return FALSE;
	}
	return FALSE;
}

