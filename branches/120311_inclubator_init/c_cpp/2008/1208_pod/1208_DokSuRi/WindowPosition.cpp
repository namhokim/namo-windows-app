#include <windows.h>
#include "WindowPosition.h"

BOOL SetWindowPosition(HWND hWnd)
{
	RECT rt;	// 직사각형
	if(GetAlternatePosition(hWnd,rt) != TRUE) return FALSE;	// 대체 위치를 구함
	// 윈도우 위치 변환
	return MoveWindow(hWnd, rt.left, rt.top, rt.right-rt.left,rt.bottom-rt.top, TRUE);
}

BOOL GetAlternatePosition(HWND hWnd, RECT &getRect)
{
	RECT desktop, currWnd;
	// 작업표시줄을 제외한 화면영역의 크기 구하기
	SystemParametersInfo(SPI_GETWORKAREA, 0, &desktop, 0);
	// 화면 좌표를 기준으로 한 윈도우 영역구하기
	GetWindowRect(hWnd,&currWnd);

	// 시각적으로 가운데 위치를 받아옴
	return GetVisuallyCentered(desktop, currWnd, getRect);
}

// 시각적 중심의 위치를 구함
BOOL GetVisuallyCentered(RECT desktop, RECT currWnd, RECT &infoVC)
{
	int offsetX, offsetY;

	offsetX = (desktop.right-desktop.left-currWnd.right-currWnd.left)/2;	// 2X = X(left) : X(right)
	offsetY = (desktop.bottom-desktop.top-currWnd.bottom-currWnd.top)/3;	// 3Y = Y(top) : 2Y(bottom)

	infoVC.left=desktop.left+offsetX;
	infoVC.top=desktop.top+offsetY;
	infoVC.right=infoVC.left+(currWnd.right-currWnd.left);
	infoVC.bottom=infoVC.top+(currWnd.bottom-currWnd.top);

	return TRUE;
}
