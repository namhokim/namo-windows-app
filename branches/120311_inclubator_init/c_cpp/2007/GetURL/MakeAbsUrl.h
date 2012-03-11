#include <windows.h>
#define STATE_PROC	0
#define STATE_HOST	1
#define STATE_PATH_OR_FILE	2
#define STATE_PATH	3
#define STATE_OTHER	4

// function prototype
BOOL IsAbsUrl(char *url);
BOOL DivUrl(char* target, char *proc, char *host, char *path, char *file);
BOOL MakeAbsUrl(char* CurrentUrl, char* TargetUrl, char* OutputUrl);
char *DotPath(char *cpath, char *rpath, char *out);