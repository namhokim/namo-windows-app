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
	void resize(LPARAM);		// ����Ʈ �� ũ�� ��ȭ
	bool isThisHandle(HWND);
	std::list<_STRING> getSelectedItems(int);
	bool isSelected();
	// insert : interface
	BOOL removeAllItem();
	void visible(bool);				// ����Ʈ�並 ���̰�/�Ⱥ��̰� ��
	void SetListviewCaption(int, _STRING, int);		// ����Ʈ���� ĸ���� �����Ѵ�
	void InsertListView(int, int, _STRING);
protected:
	HWND m_hListview;
	
	void clearAllCaption();
};
