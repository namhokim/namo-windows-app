#include "DlgProc.h"

class SampleWLDlg : public WLDialogBox
{
	LRESULT WTDlgProc(HWND hdlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg) {
			HANDLE_MSG(hdlg, WM_COMMAND, OnCommand);
			HANDLE_MSG(hdlg, WM_SETCURSOR, OnSetCursor);
		}
		return __super::WLDlgProc(hdlg, uMsg, wParam, lParam);
	}

	void OnCommand(HWND hdlg, int id, HWND hwndCtl, UINT codeNotify)
	{
		switch (id) {
		case IDCANCEL:
			MessageBox(hdlg, TEXT("Bye"), TEXT("Title"), MB_OK);
			EndDialog(hdlg, 1);
			break;
		}
	}

	BOOL OnSetCursor(HWND hdlg,	HWND hwndCursor, UINT codeHitTest, UINT msg)
	{
		if (codeHitTest == HTCAPTION) {
			SetCursor(LoadCursor(NULL, IDC_SIZEALL));
			return TRUE;
		}
		return FORWARD_WM_SETCURSOR(hdlg, hwndCursor, codeHitTest, msg, __super::WLDlgProc);
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nShowCmd)
{
	SampleWLDlg dlg;
	dlg.DoModal(hInstance, MAKEINTRESOURCE(1), NULL);

	return 0;
}