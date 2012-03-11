#include "WinMain.h"
#include "DataMan.h"

extern HWND hListRes, hListData, hEditKeyword;
extern HWND hDlgMain, hComboExt, hCheckDom;
extern bool LinkState;
extern int CurrLv;

void InsListView(char *url, char *purl, HWND hList)
{
	int idx;
	LVFINDINFO fi;
	LVITEM LI;

	// ����ϴ� url�� �ߺ� ����
	fi.flags		= LVFI_STRING;
	fi.psz			= url;
	fi.vkDirection	= VK_DOWN;
	idx = ListView_FindItem(hList,-1,&fi);
	if (idx==-1) {	// ����.

	// ���� ����ؾ��� ������ ��ȣ get
	idx = ListView_GetItemCount(hList);

	// �ؽ�Ʈ�� �̹����� ���� �����۵��� ����Ѵ�.
		LI.mask=LVIF_TEXT;
		LI.state=0;
		LI.stateMask=0;
		
		LI.iImage=-1;
		LI.iItem=idx;
		LI.iSubItem=0;
		LI.pszText=url;		// ù��° ������
		ListView_InsertItem(hList, &LI);

		ListView_SetItemText(hList,idx,1,purl);
	}
}

void DataMan(char *url, char *purl)
{
	int type;
	char Extend[20];
	char tmp_str[MAX_PATH];
//	char tmp[10];

	type = DetKeyword();
	GetExt(url, Extend);	// Ȯ���ڸ� ���Ѵ�.

	if(GetWindowText(hComboExt,tmp_str, MAX_PATH)>0 && type!=KW_LIK) {
		if(!stricmp(tmp_str, Extend)) InsListView(url, purl, hListRes);	// Ȯ���ڰ� ���� ���� �߰�
		return;
	}
	if(type==KW_LIK) {
		char Proc[10], Path[MAX_PATH], File[MAX_PATH];
		char CHost[MAX_PATH], PHost[MAX_PATH];
		if(SendMessage(hCheckDom, BM_GETCHECK, 0, 0)==BST_CHECKED) {	// ���� �����γ�������
			DivUrl(url, Proc, CHost, Path, File);
			DivUrl(purl, Proc, PHost, Path, File);

			if(stricmp(CHost,PHost)) return;	//���� ȣ��Ʈ�� ���� �ٸ��� ��� ���.
		}
	}

	switch(type)
	{
	case KW_NOT:
		InsListView(url, purl, hListRes);
		break;
	case KW_IMG:	// �̹��� : .jpg, .gif	��� : <img src>
		if(!stricmp(Extend, "jpg") || !stricmp(Extend, "gif") || !stricmp(Extend, "bmp")
			 || !stricmp(Extend, "wmf") || !stricmp(Extend, "pcx") || !stricmp(Extend, "png")) {
			InsListView(url, purl, hListRes);
		}
		break;
	case KW_DOC:	// ���� : .doc, .hwp, txt, pdf
		if(!stricmp(Extend, "doc") || !stricmp(Extend, "hwp")
			|| !stricmp(Extend, "txt") || !stricmp(Extend, "pdf")) {
			InsListView(url, purl, hListRes);
		}
		break;
	case KW_MUS:	// �������� : .mp3, .wma, mid ��� : <embed src>
		if(!stricmp(Extend, "mp3") || !stricmp(Extend, "wma") || !stricmp(Extend, "mid")
			 || !stricmp(Extend, "wav")) {
			InsListView(url, purl, hListRes);
		}
		break;
	case KW_SLI:	// �����̵� : .ppt
		if(!stricmp(Extend, "ppt")) {
			InsListView(url, purl, hListRes);
		}
		break;
	case KW_COM:	// �������� : .zip, .rar
		if(!stricmp(Extend, "zip") || !stricmp(Extend, "rar") || !stricmp(Extend, "arj")
			 || !stricmp(Extend, "lzh") || !stricmp(Extend, "gz")) {
			InsListView(url, purl, hListRes);
		}
		break;
	case KW_MAI:
		if(url[0]=='m' && url[1]=='a' && url[2]=='i' && url[3]=='l'
			&& url[4]=='t' && url[5]=='o' && url[6]==':') {
			InsListView(&url[7], purl, hListRes);
		}
		break;
	case KW_LIK:
		char Lv[MAX_PATH];
		// Ȯ���ڰ� ���� ��� : htm, html, php, jsp, asp, aspx, cgi ��.
		wsprintf(Lv, "%d", CurrLv);
		InsListView(url, Lv, hListData);
		break;
	}
	
//	wsprintf(tmp, "%d", type);
//	InsListView(Extend, tmp, hListData);
}

int DetKeyword() {
	char str_keyword[MAX_PATH];

	if(GetWindowText(hEditKeyword, str_keyword, MAX_PATH)<1) return -1;
	if(LinkState) return KW_LIK;

	if(lstrcmp("�̹���", str_keyword)==0) {
		return KW_IMG;
	}
	else if(lstrcmp("����", str_keyword)==0) {
		return KW_DOC;
	}
	else if(lstrcmp("����", str_keyword)==0) {
		return KW_MUS;
	}
	else if(lstrcmp("�����̵�", str_keyword)==0) {
		return KW_SLI;
	}
	else if(lstrcmp("����", str_keyword)==0) {
		return KW_COM;
	}
	else if(lstrcmp("2����", str_keyword)==0) {
		return KW_MAI;
	}
	else {
		return KW_NOT;
	}
	return -1;
}

char *GetExt(char *url, char *Ext)
{
	char tmp_url[MAX_PATH+MAX_PATH];
	char buf[MAX_PATH];
	char *token;
	char seps[]   = "\\/:*\"<>|.?=\t\n";

	// �ʱ�ȭ �۾�(token�� ó�� ���� ���� �� �ִ�.)
	buf[0]='\n';

	lstrcpy(tmp_url, url);	// ���������Ͱ� �սǵǴ� �� ���� ���� ��� ����

	token = strtok(tmp_url, seps);
	while(token != NULL)
	{
		lstrcpy(buf, token);

		/* Get next token: */
		token = strtok(NULL, seps);
	}

	if(lstrlen(buf) > 19) lstrcpy(Ext, "error");
	else lstrcpy(Ext, buf);
	return Ext;
}