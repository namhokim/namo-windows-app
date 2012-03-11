// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
#include <string>
#include <vector>
namespace std {
	typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;
}

using namespace std;
#include <StrSafe.h>

CMainDlg::CMainDlg()
:m_control_init_ok(FALSE), m_is_first(TRUE)
{}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// set dialog item
	m_list_env_var	= GetDlgItem(IDC_LIST_ENV_VAR);
	m_keyCap		= GetDlgItem(IDC_STATIC_KEY);
	m_valueCap		= GetDlgItem(IDC_STATIC_VALUE);
	m_key			= GetDlgItem(IDC_EDIT_KEY);
	m_value			= GetDlgItem(IDC_EDIT_VALUE);
	m_btnGet		= GetDlgItem(IDC_BUTTON_GET);
	m_btnSet		= GetDlgItem(IDC_BUTTON_SET);
	m_control_init_ok = TRUE;

	// Initialize ListView Control
	InitListView();

	// Focus
	m_key.SetFocus();

	return FALSE;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

VOID CMainDlg::InitListView()
{
	RECT rc;
	LONG lWidth, lNameWidth, lValueWidth;
	
	// get listview control width
	if(m_list_env_var.GetClientRect(&rc)) {
		lWidth = rc.right - rc.left;
		lValueWidth = lWidth/2;
		lNameWidth = lWidth - lValueWidth;
	} else {
		lNameWidth = lValueWidth = 100;
	}

	// insert column
	m_list_env_var.InsertColumn(0, TEXT("Name"), 0, lNameWidth);
	m_list_env_var.InsertColumn(1, TEXT("Value"), 0, lValueWidth);

	// data
	SetDataListView();
}

VOID CMainDlg::InitItemsSize(WORD wWidth, WORD wHeight)
{
	RECT rt;

	// Main Dlalog Init Size
	m_mainDlgPtr.x = wWidth;
	m_mainDlgPtr.y = wHeight;

	// Re-Sizing /////////////////////////
	// LIstView
	m_list_env_var.GetWindowRect(&rt);
	m_listViewPtr.x = rt.right - rt.left;
	m_listViewPtr.y = rt.bottom - rt.top;

	/// Re-positioning ////////////////////
	m_keyCap.GetWindowRect(&rt);
	m_keyCapPos.x =  rt.left;
	m_keyCapPos.y = rt.top;

	m_valueCap.GetWindowRect(&rt);
	m_valueCapPos.x =  rt.right - rt.left;
	m_valueCapPos.y = rt.bottom - rt.top;

	m_btnGet.GetWindowRect(&rt);
	m_btnGetPos.x =  rt.right - rt.left;
	m_btnGetPos.y = rt.bottom - rt.top;

	m_btnSet.GetWindowRect(&rt);
	m_btnSetPos.x =  rt.right - rt.left;
	m_btnSetPos.y = rt.bottom - rt.top;

	// Key Edit
	m_key.GetWindowRect(&m_keyRect);

	// Value Edit
	m_value.GetWindowRect(&m_valueRect);
}

VOID CMainDlg::SetDataListView()
{
	// ���� �����͸� ��� �����
	m_list_env_var.DeleteAllItems();
	int idx = m_list_env_var.GetItemCount();

	PTSTR pEnvBlock = ::GetEnvironmentStrings();
	TCHAR szName[MAX_PATH];
	TCHAR szValue[MAX_PATH];
	PTSTR pszCurrent = pEnvBlock;
	HRESULT hr = S_OK;
	PCTSTR pszPos = NULL;
	int current = 0;

	while (pszCurrent != NULL) {
		// ������ ���� �ǹ� ���� ���ڿ��� �ǳʶ�
		// "=::=::\"
		if (*pszCurrent != TEXT('=')) {
			// '='�� ã��
			pszPos = _tcschr(pszCurrent, TEXT('='));

			// ���� ù ��° ���ڿ�
			pszPos++;

			// ���� �̸� ����
			size_t cbNameLength =	// '='�� ����
				(size_t)pszPos - (size_t)pszCurrent - sizeof(TCHAR);
			hr = StringCbCopyN(szName, MAX_PATH, pszCurrent, cbNameLength);
			if (FAILED(hr)) {
				break;
			}

			// ������ NULL��¥������ �����Ͽ� ������ ���� ����
			hr = StringCchCopyN(szValue, MAX_PATH, pszPos, _tcslen(pszPos)+1);
			if (SUCCEEDED(hr)) {
				m_list_env_var.AddItem(idx, 0, szName);
				m_list_env_var.AddItem(idx++, 1, szValue);
			} else {	// ���� �߸�, ���ڿ� �߸����� Ȯ��
				if (hr == STRSAFE_E_INSUFFICIENT_BUFFER) {
					//MessageBox(TEXT("���ڿ� �߸��� �߻�"));
					m_list_env_var.AddItem(idx, 0, szName);
					m_list_env_var.AddItem(idx++, 1, szValue);
				} else {	// ����Ǿ �ȵ�
					MessageBox(TEXT("����"));
					break;
				}
			}
		} else {
			//m_list_env_var.AddItem(
		} /* if (*pszCurrent != TEXT('=')) */

		// next
		current++;

		// ���ڿ��� ������ �̵�
		while (*pszCurrent != TEXT('\0'))
			pszCurrent++;

		pszCurrent++;

		// ���������ڿ����� Ȯ��
		if (*pszCurrent == TEXT('\0'))
			break;
	}	/* while (pszCurrent != NULL) */

	// �޸𸮸� ��ȯ�� ��
	::FreeEnvironmentStrings(pEnvBlock);
}

