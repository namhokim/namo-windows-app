#include <windows.h>
#include <stdio.h>
#include <wininet.h>

#define MAX_URL	2000
#define	MAX_FILE_SIZE	65536	// �ʹ�ũ�� ������ �� �ִ�.(MAXIMUM:1019999>65,535)

int HTTPDown(char *URL, char *Path);
int DownloadFile(char *url, char *dest_path);
int IsFreeDiskSpace(char targetDrive);
int GetfromURL(char *url);
