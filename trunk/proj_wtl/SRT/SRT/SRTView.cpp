#include "stdafx.h"
#include "SRTView.h"
#include "EnsureCleanup.h"	// use in SaveToFile
#include <string>			// for file read
#include <vector>			// for tokenize

///////////////////////////////////////////////////////////////////////////////
// C base
#define SUB_ITEM_NUM			6
#define	SIZE_OF_BOM_UTF16LE		2
#define SIZE_OF_TWO_CHAR		2

static const TCHAR ZeroBase			= TEXT('0');
static const LPWSTR TabCharWide		= L"\t";
static const LPWSTR CrLfWide		= L"\r\n";
static const BYTE BOM_UTF16LE [2]	= {'\xFF', '\xFE'};

// ���ڿ� �ð�-> �ý��� �ð�
void StringTimeToSystemTime(LPCTSTR strDate,SYSTEMTIME &st) {
	::ZeroMemory(&st, sizeof(SYSTEMTIME));
	st.wYear = (strDate[0]-ZeroBase)*1000
		+ (strDate[1]-ZeroBase)*100
		+ (strDate[2]-ZeroBase)*10
		+ (strDate[3]-ZeroBase);
	st.wMonth = (strDate[5]-ZeroBase)*10 + (strDate[6]-ZeroBase);
	st.wDay = (strDate[8]-ZeroBase)*10 + (strDate[9]-ZeroBase);
	st.wHour = (strDate[11]-ZeroBase)*10 + (strDate[12]-ZeroBase);
	st.wMinute = (strDate[14]-ZeroBase)*10 + (strDate[15]-ZeroBase);
	st.wSecond = (strDate[17]-ZeroBase)*10 + (strDate[18]-ZeroBase);
}


typedef struct _MYTIME {
    long wHour;
    long wMinute;
    long wSecond;
} MYTIME, *PMYTIME, *LPMYTIME;

void TimeToSystemTime(LPCTSTR strDate, LPMYTIME mt)
{
	int len, step;
	TCHAR ch;
	CString strHour, strMin, strSec;
	
	// �Ľ�
	len =lstrlen(strDate);
	step = 0;
	for(int i=0; i<len; ++i) {
		ch = strDate[i];
		if(TEXT(':')==ch) {
			++step;
			continue;
		}
		switch(step) {
			case 0:
				strHour.Insert(strHour.GetLength(), ch);
				break;
			case 1:
				strMin.Insert(strMin.GetLength(), ch);
				break;
			case 2:
				strSec.Insert(strSec.GetLength(), ch);
				break;
		}
	}

	// ��� �Է�
	//lTotal = (lHour*60*60) + (lMin*60) + lSec;
	ZeroMemory(mt, sizeof(MYTIME));
	(*mt).wHour = _ttol(strHour);
	(*mt).wMinute = _ttol(strMin);
	(*mt).wSecond = _ttol(strSec);

	return;
}

void AccumulateStringTime(LPCTSTR strDate, LPMYTIME mt)
{
	MYTIME tmt;

	// ���ڿ� -> �ð��� ��ȯ
	TimeToSystemTime(strDate, &tmt);

	// �ð��� ��
	(*mt).wHour		+= tmt.wHour;
	(*mt).wMinute	+= tmt.wMinute;
	(*mt).wSecond	+= tmt.wSecond;

	// ����ȭ
	long r;		// remain
	// ��
	r = (*mt).wSecond / 60;
	(*mt).wSecond	%= 60;
	(*mt).wMinute	+= r;

	// ��
	r = (*mt).wMinute / 60;
	(*mt).wMinute	%= 60;
	(*mt).wHour		+= r;

	return;
}

// ���� ���پ� �б�
DWORD ReadFileByLine(__in HANDLE hFile, __out std::wstring& line)
{
	using std::wstring;

	wchar_t wch;
	DWORD dwRead;
	BOOL bResult;

	line.clear();
	while(1) {
		bResult = ::ReadFile(hFile, &wch, sizeof(wchar_t), &dwRead, NULL);

		if(wch==L'\n') {
			wstring::size_type size = line.size();
			if(size>1 && line[size-1]==L'\r') {
				line.erase(size-1);
			}
			break;
		}

		if (bResult && 0==dwRead) {	// at the end of the file
			break;
		}

		line.push_back(wch);
	}	

	return line.size();
}

