#define IP_ADDRESS_MAX	16	// 12+3+1 {000.000.000.000}
#define MAC_ADDRESS_MAX	18	// 12+5+1 {00:00:00:00:00:00}
#define	IN_TRAY_LOCAL_NAME_MAX 256

typedef struct _SOME_INFO {
	char szIPAddr[IP_ADDRESS_MAX];
	wchar_t szLocalizedName[IN_TRAY_LOCAL_NAME_MAX];
	int status;
} SOME_INFO, *LPSOME_INFO;

#ifndef _WINDEF_
typedef int                 BOOL;

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif
#endif	// end of _WINDEF_

#ifndef	p_in_out
#define p_in_out		// just display parameter
#endif

BOOL
__stdcall							// WINAPI
GetSomeInfo(
	const char*,
	LPSOME_INFO,	// PRT_SYSTEM_INFO long pointer type
	unsigned int *
);