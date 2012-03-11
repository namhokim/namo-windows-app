#include "HttpDownload.h"

long total_url;
long fail_url;
char target_dir[MAX_PATH];

// 반올림하는 함수: haftOver
// arg:
//	[float]number - 소수
//	[unsigned int]Pointposition - 소수점 아래 n자리에서 반올림 할지의 수치
// return value: [float] 반올림된 값
float haftOver(float number, unsigned int Pointposition) {
	float addNum, mul_div;
	unsigned int i;
	for(i=0, addNum=1, mul_div=(float)0.1; i<Pointposition; i++) {
		addNum *= (float)0.1;
		mul_div *= (float)10;
	}
	addNum *= 5;
	return (float)(((int)((number + addNum)*mul_div))/mul_div);
}

#define	KILO	1024
#define	MEGA	1048576
#define	GIGA	1073741824
#define	TERA	1099511627776
void DisplaybyByte2(DWORD ReceiveByte, char *Output)
{
	if(ReceiveByte < KILO)	wsprintf(Output, "%d B", ReceiveByte);
	else if(ReceiveByte < MEGA) wsprintf(Output, "%g KB", haftOver(ReceiveByte/(float)1024, 2));
	else if(ReceiveByte < GIGA) wsprintf(Output, "%g MB", haftOver(ReceiveByte/(float)1048576, 2));
	else if(ReceiveByte < TERA) wsprintf(Output, "%g GB", haftOver(ReceiveByte/(float)1073741824, 2));
	else wsprintf(Output, "%g TB", haftOver(ReceiveByte/(float)TERA, 2));
}

float DisplaybyByte(DWORD ReceiveByte, char *Output)
{
	if(ReceiveByte < KILO) {
		lstrcpy(Output, "B");
		return (float)ReceiveByte;
	}
	else if(ReceiveByte < MEGA) {
		lstrcpy(Output, "KB");
		return haftOver(ReceiveByte/(float)KILO, 2);
	}
	else if(ReceiveByte < GIGA) {
		lstrcpy(Output, "MB");
		return haftOver(ReceiveByte/(float)MEGA, 2);
	}
	else if(ReceiveByte < TERA)  {
		lstrcpy(Output, "GB");
		return haftOver(ReceiveByte/(float)GIGA, 2);
	}
	else  {
		lstrcpy(Output, "TB");
		return haftOver(ReceiveByte/(float)TERA, 2);
	}
}


// URL에서 파일을 다운로드받아 Path 파일로 저장한다.
// return value:
//				1 : 성공
//				0 : url open 실패(서버가 없을 때만 반환됨, 보통 서버가 있으면 404에러메시지 다운로드됨)
//				-1: 인터넷 연결 실패
int HTTPDown(char *URL, char *Path)
{
	HINTERNET hInternet, hURL;
	HANDLE hFile;
	DWORD Size;
	DWORD dwRead,dwWritten,Recv=0;
	char buf[MAX_FILE_SIZE];
//	char nByte[10];

	// 인터넷 연결
	hInternet=InternetOpen("HTTPTEST", INTERNET_OPEN_TYPE_PRECONFIG,
		NULL, NULL, 0);
	if (hInternet == NULL) {
		return -1;
	}

	// URL을 연다
	hURL=InternetOpenUrl(hInternet,URL,NULL,0,INTERNET_FLAG_RELOAD,0);
	if (hURL==NULL) {
		InternetCloseHandle(hInternet);
		return 0;
	}

	// 로컬 파일을 생성한다.
	hFile=CreateFile(Path,GENERIC_WRITE,0,NULL,
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	do {
		// 다운로드받아 로컬 파일에 기록한다.
		InternetQueryDataAvailable(hURL,&Size,0,0);
		InternetReadFile(hURL,buf,Size,&dwRead);
		WriteFile(hFile,buf,dwRead,&dwWritten,NULL);

		// 다운로드 받은 양을 메시지
		Recv += dwRead;
//		printf("%6g%s downloaded...\r", DisplaybyByte(Recv,nByte), nByte);
	} while (dwRead != 0);

	InternetCloseHandle(hURL);
	InternetCloseHandle(hInternet);
	CloseHandle(hFile);

	return 1;
}

int DownloadFile(char *url, char *dest_path)
{
	char url_backup[MAX_URL], filename[MAX_PATH], dir_file[MAX_PATH];
	char seps[] = "/";
	char *token;
	int err_msg;

	lstrcpy(url_backup, url);
	token = strtok(url_backup, seps);
	while(token != NULL)
	{
		/* While there are tokens in "string" */
		lstrcpy(filename, token);	// 맨마지막을 기억시키기 위해..
		/* Get next token: */
		token = strtok(NULL, seps);
	}

	wsprintf(dir_file, "%s\\%s", dest_path, filename);
	
	err_msg = HTTPDown(url, dir_file);
//	printf("[%s] %s\n", url, dir_file);
	return err_msg;
}

// 정해진 드라이브의 용량을 리턴한다.
// return value == 0: 용량이 없다.
int IsFreeDiskSpace(char targetDrive)
{
	char Drive[MAX_PATH];
	ULARGE_INTEGER Avail, Total;
	wsprintf(Drive, "%c:\\", targetDrive);

	// 디스크의 용량이 있는지 확인한다.
	GetDiskFreeSpaceEx(Drive, &Avail, &Total, NULL);
	return (int)Avail.QuadPart;
}

int GetfromURL(char *url)
{
	int err_msg;

	// 디스크의 용량이 있는지 확인한다.
	if(!IsFreeDiskSpace(target_dir[0])) {
		printf("Disk is FULL!!!\n");
		exit(-3);
	}

	printf("%s :\ndownload now...\r", url);
	err_msg = DownloadFile(url, target_dir);	// 다운로드를 받는다.
	// 다시 하드 용량 체크
	if(!IsFreeDiskSpace(target_dir[0])) {
		printf("Disk is FULL!!!\n");
		exit(-3);
	}
	else {
		if(err_msg==-1) {
			printf("# cannot connect Internet...\n");
			fail_url++;
			return -1;
		}
		else if(err_msg==1)	{
			printf("# download complete...\n");
			return 1;
		}
		else {
			printf("# download error...\n");
			// InsertFailURL(url);
			fail_url++;
			return 0;
		}
	}
}