// tokenize
template <typename T>
void getTokensWithVector(T str, T delimiters, std::vector<T> &tokensList)
{
	T::size_type lastPos = str.find_first_not_of(delimiters, 0);	// �� ù ���ڰ� �������� ��� ����
	T::size_type pos     = str.find_first_of(delimiters, lastPos);	// �����ڰ� �ƴ� ù ���ڸ� ã�´�

	while (T::npos != pos || T::npos != lastPos) {
		tokensList.push_back(str.substr(lastPos, pos - lastPos));	// token�� ã������ vector�� �߰��Ѵ�
		lastPos = str.find_first_not_of(delimiters, pos);			// �����ڸ� �پ�Ѵ´�.  "not_of"�� �����϶�
		pos = str.find_first_of(delimiters, lastPos);				// ���� �����ڰ� �ƴ� ���ڸ� ã�´�
	}
}

// �� SYSTEMTIME ����ü�� ���� ��¥���� �Ǵ�
// ��ȯ�� : �����ϸ� true, �ٸ��� false�� ��ȯ
inline bool IsSameDate(const LPSYSTEMTIME st1,const LPSYSTEMTIME st2)
{
	// �� ������ ��,��,�� ������ �Ѵ�.
	// �ٸ� ��¥�� ���
	// ������ �ٲ�� �ͺ��ٴ� ��¥�� �ٲ�� �󵵰� �����Ƿ�
	// ��Ƚ���� �� �� �� �ֱ� ����
	return (st1->wDay==st2->wDay && st1->wMonth==st2->wMonth && st1->wYear==st2->wYear);
}

///////////////////////////////////////////////////////////////////////////////

void CSRTView::InitListColumnName()
{
	// TODO : ����� -> �ϵ��ڵ� ��� ���ҽ��� ���ڿ� ������ ����
	// �ʺ� ���ҽ��� ����ϴ� ������� or strlen ���� �̿�(std::wstring.size())

	// ����Ʈ �� ��� �߰�
	this->InsertColumn(0, TEXT("seq #"),	LVCFMT_CENTER, 50);	// ù��° ���
	this->InsertColumn(1, TEXT("�۾�"),		LVCFMT_LEFT, 90);	// �ι�° ���
	this->InsertColumn(2, TEXT("�׷�"),		LVCFMT_LEFT, 50);	// ����° ���
	this->InsertColumn(3, TEXT("����"),		LVCFMT_CENTER, 120);// �׹�° ���
	this->InsertColumn(4, TEXT("��"),		LVCFMT_CENTER, 120);// �ټ���° ���
	this->InsertColumn(5, TEXT("����ð�"),	LVCFMT_CENTER, 80);	// ������° ���
}

// �ϳ��� ���ڵ带 ����Ʈ�信 �Է�
// ��ȯ�� : ��¥�� ����Ǿ����� ����(����� true)
bool CSRTView::InsertOneRecord(LPCTSTR jobname, LPCTSTR groupname, LPSYSTEMTIME lpST)
{
	bool isDateChanged = false;
	int idx;
	TCHAR time[SIZE_TIME]={0};
	wsprintf(time, TEXT("%04d/%02d/%02d %02d:%02d:%02d"),
		lpST->wYear,lpST->wMonth,lpST->wDay,lpST->wHour,lpST->wMinute,lpST->wSecond);

	// ���� ����ؾ��� ������ ��ȣ get
	idx = this->GetItemCount();
	if(idx>0) {	// ���� ��� ���� �ؾ� �Ѵٸ�,
		isDateChanged = CompleteBefore(idx-1, time);
	}

	// ��������
	TCHAR filename[MAX_PATH];

	if(isDateChanged) {	// ��¥�� �ٲ������
		// ���� ���ڸ� ����
		TCHAR strDateBefore[SIZE_TIME]={0};
		this->GetItemText(idx-1, 3, strDateBefore, SIZE_TIME);

		SYSTEMTIME st_before;
		StringTimeToSystemTime(strDateBefore,st_before);
		App::GetFilename(&st_before, filename);
		// ������ �����ϰ�
		this->SaveToFile(filename);

		// ����Ʈ�� �ʱ�ȭ ����
		this->DeleteAllItems();
		idx = 0;
	}

	NewInsert(idx, jobname, groupname, time);	// ���ο� ���ڵ带 �߰�
	this->EnsureVisible(idx, TRUE);				// �Է��� ���ڵ尡 ���̰� ��ũ��

	// ��������(����� ��¥)
	App::GetFilename(lpST, filename);
	this->SaveToFile(filename);

	return isDateChanged;
}

