#pragma once

#ifndef	_WL_DIGLOG_BOX_
#define	_WL_DIGLOG_BOX_

#include <Windows.h>
#include <WindowsX.h>	// DefDlgProcEx, CheckDefDlgRecursion, SetDlgMsgResult

class WLDialogBox
{
public:
	virtual LRESULT WLDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefDlgProcEx(hDlg, uMsg, wParam, lParam, &m_fRecursing);
	}

	INT_PTR DoModal(HINSTANCE hinst, LPCTSTR pszTemplate, HWND hwndParent)
	{
		m_fRecursing = FALSE;
		return DialogBoxParam(hinst, pszTemplate, hwndParent, s_DlgProc, (LPARAM)this);
	}

private:
	static INT_PTR CALLBACK s_DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_INITDIALOG) {
			::SetWindowLongPtr(hDlg, DWLP_USER, lParam);
		}

		WLDialogBox *self =
			reinterpret_cast<WLDialogBox*>( GetWindowLongPtr(hDlg, DWLP_USER) );
		if (!self) {
			return FALSE;
		}

		CheckDefDlgRecursion(&self->m_fRecursing);
			
		return SetDlgMsgResult(hDlg, uMsg, self->WLDlgProc(hDlg, uMsg, wParam, lParam));
	}

private:
	BOOL m_fRecursing;
};

typedef WLDialogBox super;

#endif