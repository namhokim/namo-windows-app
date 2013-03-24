#include "stdafx.h"
#include "WindowPosition.h"
#include "ShReg.h"

BOOL WindowPosition(HWND hWnd, LPCTSTR swName, int actionCode) {
	switch(actionCode) {
		case RESTORE:	// â�� ����
			return RestoreWindowPosition(hWnd, swName);

		case BACKUP:	// â ��ġ�� ���
			return BackupWindowPosition(hWnd, swName);
	}
	return FALSE;
}

// ������Ʈ������ ����� â�� ���� �����ͼ� ����
// ��ġ ����
// 1. ����� ��ġ������ ������
// 2. ������ "Visually centered"��ġ��
BOOL RestoreWindowPosition(HWND hWnd, LPCTSTR swName) {
	RECT rt;	// ���簢��
	TCHAR subKey[512+512];		// [����]: ũ�� ������ �ʿ��� ���� ����
	int countError=0;			// ���� �Ǵ��� ����(����: 0 == ERROR_SUCCESS)

	wsprintf(subKey,TEXT("Software\\%s\\Settings\\Position"), swName);	// ������Ʈ���� ��ü�� ������ġ

	rt.left=SHRegReadInt(SHCU,subKey,TEXT("Left"),-1);	// ����Ʈ�� -1�� �־���.
	rt.top=SHRegReadInt(SHCU,subKey,TEXT("Top"),-1);
	rt.right=SHRegReadInt(SHCU,subKey,TEXT("Right"),-1);
	rt.bottom=SHRegReadInt(SHCU,subKey,TEXT("Bottom"),-1);

	// ���� ������ ������� ����!
	if((rt.left+rt.top+rt.right+rt.bottom)<0) {		// ������Ʈ������ ����ȹ�濡 ����
		GetAlternatePosition(hWnd,rt);	// ��ü ��ġ�� ����
	}
	else {	// ����ȹ���� ����������, �ػ� ���� ������ ���Ͽ� ȭ�鿡 �����찡 ������ �������
//		CheckValidPosition(hWnd, rt);
	}
	
	// ������ ��ġ ��ȯ
	MoveWindow(hWnd,
		rt.left, rt.top, rt.right-rt.left,rt.bottom-rt.top, TRUE);
	return TRUE;
}

// ������Ʈ���� ���� �������� ��ġ������ ����
BOOL BackupWindowPosition(HWND hWnd, LPCTSTR swName) {
	RECT rt;	// ���簢��
	TCHAR subKey[512+512];		// [����]: ũ�� ������ �ʿ��� ���� ����

	wsprintf(subKey, TEXT("Software\\%s\\Settings\\Position"), swName);	// ������Ʈ���� ��ü�� ������ġ

	GetWindowRect(hWnd, &rt);
	SHRegWriteInt(HKEY_CURRENT_USER,subKey,TEXT("Left"),rt.left);
	SHRegWriteInt(HKEY_CURRENT_USER,subKey,TEXT("Top"),rt.top);
	SHRegWriteInt(HKEY_CURRENT_USER,subKey,TEXT("Right"),rt.right);
	SHRegWriteInt(HKEY_CURRENT_USER,subKey,TEXT("Bottom"),rt.bottom);

	return TRUE;
}

BOOL GetAlternatePosition(HWND hWnd, RECT &getRect) {
	RECT desktop, currWnd;
	// �۾�ǥ������ ������ ȭ�鿵���� ũ�� ���ϱ�
	SystemParametersInfo(SPI_GETWORKAREA, 0, &desktop, 0);
	// ȭ�� ��ǥ�� �������� �� ������ �������ϱ�
	GetWindowRect(hWnd,&currWnd);

	// �ð������� ��� ��ġ�� �޾ƿ�
	return GetVisuallyCentered(desktop, currWnd, getRect);

}

// �ð��� �߽��� ��ġ�� ����
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
