#include "MakeAbsUrl.h"
#include <stdio.h>

//	BOOL IsAbsUrl(1)
//
//	설명 : 인자로 받은 url이 절대주소인지 여부를 반환한다.
//	방법 : 맨 앞의 프로토콜로만 판단한다.(http와 file일 경우만 참)
//	인자들 :
//	i	url - 검사할 url.
//	반환값 :
//		TRUE - 절대 주소이다.
//		FALSE - 절대 주소가 아니다.
BOOL IsAbsUrl(char *url)
{
	char *token;
	char seps[]   = ":/\t\n";
	char tmp_char[MAX_PATH+MAX_PATH];	// 원본데이터가 손실되는 걸 방지 위한 백업 변수
	int state=STATE_PROC;

	lstrcpy(tmp_char, url);	// strtok을 사용하면 원본데이터가 손실된다.

	if(lstrlen(tmp_char)==0) return FALSE;
	token = strtok(tmp_char, seps);
	while(token != NULL)
	{
		if(state==STATE_PROC) {
			if(!lstrcmp("http", token) || !lstrcmp("file", token)
				 || !lstrcmp("mailto", token) || !lstrcmp("ftp", token)
				 || !lstrcmp("javascript", token)) {
				state=STATE_OTHER;
			}
		}
		/* Get next token: */
		token = strtok(NULL, seps );
	}
	if(state!=STATE_PROC) return TRUE;
	else return FALSE;
}

//	BOOL DivUrl(5)
//
//	설명 : 인자로 받은 url을 평가하여 여러 가지로 나눈다.
//	방법 : '/'와 상대적 위치를 고려해서 나눈다.
//	인자들 :
//	i	target - 대상 url.
//	o	proc - 프로토콜
//	o	host - 호스트 이름
//	o	path - 경로
//	o	file - 파일명
//	반환값 :
//		TRUE - divide에 성공.
//		FALSE - divide에 실패.
// 주의 : target은 full url이어야 한다.
BOOL DivUrl(char* target, char *proc, char *host, char *path, char *file)
{
	char *token;
	char tmp_que[MAX_PATH];
	char tmp_char[MAX_PATH+MAX_PATH];	// 원본데이터가 손실되는 걸 방지 위한 백업 변수
	const char seps[]   = "/\t\n";
	int state=STATE_PROC;

	// Current Url을 분석하여 프로토콜, 호스트 이름, 경로, 파일명으로 나눈다.

	lstrcpy(tmp_char, target);	// strtok을 사용하면 원본데이터가 손실된다.
	

	// 초기화
	proc[0] = host[0] = path[0] = file[0] = tmp_que[0]= '\0';

	// target이 null일 경우
	if(lstrlen(tmp_char)==0) return FALSE;

	// set token
	token = strtok(tmp_char, seps);
	while( token != NULL )
	{
		if(state==STATE_PROC) {
			#ifdef DEBUG
			printf("STATE PROC\n");
			#endif
			if((lstrcmp("http:", token))==0) {
				lstrcpy(proc, token);
				lstrcat(proc, "//");
				state=STATE_HOST;
			}
			else if((lstrcmp("file:", token))==0) {
				lstrcpy(proc, token);
				lstrcat(proc, "///");
				state=STATE_HOST;
			}
		}
		else if(state==STATE_HOST) {
			#ifdef DEBUG
			printf("STATE HOST\n");
			#endif
			lstrcpy(host, token);
			state=STATE_PATH_OR_FILE;
		}
		else if(state==STATE_PATH_OR_FILE) {
			#ifdef DEBUG
			printf("STATE PATH_OR_FILE\n");
			#endif
			lstrcpy(tmp_que, token);
			state=STATE_PATH;
		}
		else if(state==STATE_PATH) {
			#ifdef DEBUG
			printf("STATE PATH\n");
			#endif
			if(lstrlen(token)) {
				lstrcat(path, "/");
				lstrcat(path, tmp_que);
				lstrcpy(tmp_que, token);
			}
		}
		/* Get next token: */
		token = strtok(NULL, seps);
	}
	lstrcat(path, "/");
	lstrcpy(file, tmp_que);

	return TRUE;
}

