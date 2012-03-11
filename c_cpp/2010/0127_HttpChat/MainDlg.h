// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlmisc.h>
#include <vector>	// for dynamic memory allocation
#include <string>
#include "Ryeol/RyeolHttpClient.h"
#define TIMER1	1

//const std::wstring BaseURI(L"http://pass.sindoh.com/ws/chat/");
const std::wstring BaseURI(L"http://pass.sindoh.com/ws/chat/");

inline std::wstring string2wstring(std::string str) {
	std::wstring wstr(str.length(),L' ');
	copy(str.begin(),str.end(),wstr.begin());
	return wstr;
}

// UTF-16LE를 UTF-8로 변환
std::wstring URIEncode(std::wstring from)
{
	const int size = 1024 * 10;
	BYTE buf[size];
	::memset(buf, 0, size);
	int res = ::WideCharToMultiByte(CP_UTF8, 0, from.c_str(), from.size(), (LPSTR)buf, size, NULL, NULL);
	if(0==res) return from;

	char cBuf[100];
	std::string strBuf;
	for(int i=0; i<res; i++) {
		wsprintfA(cBuf, "%%%2X", buf[i]);
		strBuf.append(cBuf);
	}
	return string2wstring(strBuf);
}

void PutMsg(const std::vector<TCHAR> &content)
{
	using namespace Ryeol;

	CHttpClient         objHttpReq;
	CHttpResponse *     pobjHttpRes = NULL;

	try {
		// Initialize the User Agent
		objHttpReq.SetInternet (_T ("HTTP Chat v0.2 send"));
		std::wstring uri(BaseURI);
		uri.append(URI_PUT_USER);
		uri.append(TEXT("/"));
		uri.append( URIEncode(std::wstring(content.begin(), content.end()-3)) );
		pobjHttpRes = objHttpReq.RequestGet(uri.c_str()) ;
	} catch (httpclientexception e) {
		// Place exception handling codes here.
	}
}

std::wstring GetMsg()
{
	using namespace Ryeol;

	CHttpClient         objHttpReq;
	CHttpResponse *     pobjHttpRes = NULL;

	try {
		// Initialize the User Agent
		objHttpReq.SetInternet (_T ("HTTP Chat v0.2 recv"));
		objHttpReq.SetUseUtf8 (TRUE);
		std::wstring uri(BaseURI);
		uri.append(URI_GET_USER);
		pobjHttpRes = objHttpReq.RequestGet(uri.c_str()) ;

		// Reads the length of the stream
		DWORD       dwContSize ;
		// If the length is not specified
		if ( !pobjHttpRes->GetContentLength (dwContSize) )
			dwContSize = 0 ;

		const DWORD     cbBuff = 1024 * 10 ;
		BYTE            byBuff[cbBuff] ;
		DWORD           dwRead ;
		size_t          cbTotal = 0 ;

		std::string byteBuf;
		// Reads the data stream returned by the HTTP server.
		while ( dwRead = pobjHttpRes->ReadContent (byBuff, cbBuff - 1) ) {
			cbTotal += dwRead ;

			byBuff[dwRead] = '\0' ;
			byteBuf.append((char *)byBuff);
		}

		if(0==cbTotal) return L"";
		wchar_t buf[1024 * 10];
		::memset(buf, 0, 1024 * 10);
		int ret = ::MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, (LPCSTR)byteBuf.c_str(), byteBuf.size(), buf, 1024 * 10);

		return buf;
	} catch (httpclientexception e) {
		// Place exception handling codes here.
	}
	return L"";
}

class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	CEdit m_edit_DispMsg;
	CEdit m_edit_SendMsg;
	HANDLE hTimer;

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	virtual BOOL OnIdle()
	{
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		UIAddChildWindowContainer(m_hWnd);

		// 컨트롤 객체 배정
		m_edit_DispMsg = GetDlgItem(IDC_EDIT_DISP_MSG);
		m_edit_SendMsg = GetDlgItem(IDC_EDIT_SEND_MSG);

		// 표지 에디트 컨트롤 용량 확장
		m_edit_DispMsg.SendMessage(EM_LIMITTEXT, (WPARAM)-1, 0L);

		// 보내는 에디트 컨트롤에 포커싱
		m_edit_SendMsg.SetFocus();

		hTimer = (HANDLE)::SetTimer(this->m_hWnd, TIMER1, 1000, NULL);

		SetWindowText(URI_GET_USER);

		return FALSE;	// FALSE여야 포커싱이 먹는다
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(NULL!=hTimer) ::KillTimer(this->m_hWnd, TIMER1);
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		return 0;
	}
	
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		std::wstring msg = GetMsg();
		if(msg.empty()) return 0;
		else {
			// 에디트 컨트롤에 문자열 입력(스크롤 기능 포함)
			int length = m_edit_DispMsg.GetWindowTextLength();
			m_edit_DispMsg.SendMessage(EM_SETSEL, length, length);
			m_edit_DispMsg.SendMessage(EM_REPLACESEL, FALSE, (LPARAM)msg.c_str());
		}
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		// 입력한 메시지를 가져온다(최대 1024)
		int length = m_edit_SendMsg.GetWindowTextLength();
		if(0==length) {
			return 0;
		}
		length = length + 3;
		std::vector<TCHAR> buf(length);	// 동적 메모리 할당
		m_edit_SendMsg.GetWindowText(&buf[0], length);
		buf[length-3] = '\r';
		buf[length-2] = '\n';
		buf[length-1] = '\0';

		// 에디트 컨트롤에 문자열 입력(스크롤 기능 포함)
		length = m_edit_DispMsg.GetWindowTextLength();
		m_edit_DispMsg.SendMessage(EM_SETSEL, length, length);
		m_edit_DispMsg.SendMessage(EM_REPLACESEL, FALSE, (LPARAM)&buf[0]);

		// 입력창의 글씨 지움
		m_edit_SendMsg.SetWindowText(L"");

		// 메시지 전송
		::PutMsg(buf);

		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CloseDialog(wID);
		return 0;
	}

	void CloseDialog(int nVal)
	{
		DestroyWindow();
		::PostQuitMessage(nVal);
	}
};
