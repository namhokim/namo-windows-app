#include <windows.h>
#include "WindowPosition.h"

BOOL SetWindowPosition(HWND hWnd)
{
	RECT rt;	// ���簢��
	if(GetAlternatePosition(hWnd,rt) != TRUE) return FALSE;	// ��ü ��ġ�� ����
	// ������ ��ġ ��ȯ
	return MoveWindow(hWnd, rt.left, rt.top, rt.right-rt.left,rt.bottom-rt.top, TRUE);
}

BOOL GetAlternatePosition(HWND hWnd, RECT &getRect)
{
	RECT desktop, currWnd;
	// �۾�ǥ������ ������ ȭ�鿵���� ũ�� ���ϱ�
	SystemParametersInfo(SPI_GETWORKAREA, 0, &desktop, 0);
	// ȭ�� ��ǥ�� �������� �� ������ �������ϱ�
	GetWindowRect(hWnd,&currWnd);

	// �ð������� ��� ��ġ�� �޾ƿ�
	return GetVisuallyCentered(desktop, currWnd, getRect);
}

// �ð��� �߽��� ��ġ�� ����
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
