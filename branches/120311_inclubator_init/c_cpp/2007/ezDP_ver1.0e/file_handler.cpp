#include <shlobj.h>		// including shell object
#include "file_handler.h"
#include "resource.h"

extern HWND hDlgMain;
extern HWND hEdit;		// ����Ʈ â �ڵ�.
extern char fileName[MAX_PATH+MAX_PATH];	// url list file full path
extern bool FileOpened=false;			// ���� ���ȴ��� ǥ�� flag
extern HWND hDnBtn;

// ���� ���� ��
BOOL BrowseFolder(HWND hWnd, char *szFolder)
{
	LPMALLOC pMalloc;
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	bi.hwndOwner = hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = "�Ʒ� ����Ʈ���� ������ �����ϼ���." ;
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
// ���� ����
bool FileLoad(char *fileName)
{
	OPENFILENAME OFN;
	fileName[0]='\0';

	// ���Ͽ��� ��ȭ����
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize=sizeof(OPENFILENAME);
	OFN.hwndOwner=hDlgMain;
	OFN.lpstrFilter="�ؽ�Ʈ ����\0*.txt\0��� ����\0*.*\0";
	OFN.lpstrFile=fileName;
	OFN.nMaxFile=MAX_PATH;
	OFN.lpstrTitle="URL ����� �ִ� ������ �����ϼ���.";
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
// ����<->Edit box
void LoadFiletoEditBox()
{
	DWORD dwRead;
	HANDLE hFile;
	char *pBuf;
	OSVERSIONINFO osv;
	DWORD FileSize;

	osv.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);	// �������� ������ �˾ƿ´�.


	if((pBuf=(char *)malloc(MAXBUFFER))==NULL) {
		MessageBox(hDlgMain, "�޸𸮸� �Ҵ��� �� �����ϴ�.", "�޸� ����", MB_OK);
		return;
	}
	
	hFile = CreateFile(fileName, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile!=INVALID_HANDLE_VALUE) {
		FileSize = GetFileSize(hFile, NULL);	// ������ ũ�⸦ �˾ƿ´�.
		if(FileSize>(DWORD)MAXBUFFER) {
			MessageBox(hDlgMain, "���� �뷮�� Ŀ�� ó�� �� �� �����ϴ�.", "����", MB_OK);
			fileName[0]=0;
			SetDlgItemText(hDlgMain,IDC_STATICFI,fileName);
		}
		else {
			ReadFile(hFile, pBuf, MAXBUFFER, &dwRead, NULL);
			CloseHandle(hFile);
			pBuf[dwRead]=0;
			if(dwRead>MAXBUFFER_98 && osv.dwPlatformId!=VER_PLATFORM_WIN32_NT) {
				MessageBox(hDlgMain, "���� �뷮�� Ŀ�� �����ͷ� �� �� �����ϴ�.", "����", MB_OK);
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

// ���� ����
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
	GetVersionEx(&osv);	// �������� ������ �˾ƿ´�.

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hDlgMain;
	OFN.lpstrFilter="�ؽ�Ʈ ����\0*.txt\0��� ����\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.lpstrTitle="URL ����Ʈ ���� ����";
	OFN.nMaxFile=MAX_PATH;
	OFN.lpstrDefExt="txt";

	if(FileOpened==false || SaveAs) {	// �ٸ��̸����� ������ ��Ȳ�̸�
		bResult = GetSaveFileName(&OFN);
		strcpy(FilePath, OFN.lpstrFile);
	}
	else {	// ���� ���Ͽ� �����
		strcpy(FilePath, fileName);
	}

	if(bResult!=FALSE) {
		if((pBuf=(char *)malloc(MAXBUFFER))==NULL) {
			MessageBox(hDlgMain, "�޸𸮸� �Ҵ��� �� �����ϴ�.", "�޸� ����", MB_OK);
			return;
		}
		if(osv.dwPlatformId!=VER_PLATFORM_WIN32_NT) {		// 95/98�� ���
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

		strcpy(fileName, FilePath);	// ���� ������ ������ ���� �� ������ �ٲ�.
		SetDlgItemText(hDlgMain,IDC_STATICFI,fileName);	// ��� ĸ�� �ٲ�.

		SendMessage(hEdit,EM_SETMODIFY, (WPARAM)FALSE,0);	// ����Ʈ ���� ���� ����

		FileOpened=true;	// ���� ��������.
		SetDlgItemText(hDlgMain,IDC_STATICFI,fileName);
		if(hDnBtn != NULL)	EnableWindow(hDnBtn, TRUE);	// �ٿ� ��ư enable
	}
}

int ConfirmSave(void)	// ������ ���濩�� �ľ� ��, ���� ���� �ľ�.
{
	if(SendMessage(hEdit, EM_GETMODIFY, 0,0)==TRUE) {
		return MessageBox(hDlgMain, "����� ������ �����ϰڽ��ϱ�?",
			"Ȯ��", MB_YESNOCANCEL);
	}
	else return IDNO;
}

/* �ӽ������� �ӽ� ������ ���� �ٿ� ���� �� �ְ� �Ѵ�. */
void SaveTempFile()
{
	char tpath[MAX_PATH];
	GetTempPath(MAX_PATH, tpath);
	wsprintf(tpath, "%snonF.txt", tpath);
	MessageBox(hDlgMain, tpath, "�˸�", MB_OK);
}