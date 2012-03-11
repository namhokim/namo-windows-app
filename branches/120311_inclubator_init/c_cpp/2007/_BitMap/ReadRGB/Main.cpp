#include <windows.h>
#include <CommCtrl.h>
#include "structRGB.h"
#include "resource.h"
#include "dib2ddb.h"

// global variations
BITMAPLINE	imgA[BIHEIGHT], imgB[BIHEIGHT];	// RGB�� ������ ����
char file1[MAX_PATH], file2[MAX_PATH];		// �Է¹��� ���ϸ�(�����ּ�)

// Function prototypes
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
HINSTANCE g_hInst;
HWND hDlgMain, hEdit1, hEdit2, hBitmap;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;
	
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);
	
	return 0;
}

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	OPENFILENAME OFN;
	char lpstrFile[MAX_PATH]="*.bmp";

	// for DDB
	HDC hdc;
	static HBITMAP hBit;
	// for drawing
	PAINTSTRUCT ps;

	switch(iMessage)
	{
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		InitCommonControls();		// ���� ��Ʈ�� �ʱ�ȭ
		hEdit1 = GetDlgItem(hDlg,IDC_EDIT1);		// IDC_EDIT1 �� �ڵ��� ���Ѵ�.
		hEdit2 = GetDlgItem(hDlg,IDC_EDIT2);		// IDC_EDIT1 �� �ڵ��� ���Ѵ�.
		hBitmap = GetDlgItem(hDlg,IDC_BITMAP);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg,0);
			return TRUE;
		case IDC_BUTTON1:
			// ���� ���� ���� ��ȭ���� ����
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner=hDlgMain;
			OFN.lpstrFilter="��Ʈ�� ����(*.bmp)\0";
			OFN.lpstrDefExt="bmp";
			OFN.lpstrFile=lpstrFile;
			OFN.nMaxFile=MAX_PATH;
			if (GetOpenFileName(&OFN)!=0) {
				wsprintf(file1,"%s ������ �����߽��ϴ�.",OFN.lpstrFile);
//				MessageBox(hDlgMain,file1,"���� ���� ����",MB_OK);
			}

			SetWindowText(hEdit1, OFN.lpstrFile);

			hdc=GetDC(hDlg);
			hBit=MakeDDBFromDIB(hdc,OFN.lpstrFile);
			
			ReleaseDC(hDlg,hdc);
			InvalidateRect(hBitmap, NULL, TRUE);

			return 0;
		case IDC_BUTTON2:
			// ���� ���� ���� ��ȭ���� ����
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner=hDlgMain;
			OFN.lpstrFilter="��Ʈ�� ����(*.bmp)\0";
			OFN.lpstrDefExt="bmp";
			OFN.lpstrFile=lpstrFile;
			OFN.nMaxFile=MAX_PATH;
			if (GetOpenFileName(&OFN)!=0) {
				wsprintf(file1,"%s ������ �����߽��ϴ�.",OFN.lpstrFile);
			}

			SetWindowText(hEdit2, OFN.lpstrFile);

			return 0;
		}
		return FALSE;
	case WM_PAINT:
		hdc=BeginPaint(hBitmap, &ps);
		DrawBitmap(hdc,0,100,hBit);
		EndPaint(hBitmap, &ps);
		return 0;
	}
	return FALSE;
}