#include "stdafx.h"
#include "WindowPosition.h"
#include "ShReg.h"

BOOL WindowPosition(HWND hWnd, LPCTSTR swName, int actionCode) {
	switch(actionCode) {
		case RESTORE:	// 창을 세팅
			return RestoreWindowPosition(hWnd, swName);

		case BACKUP:	// 창 위치를 기억
			return BackupWindowPosition(hWnd, swName);
	}
	return FALSE;
}

// 레지스트리에서 저장된 창의 값을 가져와서 적용
// 위치 조정
// 1. 저장된 위치정보가 있으면
// 2. 없으면 "Visually centered"위치로
BOOL RestoreWindowPosition(HWND hWnd, LPCTSTR swName) {
	RECT rt;	// 직사각형
	TCHAR subKey[512+512];		// [주의]: 크기 조정이 필요할 수도 있음
	int countError=0;			// 에러 판단을 위해(가정: 0 == ERROR_SUCCESS)

	wsprintf(subKey,TEXT("Software\\%s\\Settings\\Position"), swName);	// 레지스트리의 구체적 저장위치

	rt.left=SHRegReadInt(SHCU,subKey,TEXT("Left"),-1);	// 디폴트를 -1로 주었다.
	rt.top=SHRegReadInt(SHCU,subKey,TEXT("Top"),-1);
	rt.right=SHRegReadInt(SHCU,subKey,TEXT("Right"),-1);
	rt.bottom=SHRegReadInt(SHCU,subKey,TEXT("Bottom"),-1);

	// 같은 값들이 음수라면 에러!
	if((rt.left+rt.top+rt.right+rt.bottom)<0) {		// 레지스트리에서 정보획득에 에러
		GetAlternatePosition(hWnd,rt);	// 대체 위치를 구함
	}
	else {	// 정보획득은 문제없으나, 해상도 변경 등으로 인하여 화면에 윈도우가 보이지 않을경우
//		CheckValidPosition(hWnd, rt);
	}
	
	// 윈도우 위치 변환
	MoveWindow(hWnd,
		rt.left, rt.top, rt.right-rt.left,rt.bottom-rt.top, TRUE);
	return TRUE;
}

// 레지스트리에 현재 윈도우의 위치정보를 저장
BOOL BackupWindowPosition(HWND hWnd, LPCTSTR swName) {
	RECT rt;	// 직사각형
	TCHAR subKey[512+512];		// [주의]: 크기 조정이 필요할 수도 있음

	wsprintf(subKey, TEXT("Software\\%s\\Settings\\Position"), swName);	// 레지스트리의 구체적 저장위치

	GetWindowRect(hWnd, &rt);
	SHRegWriteInt(HKEY_CURRENT_USER,subKey,TEXT("Left"),rt.left);
	SHRegWriteInt(HKEY_CURRENT_USER,subKey,TEXT("Top"),rt.top);
	SHRegWriteInt(HKEY_CURRENT_USER,subKey,TEXT("Right"),rt.right);
	SHRegWriteInt(HKEY_CURRENT_USER,subKey,TEXT("Bottom"),rt.bottom);

	return TRUE;
}

BOOL GetAlternatePosition(HWND hWnd, RECT &getRect) {
	RECT desktop, currWnd;
	// 작업표시줄을 제외한 화면영역의 크기 구하기
	SystemParametersInfo(SPI_GETWORKAREA, 0, &desktop, 0);
	// 화면 좌표를 기준으로 한 윈도우 영역구하기
	GetWindowRect(hWnd,&currWnd);

	// 시각적으로 가운데 위치를 받아옴
	return GetVisuallyCentered(desktop, currWnd, getRect);

}

// 시각적 중심의 위치를 구함
BOOL GetVisuallyCentered(RECT desktop, RECT currWnd, RECT &infoVC) {
	int offsetX, offsetY;

	offsetX = (desktop.right-desktop.left-currWnd.right-currWnd.left)/2;	// 2X = X(left) : X(right)
	offsetY = (desktop.bottom-desktop.top-currWnd.bottom-currWnd.top)/3;	// 3Y = Y(top) : 2Y(bottom)

	infoVC.left=desktop.left+offsetX;
	infoVC.top=desktop.top+offsetY;
	infoVC.right=infoVC.left+(currWnd.right-currWnd.left);
	infoVC.bottom=infoVC.top+(currWnd.bottom-currWnd.top);

	return TRUE;
}