// ��ƿ : ������ TEXT �������� �Լ�
void MyGetWindowText(__in WTL::CEdit& hWnd, __out std::tstring& str)
{
	BSTR bstrKey = NULL;
	hWnd.GetWindowText(bstrKey);
	str.clear();
	str.reserve(_tcslen(bstrKey));
	str.assign(bstrKey);

	::SysFreeString(bstrKey);
}

LRESULT CMainDlg::OnBnClickedButtonGet(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Clear
	m_value.SetWindowText(TEXT(""));

	// Get Dialog Edit Text
	tstring szKey;
	MyGetWindowText(m_key, szKey);

	// Get
	PTSTR pszValue = NULL;
	// ȯ�� ���� ���� ũ�� ����
	DWORD dwResult = ::GetEnvironmentVariable(szKey.c_str(), pszValue, 0);
	if (dwResult != 0) {
		// ���۸� �Ҵ�
		vector<TCHAR> buf(dwResult);
		pszValue = &buf[0];
		::GetEnvironmentVariable(szKey.c_str(), pszValue, dwResult);

		// �޽���â
		m_value.SetWindowText(pszValue);
	} else {
		tstring msg(TEXT("Ű("));
		msg.append(szKey);
		msg.append(TEXT(")���� ���� �� �����ϴ�"));

		// �޽���â
		MessageBox(msg.c_str(), TEXT("����"));
	}

	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonSet(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	tstring szKey;
	tstring szValue;

	MyGetWindowText(m_key, szKey);
	MyGetWindowText(m_value, szValue);

	::SetEnvironmentVariable(szKey.c_str(), szValue.c_str());

	return 0;
}

LRESULT CMainDlg::OnSize(
	UINT /*uMsg*/,
	WPARAM /*wParam*/,	// SIZE_RESTORED(0), SIZE_MINIMIZED(1), SIZE_MAXIMIZED(2), SIZE_MAXSHOW(3), SIZE_MAXHIDE(4)
	LPARAM lParam,	// low-order:width / high-order:height
	BOOL& /*bHandled*/
)
{
	if(m_control_init_ok)
	{
		WORD wWidth = LOWORD(lParam);
		WORD wHeight = HIWORD(lParam);

		if(m_is_first) {
			InitItemsSize(wWidth, wHeight);

			m_is_first = FALSE;
			return 0;
		}

		// Get Offset
		POINT offset;
		offset.x = m_mainDlgPtr.x - wWidth;
		offset.y = m_mainDlgPtr.y - wHeight;


		POINT n;
		// ListView
		n.x = m_listViewPtr.x - offset.x;
		n.y = m_listViewPtr.y - offset.y;
		m_list_env_var.SetWindowPos(NULL, 0/* ignores */,0 /* ignores */,n.x, n.y, SWP_NOMOVE | SWP_NOZORDER);

		RECT rcOwner;
		GetWindowRect(&rcOwner);

		// Static & Buttons
		/*n.x = m_keyCapPos.x;
		n.y = m_keyCapPos.y - offset.y;*/
		m_keyCap.SetWindowPos(NULL, m_keyCapPos.x - rcOwner.left, m_keyCapPos.y - rcOwner.top, 100, 100 , SWP_NOSIZE | SWP_NOZORDER);
		//RECT rt;
		//// Edit
		//rt = m_keyRect;
		//rt.right += offset.x;
		//rt.top += offset.y;
		//m_key.SetWindowPos(NULL, &rt, SWP_NOZORDER);

		return 1;
	}

	return 0;
}