// ���� ����Ʈ���� ������ ���Ͽ� ����
void CSRTView::SaveToFile(LPCTSTR filename)
{
	// ������ �ڵ��� ����.(�ڵ��� �ڵ����� ����)
	CEnsureCloseFile hFile = ::CreateFile(filename, GENERIC_WRITE,
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if(INVALID_HANDLE_VALUE == hFile) {		// ���� �ڵ� ����
		TCHAR ErrMsg[1024] = {0,};
		Win::TranslateErrorCode(GetLastError(), ErrMsg, 1024);
		MessageBox(ErrMsg, TEXT("����"));
		return;								// ����
	}

	// ����Ʈ�信�� ���� �����ͼ� ���Ϸ� ����
	SaveWithUnicode(hFile);
}

// ������ ������ ����Ʈ��� �ҷ��´�
void CSRTView::LoadFromFile(LPCTSTR filename)
{
	// ������ �ڵ��� ����.(�ڵ��� �ڵ����� ����)
	CEnsureCloseFile hFile = ::CreateFile(filename, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile) {		// ���� �ڵ� ����
		DWORD dwCode = ::GetLastError();

		// ������ ��� �߻��Ѵٸ� ���� �˸� �ʿ䰡 ����
		if(ERROR_FILE_NOT_FOUND!=dwCode) {
			TCHAR ErrMsg[1024] = {0,};
			Win::TranslateErrorCode(dwCode, ErrMsg, 1024);
			MessageBox(ErrMsg, TEXT("����"));
		}
		return;								// ����
	}

	// BOM ���� Ȯ��
	BYTE b[SIZE_OF_BOM_UTF16LE];
	DWORD dwRead;
	bool isUnicode = false;
	if(::ReadFile(hFile, b, SIZE_OF_BOM_UTF16LE, &dwRead, NULL)) {
		if(SIZE_OF_BOM_UTF16LE==dwRead
			&& b[0]==BOM_UTF16LE[0] && b[1]==BOM_UTF16LE[1])
		{
			isUnicode = true;
		} else {
			::SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
		}
	}
	// �ڵ�κ��� �����͸� �о ����Ʈ�信 �Է�
	if(isUnicode) LoadWithUnicode(hFile);
	// TODO : �������ڵ忡�� �����͸� �ҷ�����
}

UINT CSRTView::GetSelectedCountWithSum(WTL::CString& str) const
{
	UINT selCnt;
	int items;
	TCHAR strDate[SIZE_TIME]={0};
	MYTIME mt;

	ZeroMemory(&mt, sizeof(mt));

	// ������ �׸��� ������ ��������
	selCnt = this->GetSelectedCount();

	if(0==selCnt) return 0;

	items = this->GetItemCount();
	for(int i=0; i<items; ++i) {
		if( GetItemState(i, LVIS_SELECTED) ) {
			GetItemText(i, 5, strDate, SIZE_TIME);
			if(lstrlen(strDate)>0) {
				AccumulateStringTime(strDate, &mt);
			}
		}
	}
	str.Format(TEXT("%d:%02d:%02d"), mt.wHour, mt.wMinute, mt.wSecond);
	return selCnt;
}

// ���� �߰��� ���ڵ带 ������
// ��ȯ�� : ��¥�� ����Ǿ����� ����(����� true)
bool CSRTView::CompleteBefore(LONG index, LPCTSTR time)
{
	bool isDateChanged = false;
	TCHAR strDateBefore[SIZE_TIME]={0};
	TCHAR strDateDiff[SIZE_TIME]={0};
	// 1. ���� �׸�: ���� �ð� ���
	this->SetItemText(index, 4, time);

	// 2. ���� �׸�: ���� �ð� ��� & ���
	//	2-1.���۽ð� ��������
	this->GetItemText(index, 3, strDateBefore, SIZE_TIME);
	//	2-2. ���� ���ϱ�
	isDateChanged = this->DiffStringTime(time, strDateBefore, strDateDiff);
	//	2-3. ��� ����ϱ�
	this->SetItemText(index,5,strDateDiff);

	return isDateChanged;
}

// ���ο� ���ڵ带 �߰�(�Ϻ�)
void CSRTView::NewInsert(LONG index, LPCTSTR job, LPCTSTR group, LPCTSTR time)
{
	TCHAR seqNumber[SIZE_SEQ]={0};	// �Ϸù�ȣ(������<- ����)
	wsprintf(seqNumber, TEXT("%d"), index+1);	// �Ϸù�ȣ

	// - ù��° �÷� -	
	this->InsertItem(index, seqNumber);
	// - �ι�° �÷� -
	this->SetItem(index, 1, LVIF_TEXT, job, 0, 0, 0, 0);
	// - ����° �÷� -
	this->SetItem(index, 2, LVIF_TEXT, group, 0, 0, 0, 0);
	// - �׹�° �÷� -
	this->SetItem(index, 3, LVIF_TEXT, time, 0, 0, 0, 0);
}

// ����Ʈ�信 ���ڵ带 �߰�(�� ��ü)
void CSRTView::InsertOneRecord(LPCWSTR seq, LPCWSTR job, LPCWSTR group,
		LPCWSTR s_time, LPCWSTR e_time, LPCWSTR lap_time)
{
	int idx = this->GetItemCount();	// �Է��� ��ġ ã��

#ifdef _UNICODE
	// - ù��° �÷� -
	this->InsertItem(idx, seq);
	// - �ι�° �÷� -
	if(NULL!=job) this->SetItem(idx, 1, LVIF_TEXT, job, 0, 0, 0, 0);
	// - ����° �÷� -
	if(NULL!=group) this->SetItem(idx, 2, LVIF_TEXT, group, 0, 0, 0, 0);
	// - �׹�° �÷� -
	if(NULL!=s_time) this->SetItem(idx, 3, LVIF_TEXT, s_time, 0, 0, 0, 0);

	// - �ټ���° �÷� -
	if(NULL!=e_time) this->SetItem(idx, 4, LVIF_TEXT, e_time, 0, 0, 0, 0);
	// - ������° �÷� -
	if(NULL!=lap_time) this->SetItem(idx, 5, LVIF_TEXT, lap_time, 0, 0, 0, 0);
#else
	USES_CONVERSION;
	// - ù��° �÷� -
	this->InsertItem(idx, W2T(seq));
	// - �ι�° �÷� -
	if(NULL!=job) this->SetItem(idx, 1, LVIF_TEXT, W2T(job), 0, 0, 0, 0);
	// - ����° �÷� -
	if(NULL!=group) this->SetItem(idx, 2, LVIF_TEXT, W2T(group), 0, 0, 0, 0);
	// - �׹�° �÷� -
	if(NULL!=s_time) this->SetItem(idx, 3, LVIF_TEXT, W2T(s_time), 0, 0, 0, 0);

	// - �ټ���° �÷� -
	if(NULL!=e_time) this->SetItem(idx, 4, LVIF_TEXT, W2T(e_time), 0, 0, 0, 0);
	// - ������° �÷� -
	if(NULL!=lap_time) this->SetItem(idx, 5, LVIF_TEXT, W2T(lap_time), 0, 0, 0, 0);
#endif
}

// ���ڿ� �ð��� ���̸� ����
bool CSRTView::DiffStringTime(LPCTSTR postStringTime,LPCTSTR preStringTime,TCHAR* diffStringTime)
{
	// ���ڿ� -> SYSTEMTIME�� �ְ�
	SYSTEMTIME stPost, stPre;
	StringTimeToSystemTime(postStringTime,stPost);
	StringTimeToSystemTime(preStringTime,stPre);

	// FILETIME���� ��ȯ�� ����
	FILETIME ftPost, ftPre;
	::SystemTimeToFileTime(&stPost, &ftPost);
	::SystemTimeToFileTime(&stPre, &ftPre);

	// ���̸� 64��Ʈ ������ �ؼ� ���
	ULARGE_INTEGER ulPost, ulPre, ulDiff;
	ulPost.HighPart	= ftPost.dwHighDateTime;
	ulPost.LowPart	= ftPost.dwLowDateTime;

	ulPre.HighPart	= ftPre.dwHighDateTime;
	ulPre.LowPart	= ftPre.dwLowDateTime;

	bool isPositive = (ulPost.QuadPart >= ulPre.QuadPart);
	if(isPositive) ulDiff.QuadPart	= ulPost.QuadPart - ulPre.QuadPart;
	else ulDiff.QuadPart = ulPre.QuadPart - ulPost.QuadPart;

	// ��� ����� FILETIME�� SYSTEMTIME���� ��ȯ
	FILETIME ftDiff;
	ftDiff.dwHighDateTime = ulDiff.HighPart;
	ftDiff.dwLowDateTime  = ulDiff.LowPart;

	SYSTEMTIME stDiff;
	::FileTimeToSystemTime(&ftDiff, &stDiff);

	if(isPositive) wsprintf(diffStringTime,TEXT("%d:%02d:%02d"),
		((stDiff.wDay-1)*24 + stDiff.wHour),stDiff.wMinute,stDiff.wSecond);
	else wsprintf(diffStringTime,TEXT("-%d:%02d:%02d"),
		((stDiff.wDay-1)*24 + stDiff.wHour),stDiff.wMinute,stDiff.wSecond);

	return (!IsSameDate(&stPost, &stPre));
}

// ����Ʈ���� ������ �����ڵ�� ����
void CSRTView::SaveWithUnicode(HANDLE hFile)
{
	DWORD dwWritten;	// ���� ������ ����ϱ� ����

	// BOM ���
	::WriteFile(hFile, BOM_UTF16LE, SIZE_OF_BOM_UTF16LE, &dwWritten, NULL);

	// ���� ������ ����
	int idx = this->GetItemCount();
	for(int i=0; i<idx; ++i) {
		for(int j=0; j<SUB_ITEM_NUM; ++j) {
			if(0!=j) ::WriteFile(hFile, TabCharWide, sizeof(WCHAR), &dwWritten, NULL);	// �ǹ���

			BSTR bstrText = NULL;
			this->GetItemText(i, j, bstrText);
			if(NULL!=bstrText) {
				::WriteFile(hFile, bstrText, wcslen(bstrText)*sizeof(OLECHAR), &dwWritten, NULL);
				::SysFreeString(bstrText);
			}
		}	
		::WriteFile(hFile, CrLfWide, SIZE_OF_TWO_CHAR*sizeof(WCHAR), &dwWritten, NULL);	// �ٹٲ�
	}
}

// �����ڵ��� ������ ����Ʈ��� �ҷ���
void CSRTView::LoadWithUnicode(HANDLE hFile)
{
	using std::wstring;
	using std::vector;

	DWORD dwRead;
	wstring line;

	while(1) {
		dwRead = ReadFileByLine(hFile, line);
		if (0==dwRead) {	// at the end of the file
			break;
		} else {
			vector<wstring> vec;
			vector<wstring>::size_type size;
			getTokensWithVector<wstring>(line, L"\t", vec);	// ������ �ǹ��ڷ� �и�
			size = vec.size();
			if(size>=4) {
				switch(vec.size()) {
				case 4:
					this->InsertOneRecord(vec[0].c_str(), vec[1].c_str(), vec[2].c_str(), vec[3].c_str());
					break;
				case 5:
					this->InsertOneRecord(vec[0].c_str(), vec[1].c_str(), vec[2].c_str(), vec[3].c_str(), vec[4].c_str());
					break;
				case 6:
				default:	// �� ���еǾ� ���� ���ڵ��� 6���̻�
					this->InsertOneRecord(vec[0].c_str(), vec[1].c_str(), vec[2].c_str(), vec[3].c_str(), vec[4].c_str(), vec[5].c_str());
					break;
				}
			}
		}
	}
}
