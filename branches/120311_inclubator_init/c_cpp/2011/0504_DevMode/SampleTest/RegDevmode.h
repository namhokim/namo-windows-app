#pragma once

#define	MAX_EMAIL_LENGTH	256		// ATLSMTP_MAX_SERVER_NAME_LENGTH in <atlsmtputil.h> or RFC3696 and RFC5321
#define	MAX_URL_LENGTH		2084	// INTERNET_MAX_URL_LENGTH in <WinInet.h>

typedef struct _ltEnvVarsA
{
	unsigned long autobackup;	// UNIT : secound
	unsigned long autoupdate;	// UNIT : secound
	char rcpt[MAX_EMAIL_LENGTH];
	char backup_base_url[MAX_URL_LENGTH];
	char reqrelease_jsp[MAX_URL_LENGTH];
} LtEnvVarsA, *LPLtEnvVarsA;

typedef struct _ltEnvVarsW
{
	unsigned long autobackup;	// UNIT : secound
	unsigned long autoupdate;	// UNIT : secound
	wchar_t rcpt[MAX_EMAIL_LENGTH];
	wchar_t backup_base_url[MAX_URL_LENGTH];
	wchar_t reqrelease_jsp[MAX_URL_LENGTH];
} LtEnvVarsW, *LPLtEnvVarsW;

#ifdef UNICODE
#define LtEnvVars  LtEnvVarsW
#define LPLtEnvVars  LPLtEnvVarsW
#else
#define LtEnvVars  LtEnvVarsA
#define LPLtEnvVars  LPLtEnvVarsA
#endif // !UNICODE


bool GetDevmodeW(wchar_t* app, LPLtEnvVarsW data);
bool GetDevmodeA(char* app, LPLtEnvVarsA data);

#ifdef UNICODE
#define GetDevmode  GetDevmodeW
#else
#define GetDevmode  GetDevmodeA
#endif // !UNICODE


bool SetDevmodeW(wchar_t* app, const LPLtEnvVarsW data);
bool SetDevmodeA(char* app, const LPLtEnvVarsA data);

/*
bool GetDevmode(��ƿ� �Ķ����);	// ����(false)�� �� ������ ���, RegQueryEx
bool SetDevmode(������ �Ķ����);	// ���н� ���Ѻ��� ���ɼ�(GetLastError), RegWriteValueEx
void DelDevmode();					// ���н� ���Ѻ��� ���ɼ�(GetLastError), RegDeleteKeyEx
 */