#include "MakeAbsUrl.h"
#include <stdio.h>

//	BOOL IsAbsUrl(1)
//
//	���� : ���ڷ� ���� url�� �����ּ����� ���θ� ��ȯ�Ѵ�.
//	��� : �� ���� �������ݷθ� �Ǵ��Ѵ�.(http�� file�� ��츸 ��)
//	���ڵ� :
//	i	url - �˻��� url.
//	��ȯ�� :
//		TRUE - ���� �ּ��̴�.
//		FALSE - ���� �ּҰ� �ƴϴ�.
BOOL IsAbsUrl(char *url)
{
	char *token;
	char seps[]   = ":/\t\n";
	char tmp_char[MAX_PATH+MAX_PATH];	// ���������Ͱ� �սǵǴ� �� ���� ���� ��� ����
	int state=STATE_PROC;

	lstrcpy(tmp_char, url);	// strtok�� ����ϸ� ���������Ͱ� �սǵȴ�.

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
//	���� : ���ڷ� ���� url�� ���Ͽ� ���� ������ ������.
//	��� : '/'�� ����� ��ġ�� ����ؼ� ������.
//	���ڵ� :
//	i	target - ��� url.
//	o	proc - ��������
//	o	host - ȣ��Ʈ �̸�
//	o	path - ���
//	o	file - ���ϸ�
//	��ȯ�� :
//		TRUE - divide�� ����.
//		FALSE - divide�� ����.
// ���� : target�� full url�̾�� �Ѵ�.
BOOL DivUrl(char* target, char *proc, char *host, char *path, char *file)
{
	char *token;
	char tmp_que[MAX_PATH];
	char tmp_char[MAX_PATH+MAX_PATH];	// ���������Ͱ� �սǵǴ� �� ���� ���� ��� ����
	const char seps[]   = "/\t\n";
	int state=STATE_PROC;

	// Current Url�� �м��Ͽ� ��������, ȣ��Ʈ �̸�, ���, ���ϸ����� ������.

	lstrcpy(tmp_char, target);	// strtok�� ����ϸ� ���������Ͱ� �սǵȴ�.
	

	// �ʱ�ȭ
	proc[0] = host[0] = path[0] = file[0] = tmp_que[0]= '\0';

	// target�� null�� ���
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
//	���� : ���ڷ� ���� url�� ���Ͽ� ���� url�� �����.
//	��� : ������ url�� ��� url�� ���Ͽ� �ͽ��Ͽ� ��� url�� ����.
//	���ڵ� :
//	i	CurrentUrl - ���� URL
//	i	TargetUrl - �ٲ� ��� URL
//	o	OutputUrl -  ��� URL
//	��ȯ�� :
//		TRUE - ��ȯ ������ �־���.
//		FALSE - ��ȯ ������ ������.
BOOL MakeAbsUrl(char* CurrentUrl, char* TargetUrl, char* OutputUrl)
{
	char Proc[10], Host[MAX_PATH], Path[MAX_PATH], File[MAX_PATH];

	// Current Url�� �м��Ͽ� ��������, ȣ��Ʈ �̸�, ���, ���ϸ����� ������.
	DivUrl(CurrentUrl, Proc, Host, Path, File);

	#ifdef DEBUG
	printf("p: %s, h: %s p: %s, f: %s\n", Proc, Host, Path, File);
	#endif

//#define	DEBUG	1
	OutputUrl[0]='\0';
	// Target Url�� �м��Ͽ� ���� ��Ȳ���� ������.
	// 1. ó���� �ʿ����.(���� ���� Url�̴�.)
	if(IsAbsUrl(TargetUrl)) {
		#ifdef DEBUG
		printf("case #1\n");
		#endif
		lstrcpy(OutputUrl, TargetUrl);
		return FALSE;
	}
	else {
		// 2. /~/���ϸ� : Proc + Host + TargetUrl
		if(TargetUrl[0]=='/') {
			#ifdef DEBUG
			printf("case #2\n");
			#endif
			wsprintf(OutputUrl, "%s%s%s", Proc, Host, TargetUrl);
			return TRUE;
		}
		// 3. ���ϸ� : Proc + Host + Path + TargetUrl
		// 4. ~/���ϸ� : Proc + Host + Path + TargetUrl
		else if(/*TargetUrl[0]!='/' && */TargetUrl[0]!='.') {
			#ifdef DEBUG
			printf("case #3,4\n");
			#endif
			wsprintf(OutputUrl, "%s%s%s%s", Proc, Host, Path, TargetUrl);
			return TRUE;
		}
		// 5. ./~/���ϸ� : Proc + Host + Path + TargetUrl
		// 6. ../~/���ϸ� : ��ĭ �Ʒ��� ��ο��� �����̸��� ����
		// 7. . �� ..�� ������ ��� ��
		else {
			wsprintf(OutputUrl, "%s%s%s", Proc, Host, DotPath(Path, TargetUrl, File));
		}
	return TRUE;
	}

	// 8. javascript���� ��ũ��Ʈ�� ȣ���ؼ� ��� ���Ͻ� ó�� x.
	return FALSE;
}

char *DotPath(char *cpath, char *rpath, char *out)
{
	int clv, lv;
	char new_cpath[MAX_PATH+MAX_PATH], new_rpath[MAX_PATH+MAX_PATH];
	char *token;
	char seps[] = "/\t\n";
	char tmp_char[MAX_PATH+MAX_PATH];	// ���������Ͱ� �սǵǴ� �� ���� ���� ��� ����
	char cpath_array[MAX_PATH][MAX_PATH+MAX_PATH];

	lstrcpy(tmp_char, cpath);	// strtok�� ����ϸ� ���������Ͱ� �սǵȴ�.
	// set token
	token = strtok(tmp_char, seps);
	clv = 0;
	// cpath�� token�� ������ ���Ѵ�.
	while(token != NULL) {
		lstrcpy(cpath_array[clv],token);
		clv++;

		/* Get next token: */
		token = strtok(NULL, seps);
	}

	lstrcpy(tmp_char, rpath);	// strtok�� ����ϸ� ���������Ͱ� �սǵȴ�.
	// set token
	token = strtok(tmp_char, seps);
	lv = 0;
	new_rpath[0]='\0';
	// rpath�� token�� '..'�� ������ ���Ѵ�.
	while(token != NULL) {
		if(lstrcmp(token,"..")==0) lv++;
		else if(lstrcmp(token,".")!=0) {	// �׿��� ��ū���� new rpath�� �����.
			strcat(new_rpath, "/");
			strcat(new_rpath, token);
		}

		/* Get next token: */
		token = strtok(NULL, seps);
	}

	// ���ο� cpath����
	// cpath token loop�ϸ� �����Ѵ� by clv - lv ��
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