//	BOOL MakeAbsUrl(3)
//
//	설명 : 인자로 받은 url을 평가하여 절대 url을 만든다.
//	방법 : 현재의 url과 대상 url을 평가하여 믹스하여 결과 url을 만듬.
//	인자들 :
//	i	CurrentUrl - 현재 URL
//	i	TargetUrl - 바꿀 대상 URL
//	o	OutputUrl -  출력 URL
//	반환값 :
//		TRUE - 변환 과정이 있었다.
//		FALSE - 변환 과정이 없었다.
BOOL MakeAbsUrl(char* CurrentUrl, char* TargetUrl, char* OutputUrl)
{
	char Proc[10], Host[MAX_PATH], Path[MAX_PATH], File[MAX_PATH];

	// Current Url을 분석하여 프로토콜, 호스트 이름, 경로, 파일명으로 나눈다.
	DivUrl(CurrentUrl, Proc, Host, Path, File);

	#ifdef DEBUG
	printf("p: %s, h: %s p: %s, f: %s\n", Proc, Host, Path, File);
	#endif

//#define	DEBUG	1
	OutputUrl[0]='\0';
	// Target Url을 분석하여 다음 상황으로 나눈다.
	// 1. 처리할 필요없다.(원래 절대 Url이다.)
	if(IsAbsUrl(TargetUrl)) {
		#ifdef DEBUG
		printf("case #1\n");
		#endif
		lstrcpy(OutputUrl, TargetUrl);
		return FALSE;
	}
	else {
		// 2. /~/파일명 : Proc + Host + TargetUrl
		if(TargetUrl[0]=='/') {
			#ifdef DEBUG
			printf("case #2\n");
			#endif
			wsprintf(OutputUrl, "%s%s%s", Proc, Host, TargetUrl);
			return TRUE;
		}
		// 3. 파일명 : Proc + Host + Path + TargetUrl
		// 4. ~/파일명 : Proc + Host + Path + TargetUrl
		else if(/*TargetUrl[0]!='/' && */TargetUrl[0]!='.') {
			#ifdef DEBUG
			printf("case #3,4\n");
			#endif
			wsprintf(OutputUrl, "%s%s%s%s", Proc, Host, Path, TargetUrl);
			return TRUE;
		}
		// 5. ./~/파일명 : Proc + Host + Path + TargetUrl
		// 6. ../~/파일명 : 한칸 아래의 경로에서 파일이름을 붙임
		// 7. . 나 ..가 여러번 사용 시
		else {
			wsprintf(OutputUrl, "%s%s%s", Proc, Host, DotPath(Path, TargetUrl, File));
		}
	return TRUE;
	}

	// 8. javascript같은 스크립트를 호출해서 경로 리턴시 처리 x.
	return FALSE;
}

char *DotPath(char *cpath, char *rpath, char *out)
{
	int clv, lv;
	char new_cpath[MAX_PATH+MAX_PATH], new_rpath[MAX_PATH+MAX_PATH];
	char *token;
	char seps[] = "/\t\n";
	char tmp_char[MAX_PATH+MAX_PATH];	// 원본데이터가 손실되는 걸 방지 위한 백업 변수
	char cpath_array[MAX_PATH][MAX_PATH+MAX_PATH];

	lstrcpy(tmp_char, cpath);	// strtok을 사용하면 원본데이터가 손실된다.
	// set token
	token = strtok(tmp_char, seps);
	clv = 0;
	// cpath의 token의 개수를 구한다.
	while(token != NULL) {
		lstrcpy(cpath_array[clv],token);
		clv++;

		/* Get next token: */
		token = strtok(NULL, seps);
	}

	lstrcpy(tmp_char, rpath);	// strtok을 사용하면 원본데이터가 손실된다.
	// set token
	token = strtok(tmp_char, seps);
	lv = 0;
	new_rpath[0]='\0';
	// rpath의 token중 '..'의 개수를 구한다.
	while(token != NULL) {
		if(lstrcmp(token,"..")==0) lv++;
		else if(lstrcmp(token,".")!=0) {	// 그외의 토큰들은 new rpath로 만든다.
			strcat(new_rpath, "/");
			strcat(new_rpath, token);
		}

		/* Get next token: */
		token = strtok(NULL, seps);
	}

	// 새로운 cpath생성
	// cpath token loop하며 누적한다 by clv - lv 번
	new_cpath[0]='\0';
	if(clv>lv) {
		for(int i=0; i<clv-lv ; i++) {
			lstrcat(new_cpath, "/");
			lstrcat(new_cpath, cpath_array[i]);
		}
	}

	// out <= new_cpath + new_rpath;
	wsprintf(out, "%s%s", new_cpath, new_rpath);

	return out;
}
