#pragma once
#include "StringUtil.h"
#include <windows.h>
#include <list>

class Listview
{
public:
	Listview(void);
	Listview(HWND hListview);
	~Listview(void);
	void setHandle(HWND hParentDlgHandle, int nIDDlgItem);
	HWND getHandle()	{	return m_hListview;	}
	void resize(LPARAM);		// 리스트 뷰 크기 변화
	bool isThisHandle(HWND);
	std::list<_STRING> getSelectedItems(int);
	bool isSelected();
	// insert : interface
	BOOL removeAllItem();
	void visible(bool);				// 리스트뷰를 보이게/안보이게 함
	void SetListviewCaption(int, _STRING, int);		// 리스트뷰의 캡션을 설정한다
	void InsertListView(int, int, _STRING);
protected:
	HWND m_hListview;
	
	void clearAllCaption();
};
