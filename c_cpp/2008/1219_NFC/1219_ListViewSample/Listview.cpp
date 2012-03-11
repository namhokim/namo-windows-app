#include "Listview.h"
#include <commctrl.h>
#include <assert.h>

const int LengthOfSubItem = 255;
const int CaptionLength = 256;
const int FieldLength = 256;

Listview::Listview(void)
: m_hListview(NULL)
{
	//setCaption();
}

Listview::Listview(HWND hListview)
{
	m_hListview = hListview;
	ListView_SetExtendedListViewStyle(m_hListview,LVS_EX_FULLROWSELECT);
	//setCaption();
}

Listview::~Listview(void)
{
}

void Listview::setHandle(HWND hParentDlgHandle, int nIDDlgItem)
{
	m_hListview = GetDlgItem(hParentDlgHandle, nIDDlgItem);
	ListView_SetExtendedListViewStyle(m_hListview,LVS_EX_FULLROWSELECT);
}

void Listview::resize(LPARAM lParam)
{
	MoveWindow(m_hListview,20,40,LOWORD(lParam)-60,HIWORD(lParam)-80,TRUE);
}

bool Listview::isThisHandle(HWND windowHandle)
{
	return (m_hListview == windowHandle);
}

std::list<_STRING> Listview::getSelectedItems(int iSubItem)
{
	std::list<_STRING> itemLists;
	_STRING aString;
	_TCHAR stringBuffer[LengthOfSubItem];
	int idx;
	idx = ListView_GetNextItem(m_hListview, -1, LVNI_ALL | LVNI_SELECTED);
	if(idx != -1) {
		do {
			ListView_GetItemText(m_hListview, idx, iSubItem, stringBuffer, LengthOfSubItem);
			aString = stringBuffer;
			itemLists.push_back(aString);
			idx = ListView_GetNextItem(m_hListview, idx, LVNI_ALL | LVNI_SELECTED);
		} while (idx != -1);
	}
	return itemLists;
}

bool Listview::isSelected()
{
	if(ListView_GetNextItem(m_hListview, -1, LVNI_ALL | LVNI_SELECTED) != -1) return true;
	else return false;
}

BOOL Listview::removeAllItem()
{
	return ListView_DeleteAllItems(m_hListview);
}

void Listview::visible(bool isVisible)
{
	if(isVisible)	ShowWindow(m_hListview, SW_SHOW);	// 보이게
	else			ShowWindow(m_hListview, SW_HIDE);			// 안보이게
}

VOID Listview::SetListviewCaption(int subItemIndex, _STRING captionName, int itemWidth)
{
	LVCOLUMN COL;
	_TCHAR varChar[CaptionLength];
	assert(captionName.size() < CaptionLength);
	wsprintf(varChar, _T("%s"), captionName.c_str());

	// 헤더 추가
	COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;
	COL.fmt=LVCFMT_LEFT;
	COL.iSubItem = subItemIndex;	// 아이템 순번
	COL.pszText = varChar;			// 첫번째 헤더
	COL.cx = itemWidth;
	
	ListView_InsertColumn(m_hListview,subItemIndex,&COL);
}

void Listview::InsertListView(int itemIndex, int subItemIndex, _STRING itemText)
{
	LVITEM LI;
	_TCHAR varChar[FieldLength];
	assert(itemText.size() < FieldLength);
	wsprintf(varChar, _T("%s"), itemText.c_str());

	// 텍스트와 이미지를 가진 아이템들을 등록한다.
	LI.mask=LVIF_TEXT;
	LI.state=0;
	LI.stateMask=0;

	LI.iImage=-1;			// 이미지 안붙임
	LI.iItem=itemIndex;			// 레코드번호
	LI.iSubItem=subItemIndex;	// 서브 아이템

	if(0 == subItemIndex) {		// 첫번째 아이템일 경우
		LI.pszText=varChar;
		ListView_InsertItem(m_hListview, &LI);
	}
	else ListView_SetItemText(m_hListview,itemIndex,subItemIndex,varChar);
}

void Listview::clearAllCaption()
{
	while(ListView_DeleteColumn(m_hListview,0));
}
