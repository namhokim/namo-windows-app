// ����<->edit box
#define MAXBUFFER 1048576	// 1MB
#define MAXBUFFER_98	32768	// ������ 95/98������ �ִ� 32K���� ������ ����.

// ���� ���� ��
BOOL BrowseFolder(HWND hWnd, char *szFolder);

///// URL file handle func /////////////////////////////////////////////////////
bool FileLoad(char *fileName);		// ���� ����
void EditBoxEnable(BOOL enable);	// enable or disable
void LoadFiletoEditBox();			// ����<->Edit box
void SaveFilefromEditBox(BOOL SaveAs);	// ���� ����
int ConfirmSave(void);				// ������ ���濩�� �ľ� ��, ���� ���� �ľ�.
void SaveTempFile();				// �ӽ������� �ӽ� ������ ���� �ٿ� ���� �� �ְ� �Ѵ�.
