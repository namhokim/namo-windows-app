#include "WinMain.h"
extern char fileName[MAX_PATH];
extern bool FileOpened;			// 파일 열렸는지 표시 flag
extern HWND hDlgMain, hListRes;

// 파일 저장
void SaveFilefromListView(BOOL SaveAs)
{
	OPENFILENAME OFN;
	char lpstrFile[MAX_PATH]="";
	BOOL bResult = TRUE;		// 저장 대화상자 입력 없으면 FALSE로 바뀜.
	char FilePath[MAX_PATH];
	int idx;
	char SubCaption1[MAX_PATH+MAX_PATH], SubCaption2[MAX_PATH+MAX_PATH];
	char DomainName[MAX_PATH+MAX_PATH]="";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hDlgMain;
	OFN.lpstrFilter="텍스트 문서\0*.txt\0모든 파일\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.lpstrTitle="URL 리스트 파일 저장";
	OFN.nMaxFile=MAX_PATH;
	OFN.lpstrDefExt="txt";
	FILE *fp;

	if(FileOpened==false || SaveAs) {	// 다른이름으로 저장할 상황이면
		bResult = GetSaveFileName(&OFN);
		strcpy(FilePath, OFN.lpstrFile);
	}
	else {	// 동일 파일에 저장시
		strcpy(FilePath, fileName);
	}

	if(bResult!=FALSE) {	// 파일이름 지정시
		// 파일 기록 관련
		if ((fp=fopen(FilePath, "wt"))==NULL) {
			MessageBox(hDlgMain, "파일 저장 에러!", "알림", MB_OK);
			return;
		}
		fputs(";;; URL list file\n;;; URL Captured by GetURL.\n", fp);
		idx = ListView_GetItemCount(hListRes);
		for(int i=0; i < idx; i++) {
			ListView_GetItemText(hListRes, i, 0, SubCaption1, (MAX_PATH+MAX_PATH));	// 서브1 가져옴
			ListView_GetItemText(hListRes, i, 1, SubCaption2, (MAX_PATH+MAX_PATH));	// 서브2 가져옴

			if(lstrcmp(DomainName, SubCaption2)!= 0) {
				lstrcpy(DomainName, SubCaption2);
				// '; <= SubCaption2'을 기록
				fputs("\n; <= ", fp);
				fputs(SubCaption2, fp);
				fputc('\n', fp);
			}
			// url 기록
			fputs(SubCaption1, fp);
			fputc('\n', fp);
		}
		fclose(fp);
		/// 파일 기록 끝

		// 부과적 설정
		strcpy(fileName, FilePath);	// 현재 저장한 파일을 현재 연 것으로 바꿈.
		FileOpened=true;	// 파일 열려있음.
	}
}