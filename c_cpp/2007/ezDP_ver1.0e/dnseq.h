/*
 *	solution explore : wininet.lib;yld.lib
 */
#include <wininet.h>

#define MAX_URL	2000
#define	MAX_FILE_SIZE	65536	// 너무크면 에러날 수 있다.(MAXIMUM:1019999>65,535)

#define	KILO	1024
#define	MEGA	1048576
#define	GIGA	1073741824
#define	TERA	1099511627776

float haftOver(float number, unsigned int Pointposition);

void DisplaybyByte2(DWORD ReceiveByte, char *Output);
float DisplaybyByte(DWORD ReceiveByte, char *Output);
int HTTPDown(char *URL, char *Path);
int DownloadFile(char *url, char *dest_path);
int IsFreeDiskSpace(char targetDrive);
int GetfromURL(char *url);
void ExecuteDownload(HWND handle, char *URLListFilename);
