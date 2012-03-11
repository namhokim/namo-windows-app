#include <shlobj.h>		// including shell object
#include "file_handler.h"
#include "resource.h"

extern HWND hDlgMain;
extern HWND hEdit;		// 에디트 창 핸들.
extern char fileName[MAX_PATH+MAX_PATH];	// url list file full path
extern bool FileOpened=false;			// 파일 열렸는지 표시 flag
extern HWND hDnBtn;

// 폴더 선택 쉘
BOOL BrowseFolder(HWND hWnd, char *szFolder)
{
	LPMALLOC pMalloc;
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = "아래 리스트에서 폴더를 선택하세요." ;
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
						
	pidl = SHBrowseForFolder(&bi);

	if (pidl == NULL) {
		return FALSE;
	}
	SHGetPathFromIDList(pidl, szFolder);

	if (SHGetMalloc(&pMalloc) != NOERROR) {
		return FALSE;
	}
	pMalloc->Free(pidl);
	pMalloc->Release();
	return TRUE;
}

///// URL file handle func /////////////////////////////////////////////////////
// 파일 열기
bool FileLoad(char *fileName)
{
	OPENFILENAME OFN;
	fileName[0]='\0';

	// 파일열기 대화상자
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize=sizeof(OPENFILENAME);
	OFN.hwndOwner=hDlgMain;
	OFN.lpstrFilter="텍스트 문서\0*.txt\0모든 파일\0*.*\0";
	OFN.lpstrFile=fileName;
	OFN.nMaxFile=MAX_PATH;
	OFN.lpstrTitle="URL 목록이 있는 파일을 선택하세요.";
	OFN.nMaxFileTitle=MAX_PATH;
	OFN.lpstrDefExt="txt";

	if(GetOpenFileName(&OFN)==0) return false;
	else return true;
}

void EditBoxEnable(BOOL enable)
{
	if(enable==TRUE) {
		EnableWindow(hEdit, TRUE);
	}
	else {
		SetWindowText(hEdit, "");
		EnableWindow(hEdit, FALSE);
	}
}
// 파일<->Edit box
void LoadFiletoEditBox()
{
	DWORD dwRead;
	HANDLE hFile;
	char *pBuf;
	OSVERSIONINFO osv;
	DWORD FileSize;

	osv.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);	// 윈도우의 버젼을 알아온다.


	if((pBuf=(char *)malloc(MAXBUFFER))==NULL) {
		MessageBox(hDlgMain, "메모리를 할당할 수 없습니다.", "메모리 부족", MB_OK);
		return;
	}
	
	hFile = CreateFile(fileName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile!=INVALID_HANDLE_VALUE) {
		FileSize = GetFileSize(hFile, NULL);	// 파일의 크기를 알아온다.
		if(FileSize>(DWORD)MAXBUFFER) {
			MessageBox(hDlgMain, "파일 용량이 커서 처리 할 수 없습니다.", "오류", MB_OK);
			fileName[0]=0;
			SetDlgItemText(hDlgMain,IDC_STATICFI,fileName);
		}
		else {
			ReadFile(hFile, pBuf, MAXBUFFER, &dwRead, NULL);
			CloseHandle(hFile);
			pBuf[dwRead]=0;
			if(dwRead>MAXBUFFER_98 && osv.dwPlatformId!=VER_PLATFORM_WIN32_NT) {
				MessageBox(hDlgMain, "파일 용량이 커서 에디터로 열 수 없습니다.", "오류", MB_OK);
				EditBoxEnable(FALSE);
			}
			else {
				SetWindowText(hEdit, (LPCTSTR)pBuf);
				EditBoxEnable(TRUE);
			}
		}
	}
	free(pBuf);

	SendMessage(hEdit, EM_SETMODIFY,(WPARAM)FALSE,0);
}

// 파일 저장
void SaveFilefromEditBox(BOOL SaveAs)
{
	OPENFILENAME OFN;
	char lpstrFile[MAX_PATH]="";
	HANDLE hFile;
	char *pBuf;
	int length;
	DWORD Written;
	BOOL bResult = TRUE;
	char FilePath[MAX_PATH];
	OSVERSIONINFO osv;

	osv.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);	// 윈도우의 버젼을 알아온다.

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hDlgMain;
	OFN.lpstrFilter="텍스트 문서\0*.txt\0모든 파일\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.lpstrTitle="URL 리스트 파일 저장";
	OFN.nMaxFile=MAX_PATH;
	OFN.lpstrDefExt="txt";

	if(FileOpened==false || SaveAs) {	// 다른이름으로 저장할 상황이면
		bResult = GetSaveFileName(&OFN);
		strcpy(FilePath, OFN.lpstrFile);
	}
	else {	// 동일 파일에 저장시
		strcpy(FilePath, fileName);
	}

	if(bResult!=FALSE) {
		if((pBuf=(char *)malloc(MAXBUFFER))==NULL) {
			MessageBox(hDlgMain, "메모리를 할당할 수 없습니다.", "메모리 부족", MB_OK);
			return;
		}
		if(osv.dwPlatformId!=VER_PLATFORM_WIN32_NT) {		// 95/98일 경우
			length = GetWindowText(hEdit, pBuf, MAXBUFFER_98);
		}
		else {
			length = GetWindowText(hEdit, pBuf, MAXBUFFER);
		}
		hFile = CreateFile(FilePath, GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(hFile, pBuf, length, &Written, NULL);
		CloseHandle(hFile);
		free(pBuf);

		strcpy(fileName, FilePath);	// 현재 저장한 파일을 현재 연 것으로 바꿈.
		SetDlgItemText(hDlgMain,IDC_STATICFI,fileName);	// 경로 캡션 바꿈.

		SendMessage(hEdit,EM_SETMODIFY, (WPARAM)FALSE,0);	// 에디트 수정 없음 상태

		FileOpened=true;	// 파일 열려있음.
		SetDlgItemText(hDlgMain,IDC_STATICFI,fileName);
		if(hDnBtn != NULL)	EnableWindow(hDnBtn, TRUE);	// 다운 버튼 enable
	}
}

int ConfirmSave(void)	// 에디터 변경여부 파악 후, 저장 여부 파악.
{
	if(SendMessage(hEdit, EM_GETMODIFY, 0,0)==TRUE) {
		return MessageBox(hDlgMain, "변경된 파일을 저장하겠습니까?",
			"확인", MB_YESNOCANCEL);
	}
	else return IDNO;
}

/* 임시폴더에 임시 파일을 만들어서 다운 받을 수 있게 한다. */
void SaveTempFile()
{
	char tpath[MAX_PATH];
	GetTempPath(MAX_PATH, tpath);
	wsprintf(tpath, "%snonF.txt", tpath);
	MessageBox(hDlgMain, tpath, "알림", MB_OK);
}