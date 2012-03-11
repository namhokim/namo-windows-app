#include "Analyzer.h"
#include "MakeAbsUrl.h"
#include "HttpDownload.h"
#include "DataMan.h"
extern HWND hDlgMain;
extern BOOL InetConnect;
extern BOOL DiskState;

#define	INIT	0
//	void GetTagInfo2(3)
//
//	���� : ���ڷ� ���� url���� tagname�� Ư�� �Ӽ����� ����Ѵ�.
//	��� : 
//	���ڵ� :
//	i	url - ������ url.
//	i	tagname - ��� tag
//	i	attr	- ��� tag���� Ư�� �Ӽ�
//	��ȯ�� :
//		����(���� ���� ǥ�÷� �ٲܰ�!)
void GetTagInfo2(char *url, char* tagname, char* attr)
{
	FILE *fp;
	char pre_c, c;
	unsigned long line_count=1;
	char IsFire=FALSE, IsMatch=FALSE, IsAttrMatch=FALSE;
	int TagSize = lstrlen(tagname);
	int AttrSize = lstrlen(attr);
	int TagLevel = INIT, CheckTagLevel;
	int CompareCount;
	int SubmatchinTag_c, OutofAttrBound;
	int StateInAttr;
	char AttrData[MAX_PATH+MAX_PATH],AttrData2[MAX_PATH+MAX_PATH];
#ifdef	DEBUG
	char tmp[256];
#endif
	int err;
	if((err=HTTPDown(url, "tmp_file"))!=1) {	// �������� ���� ��� ����
//		if ((fp=fopen(url, "rt"))==NULL) {
#ifdef	DEBUG
			wsprintf(tmp, "���� ���� �Ұ���!(%d)", err);
			if(InetConnect) MessageBox(hDlgMain, tmp, "�˸�", MB_OK);
			InetConnect = FALSE;
#endif
			return;
//		}
	}
	else {
		InetConnect = TRUE;	// ���� ���ͳ� ���� ���´� ��ȣ�ϴ�.
		if ((fp=fopen("tmp_file", "rt"))==NULL) {
			if(DiskState) MessageBox(hDlgMain, "���� ���� ����!", "�˸�", MB_OK);
			DiskState = FALSE;
			return;
		}
		else {
			DiskState = TRUE;
		}
	}

	pre_c = '0';

	#ifdef DEBUG
		printf("Current target tag name is %s.\nsize of name is %d\n", tagname, TagSize);
	#endif

	// File scanning and Parsing
	while((c=fgetc(fp))!=EOF) {
		if(c == '\n') line_count++;

		if(c == '>') {
			TagLevel--;
			#ifdef DEBUG
				printf("tag lv: %d\n", TagLevel);
			#endif
			if(IsMatch) {
				MakeAbsUrl(url, AttrData, AttrData2);
//				printf("%s\n", AttrData2);	// printf("\n");	###EOATTR CASE 3/3
				DataMan(AttrData2, url);
				IsMatch=FALSE;
			}
		}
		else if(c == '<') {
			TagLevel++;
			IsFire=TRUE;
			#ifdef DEBUG
				printf("tag lv: %d\n", TagLevel);
			#endif
			CompareCount = 0;
		}
		else if(IsFire) {
			CompareCount++;
			if(CompareCount>TagSize) {
				IsFire = FALSE;
			}
			else {
				if(charicmp(c,tagname[CompareCount-1])==FALSE) {	// �빮�� �����
					IsFire = FALSE;
					#ifdef DEBUG
						printf("%c %c", c, tagname[CompareCount-1]);
					#endif
				}
				else {
					if(CompareCount==TagSize) {	// tag�� ��ġ�Ǵ� ��Ȳ������
						IsMatch = TRUE;
						CheckTagLevel = TagLevel;
						SubmatchinTag_c = 0;
						IsAttrMatch = FALSE;
						OutofAttrBound = FALSE;
						IsFire = FALSE;
						AttrData[0]='\0';
						if((c=fgetc(fp))==EOF) break;	// <tagname_ <=tagname�������� white space�� �־�� �Ѵ�.
						#ifdef DEBUG
							printf("%c <===\n", c);
						#endif
						if(c!=' ' && c!='\t' && c!='\n') IsMatch = FALSE;
#define STATE_WS_B_EQ	0
#define STATE_WS_A_EQ	1
#define STATE_WS_CH		2
#define STATE_WS_CH_COMMA	3
						StateInAttr = STATE_WS_B_EQ;
					}
				}
			}
		}
        else if(IsMatch && CheckTagLevel==TagLevel) {
			// ToDo: ��ġ�Ǿ��� �� �� ���� �����ϴ�.
			//
			// ���� 1 - attribute name ã�� ��
			if(!OutofAttrBound) {
				if(!IsAttrMatch) {
					if(charicmp(c,attr[SubmatchinTag_c])==TRUE) {	// �빮�� �����
						SubmatchinTag_c++;
						if(SubmatchinTag_c >= AttrSize) IsAttrMatch=TRUE;	// �Ӽ��̸��� ��ġ�� ����
					}
					else SubmatchinTag_c=0;	// �ٽ� �Ӽ� �̸��� ù ���ڿ� ��
				}
				// ���� 2 - attribute ���
				else {
					// ó�� '='�� ���� ������ white space�� ����
					if(StateInAttr==STATE_WS_B_EQ) {
						if(c == '=') StateInAttr=STATE_WS_A_EQ;
						else if(c != ' ' && c != '\n' && c != '\t')	IsMatch = FALSE;	// ���� (�Ӽ� �������� '='�� �ݵ�� ���;� �Ѵ�)
					}
					// '='�� ������ white space�� ����
					else if(StateInAttr==STATE_WS_A_EQ) {
						if(c != ' ' && c != '\n' && c != '\t') {
							if(c == '\'' || c == '\"') StateInAttr = STATE_WS_CH_COMMA;
							else {
								wsprintf(AttrData, "%s%c", AttrData, c);	//	printf("%c", c);
								StateInAttr = STATE_WS_CH;
							}
						}
					}
					// '='�� ������ non white space�� ��� ���
					else if(StateInAttr==STATE_WS_CH) {
					// '='�� ������ non white space�� ������ �ٽ� white space�� ������ value��(OutofAttrBound=TRUE)
						if(c == ' ' || c == '\t') {
							MakeAbsUrl(url, AttrData, AttrData2);
//							printf("%s\n", AttrData2);	// printf("\n");	###EOATTR CASE 1/3
							DataMan(AttrData2, url);
							IsMatch = FALSE;
						}
						else wsprintf(AttrData, "%s%c", AttrData, c);	//	printf("%c", c);
					}
					// '�� "�� �ѷ� �׿��� ��� space�� ���� %20�ٲ ���, ��¦�� ���� ���� OutofAttrBound=TRUE�� ��.
					else if(StateInAttr==STATE_WS_CH_COMMA) {
						if(c == '\'' || c == '\"') {
							MakeAbsUrl(url, AttrData, AttrData2);
//							printf("%s\n", AttrData2);	// printf("\n");	###EOATTR CASE 2/3
							DataMan(AttrData2, url);
							IsMatch = FALSE;
						}
						else if(c == ' ') wsprintf(AttrData, "%s%%20", AttrData);	//	printf("%20");
						else wsprintf(AttrData, "%s%c", AttrData, c);	//	printf("%c", c);
					}
				}
			}
			
			//
			// end of Todo.
		}
		pre_c = c;
	}
	// close file pointer
	fclose(fp);
}
