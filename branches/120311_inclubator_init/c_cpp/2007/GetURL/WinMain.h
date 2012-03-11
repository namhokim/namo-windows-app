#include <windows.h>
#include <commctrl.h>
#include "Analyzer.h"
#include "HttpDownload.h"
#include "MakeAbsUrl.h"

#define KW_ERR	-1
#define	KW_NOT	0
#define	KW_IMG	1
#define	KW_DOC	2
#define	KW_MUS	3
#define	KW_SLI	4
#define	KW_COM	5
#define KW_MAI	6
#define KW_LIK	100