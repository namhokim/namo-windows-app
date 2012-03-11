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
//	설명 : 인자로 받은 url에서 tagname의 특정 속성값을 출력한다.
//	방법 : 
//	인자들 :
//	i	url - 조사할 url.
//	i	tagname - 대상 tag
//	i	attr	- 대상 tag에서 특정 속성
//	반환값 :
//		없음(에러 상태 표시로 바꿀것!)
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
	if((err=HTTPDown(url, "tmp_file"))!=1) {	// 성공했을 때만 계속 수행
//		if ((fp=fopen(url, "rt"))==NULL) {
#ifdef	DEBUG
			wsprintf(tmp, "서버 연결 불가능!(%d)", err);
			if(InetConnect) MessageBox(hDlgMain, tmp, "알림", MB_OK);
			InetConnect = FALSE;
#endif
			return;
//		}
	}
	else {
		InetConnect = TRUE;	// 현재 인터넷 연결 상태는 양호하다.
		if ((fp=fopen("tmp_file", "rt"))==NULL) {
			if(DiskState) MessageBox(hDlgMain, "파일 오픈 에러!", "알림", MB_OK);
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
				if(charicmp(c,tagname[CompareCount-1])==FALSE) {	// 대문자 고려비교
					IsFire = FALSE;
					#ifdef DEBUG
						printf("%c %c", c, tagname[CompareCount-1]);
					#endif
				}
				else {
					if(CompareCount==TagSize) {	// tag가 매치되는 상황에서는
						IsMatch = TRUE;
						CheckTagLevel = TagLevel;
						SubmatchinTag_c = 0;
						IsAttrMatch = FALSE;
						OutofAttrBound = FALSE;
						IsFire = FALSE;
						AttrData[0]='\0';
						if((c=fgetc(fp))==EOF) break;	// <tagname_ <=tagname다음에는 white space가 있어야 한다.
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
			// ToDo: 매치되었을 때 할 일을 적습니다.
			//
			// 상태 1 - attribute name 찾는 중
			if(!OutofAttrBound) {
				if(!IsAttrMatch) {
					if(charicmp(c,attr[SubmatchinTag_c])==TRUE) {	// 대문자 고려비교
						SubmatchinTag_c++;
						if(SubmatchinTag_c >= AttrSize) IsAttrMatch=TRUE;	// 속성이름과 일치된 순간
					}
					else SubmatchinTag_c=0;	// 다시 속성 이름의 첫 글자와 비교
				}
				// 상태 2 - attribute 출력
				else {
					// 처음 '='를 만날 때까지 white space는 무시
					if(StateInAttr==STATE_WS_B_EQ) {
						if(c == '=') StateInAttr=STATE_WS_A_EQ;
						else if(c != ' ' && c != '\n' && c != '\t')	IsMatch = FALSE;	// 에러 (속성 다음에는 '='가 반드시 나와야 한다)
					}
					// '='를 만나고 white space는 무시
					else if(StateInAttr==STATE_WS_A_EQ) {
						if(c != ' ' && c != '\n' && c != '\t') {
							if(c == '\'' || c == '\"') StateInAttr = STATE_WS_CH_COMMA;
							else {
								wsprintf(AttrData, "%s%c", AttrData, c);	//	printf("%c", c);
								StateInAttr = STATE_WS_CH;
							}
						}
					}
					// '='를 만나고 non white space일 경우 출력
					else if(StateInAttr==STATE_WS_CH) {
					// '='를 만나고 non white space을 지나고 다시 white space를 만나면 value끝(OutofAttrBound=TRUE)
						if(c == ' ' || c == '\t') {
							MakeAbsUrl(url, AttrData, AttrData2);
//							printf("%s\n", AttrData2);	// printf("\n");	###EOATTR CASE 1/3
							DataMan(AttrData2, url);
							IsMatch = FALSE;
						}
						else wsprintf(AttrData, "%s%c", AttrData, c);	//	printf("%c", c);
					}
					// '나 "로 둘러 쌓였을 경우 space일 경우는 %20바뀌서 출력, 원짝을 만날 때가 OutofAttrBound=TRUE가 됨.
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
