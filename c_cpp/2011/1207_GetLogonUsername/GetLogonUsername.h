#pragma once

#ifndef	UNLEN
#define UNLEN       256                 // Maximum user name length
#endif

bool GetLogonUsernameA (
    char * lpBuffer,
    unsigned long* pcbBuffer
    );


bool GetLogonUsernameW (
    wchar_t * lpBuffer,
    unsigned long* pcbBuffer
    );

#ifdef UNICODE
#define GetLogonUsername  GetLogonUsernameW
#else
#define GetLogonUsername  GetLogonUsernameA
#endif // !UNICODE