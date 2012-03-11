#include "HttpDownload.h"

long total_url;
long fail_url;
char target_dir[MAX_PATH];

// �ݿø��ϴ� �Լ�: haftOver
// arg:
//	[float]number - �Ҽ�
//	[unsigned int]Pointposition - �Ҽ��� �Ʒ� n�ڸ����� �ݿø� ������ ��ġ
// return value: [float] �ݿø��� ��
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


// URL���� ������ �ٿ�ε�޾� Path ���Ϸ� �����Ѵ�.
// return value:
//				1 : ����
//				0 : url open ����(������ ���� ���� ��ȯ��, ���� ������ ������ 404�����޽��� �ٿ�ε��)
//				-1: ���ͳ� ���� ����
int HTTPDown(char *URL, char *Path)
{
	HINTERNET hInternet, hURL;
	HANDLE hFile;
	DWORD Size;
	DWORD dwRead,dwWritten,Recv=0;
	char buf[MAX_FILE_SIZE];
//	char nByte[10];

	// ���ͳ� ����
	hInternet=InternetOpen("HTTPTEST", INTERNET_OPEN_TYPE_PRECONFIG,
		NULL, NULL, 0);
	if (hInternet == NULL) {
		return -1;
	}

	// URL�� ����
	hURL=InternetOpenUrl(hInternet,URL,NULL,0,INTERNET_FLAG_RELOAD,0);
	if (hURL==NULL) {
		InternetCloseHandle(hInternet);
		return 0;
	}

	// ���� ������ �����Ѵ�.
	hFile=CreateFile(Path,GENERIC_WRITE,0,NULL,
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	do {
		// �ٿ�ε�޾� ���� ���Ͽ� ����Ѵ�.
		InternetQueryDataAvailable(hURL,&Size,0,0);
		InternetReadFile(hURL,buf,Size,&dwRead);
		WriteFile(hFile,buf,dwRead,&dwWritten,NULL);

		// �ٿ�ε� ���� ���� �޽���
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
		lstrcpy(filename, token);	// �Ǹ������� ����Ű�� ����..
		/* Get next token: */
		token = strtok(NULL, seps);
	}

	wsprintf(dir_file, "%s\\%s", dest_path, filename);
	
	err_msg = HTTPDown(url, dir_file);
//	printf("[%s] %s\n", url, dir_file);
	return err_msg;
}

// ������ ����̺��� �뷮�� �����Ѵ�.
// return value == 0: �뷮�� ����.
int IsFreeDiskSpace(char targetDrive)
{
	char Drive[MAX_PATH];
	ULARGE_INTEGER Avail, Total;
	wsprintf(Drive, "%c:\\", targetDrive);

	// ��ũ�� �뷮�� �ִ��� Ȯ���Ѵ�.
	GetDiskFreeSpaceEx(Drive, &Avail, &Total, NULL);
	return (int)Avail.QuadPart;
}

int GetfromURL(char *url)
{
	int err_msg;

	// ��ũ�� �뷮�� �ִ��� Ȯ���Ѵ�.
	if(!IsFreeDiskSpace(target_dir[0])) {
		printf("Disk is FULL!!!\n");
		exit(-3);
	}

	printf("%s :\ndownload now...\r", url);
	err_msg = DownloadFile(url, target_dir);	// �ٿ�ε带 �޴´�.
	// �ٽ� �ϵ� �뷮 üũ
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