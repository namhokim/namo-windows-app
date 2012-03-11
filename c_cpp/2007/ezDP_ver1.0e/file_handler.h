// 파일<->edit box
#define MAXBUFFER 1048576	// 1MB
#define MAXBUFFER_98	32768	// 윈도우 95/98에서는 최대 32K까지 지원을 못함.

// 폴더 선택 쉘
BOOL BrowseFolder(HWND hWnd, char *szFolder);

///// URL file handle func /////////////////////////////////////////////////////
bool FileLoad(char *fileName);		// 파일 열기
void EditBoxEnable(BOOL enable);	// enable or disable
void LoadFiletoEditBox();			// 파일<->Edit box
void SaveFilefromEditBox(BOOL SaveAs);	// 파일 저장
int ConfirmSave(void);				// 에디터 변경여부 파악 후, 저장 여부 파악.
void SaveTempFile();				// 임시폴더에 임시 파일을 만들어서 다운 받을 수 있게 한다.
