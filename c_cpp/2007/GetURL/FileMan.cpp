#include "WinMain.h"
extern char fileName[MAX_PATH];
extern bool FileOpened;			// ���� ���ȴ��� ǥ�� flag
extern HWND hDlgMain, hListRes;

// ���� ����
void SaveFilefromListView(BOOL SaveAs)
{
	OPENFILENAME OFN;
	char lpstrFile[MAX_PATH]="";
	BOOL bResult = TRUE;		// ���� ��ȭ���� �Է� ������ FALSE�� �ٲ�.
	char FilePath[MAX_PATH];
	int idx;
	char SubCaption1[MAX_PATH+MAX_PATH], SubCaption2[MAX_PATH+MAX_PATH];
	char DomainName[MAX_PATH+MAX_PATH]="";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hDlgMain;
	OFN.lpstrFilter="�ؽ�Ʈ ����\0*.txt\0��� ����\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.lpstrTitle="URL ����Ʈ ���� ����";
	OFN.nMaxFile=MAX_PATH;
	OFN.lpstrDefExt="txt";
	FILE *fp;

	if(FileOpened==false || SaveAs) {	// �ٸ��̸����� ������ ��Ȳ�̸�
		bResult = GetSaveFileName(&OFN);
		strcpy(FilePath, OFN.lpstrFile);
	}
	else {	// ���� ���Ͽ� �����
		strcpy(FilePath, fileName);
	}

	if(bResult!=FALSE) {	// �����̸� ������
		// ���� ��� ����
		if ((fp=fopen(FilePath, "wt"))==NULL) {
			MessageBox(hDlgMain, "���� ���� ����!", "�˸�", MB_OK);
			return;
		}
		fputs(";;; URL list file\n;;; URL Captured by GetURL.\n", fp);
		idx = ListView_GetItemCount(hListRes);
		for(int i=0; i < idx; i++) {
			ListView_GetItemText(hListRes, i, 0, SubCaption1, (MAX_PATH+MAX_PATH));	// ����1 ������
			ListView_GetItemText(hListRes, i, 1, SubCaption2, (MAX_PATH+MAX_PATH));	// ����2 ������

			if(lstrcmp(DomainName, SubCaption2)!= 0) {
				lstrcpy(DomainName, SubCaption2);
				// '; <= SubCaption2'�� ���
				fputs("\n; <= ", fp);
				fputs(SubCaption2, fp);
				fputc('\n', fp);
			}
			// url ���
			fputs(SubCaption1, fp);
			fputc('\n', fp);
		}
		fclose(fp);
		/// ���� ��� ��

		// �ΰ��� ����
		strcpy(fileName, FilePath);	// ���� ������ ������ ���� �� ������ �ٲ�.
		FileOpened=true;	// ���� ��������.
	}
}