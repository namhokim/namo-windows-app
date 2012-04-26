#include "stdafx.h"
#include "tray.h"
#include "resource.h"

// message : NIM_ADD / NIM_DELETE
void SetTray(HWND hWindowHandle, HICON hIconHandle, int message, LPCTSTR tip)
{
	NOTIFYICONDATA nid;
	::ZeroMemory(&nid, sizeof(NOTIFYICONDATA));

	nid.cbSize=sizeof(NOTIFYICONDATA);
	nid.hWnd = hWindowHandle;
	nid.uID = 0;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.uCallbackMessage = TRAY_NOTIFY;
	if(hIconHandle) nid.hIcon = hIconHandle;

	if(tip) _tcscpy_s(nid.szTip, 128, tip);
	else _tcscpy_s(nid.szTip, 128, TEXT("SRT\nClick to Restore"));

	Shell_NotifyIcon(message, &nid);
	switch(message) {
		case NIM_ADD:
//			ShowWindow(hWindowHandle, SW_HIDE);
			ShowWindow(hWindowHandle, SW_MINIMIZE);
			ShowWindow(hWindowHandle, SW_HIDE);
			break;
		case NIM_DELETE:
			ShowWindow(hWindowHandle, SW_SHOW);	
			ShowWindow(hWindowHandle, SW_RESTORE);
			break;
	}
}