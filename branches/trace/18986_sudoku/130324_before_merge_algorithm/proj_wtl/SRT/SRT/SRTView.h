// SRTView.h : interface of the CSRTView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlmisc.h>		// for WTL:CString
#include "resource.h"

class CSRTView : public CWindowImpl<CSRTView, CListViewCtrl>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	// 메세지 맵
	BEGIN_MSG_MAP(CSRTView)
	END_MSG_MAP()

	// 컬럼이름 초기화
	void InitListColumnName();

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	bool InsertOneRecord(LPCTSTR jobname, LPCTSTR groupname, LPSYSTEMTIME st);
	void SaveToFile(LPCTSTR filename);
	void LoadFromFile(LPCTSTR filename);
	UINT GetSelectedCountWithSum(WTL::CString& str) const;
private:
	// 리스트뷰 추가관련
	bool CompleteBefore(LONG index, LPCTSTR time);
	void NewInsert(LONG index, LPCTSTR job, LPCTSTR group, LPCTSTR time);
	void InsertOneRecord(LPCWSTR seq, LPCWSTR job=NULL, LPCWSTR group=NULL,
		LPCWSTR s_time=NULL, LPCWSTR e_time=NULL, LPCWSTR lap_time=NULL);
	bool DiffStringTime(LPCTSTR postStringTime,LPCTSTR preStringTime,TCHAR* diffStringTime);

	// 파일저장 관련
	void SaveWithUnicode(HANDLE hFile);
	// 파일불러오기 관련
	void LoadWithUnicode(HANDLE hFile);	
};
