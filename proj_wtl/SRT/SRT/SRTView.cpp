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

// 문자열 시간-> 시스템 시간
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
	
	// 파싱
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

	// 결과 입력
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

	// 문자열 -> 시간형 변환
	TimeToSystemTime(strDate, &tmt);

	// 시간형 합
	(*mt).wHour		+= tmt.wHour;
	(*mt).wMinute	+= tmt.wMinute;
	(*mt).wSecond	+= tmt.wSecond;

	// 정규화
	long r;		// remain
	// 초
	r = (*mt).wSecond / 60;
	(*mt).wSecond	%= 60;
	(*mt).wMinute	+= r;

	// 분
	r = (*mt).wMinute / 60;
	(*mt).wMinute	%= 60;
	(*mt).wHour		+= r;

	return;
}

// 파일 한줄씩 읽기
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
	T::size_type lastPos = str.find_first_not_of(delimiters, 0);	// 맨 첫 글자가 구분자인 경우 무시
	T::size_type pos     = str.find_first_of(delimiters, lastPos);	// 구분자가 아닌 첫 글자를 찾는다

	while (T::npos != pos || T::npos != lastPos) {
		tokensList.push_back(str.substr(lastPos, pos - lastPos));	// token을 찾았으니 vector에 추가한다
		lastPos = str.find_first_not_of(delimiters, pos);			// 구분자를 뛰어넘는다.  "not_of"에 주의하라
		pos = str.find_first_of(delimiters, lastPos);				// 다음 구분자가 아닌 글자를 찾는다
	}
}

// 두 SYSTEMTIME 구조체가 같은 날짜인지 판단
// 반환값 : 동일하면 true, 다르면 false를 반환
inline bool IsSameDate(const LPSYSTEMTIME st1,const LPSYSTEMTIME st2)
{
	// 비교 순서는 일,월,년 순으로 한다.
	// 다른 날짜일 경우
	// 연도가 바뀌는 것보다는 날짜가 바뀌는 빈도가 높으므로
	// 비교횟수를 줄 일 수 있기 때문
	return (st1->wDay==st2->wDay && st1->wMonth==st2->wMonth && st1->wYear==st2->wYear);
}

///////////////////////////////////////////////////////////////////////////////

void CSRTView::InitListColumnName()
{
	// TODO : 헤더명 -> 하드코딩 대신 리소스의 문자열 사용토록 변경
	// 너비도 리소스를 사용하는 방안으로 or strlen 등을 이용(std::wstring.size())

	// 리스트 뷰 헤더 추가
	this->InsertColumn(0, TEXT("seq #"),	LVCFMT_CENTER, 50);	// 첫번째 헤더
	this->InsertColumn(1, TEXT("작업"),		LVCFMT_LEFT, 90);	// 두번째 헤더
	this->InsertColumn(2, TEXT("그룹"),		LVCFMT_LEFT, 50);	// 세번째 헤더
	this->InsertColumn(3, TEXT("시작"),		LVCFMT_CENTER, 120);// 네번째 헤더
	this->InsertColumn(4, TEXT("끝"),		LVCFMT_CENTER, 120);// 다섯번째 헤더
	this->InsertColumn(5, TEXT("수행시간"),	LVCFMT_CENTER, 80);	// 여섯번째 헤더
}

// 하나의 레코드를 리스트뷰에 입력
// 반환값 : 날짜가 변경되었는지 여부(변경시 true)
bool CSRTView::InsertOneRecord(LPCTSTR jobname, LPCTSTR groupname, LPSYSTEMTIME lpST)
{
	bool isDateChanged = false;
	int idx;
	TCHAR time[SIZE_TIME]={0};
	wsprintf(time, TEXT("%04d/%02d/%02d %02d:%02d:%02d"),
		lpST->wYear,lpST->wMonth,lpST->wDay,lpST->wHour,lpST->wMinute,lpST->wSecond);

	// 새로 등록해야할 아이템 번호 get
	idx = this->GetItemCount();
	if(idx>0) {	// 이전 기록 수행 해야 한다면,
		isDateChanged = CompleteBefore(idx-1, time);
	}

	// 파일저장
	TCHAR filename[MAX_PATH];

	if(isDateChanged) {	// 날짜가 바뀌었으면
		// 이전 날자를 구해
		TCHAR strDateBefore[SIZE_TIME]={0};
		this->GetItemText(idx-1, 3, strDateBefore, SIZE_TIME);

		SYSTEMTIME st_before;
		StringTimeToSystemTime(strDateBefore,st_before);
		App::GetFilename(&st_before, filename);
		// 파일을 저장하고
		this->SaveToFile(filename);

		// 리스트를 초기화 한후
		this->DeleteAllItems();
		idx = 0;
	}

	NewInsert(idx, jobname, groupname, time);	// 새로운 레코드를 추가
	this->EnsureVisible(idx, TRUE);				// 입력한 레코드가 보이게 스크롤

	// 파일저장(기록한 날짜)
	App::GetFilename(lpST, filename);
	this->SaveToFile(filename);

	return isDateChanged;
}

// 현재 리스트뷰의 내용을 파일에 저장
void CSRTView::SaveToFile(LPCTSTR filename)
{
	// 파일의 핸들을 연다.(핸들은 자동으로 닫힘)
	CEnsureCloseFile hFile = ::CreateFile(filename, GENERIC_WRITE,
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if(INVALID_HANDLE_VALUE == hFile) {		// 파일 핸들 에러
		TCHAR ErrMsg[1024] = {0,};
		Win::TranslateErrorCode(GetLastError(), ErrMsg, 1024);
		MessageBox(ErrMsg, TEXT("에러"));
		return;								// 종료
	}

	// 리스트뷰에서 값을 가져와서 파일로 저장
	SaveWithUnicode(hFile);
}

// 파일의 내용을 리스트뷰로 불러온다
void CSRTView::LoadFromFile(LPCTSTR filename)
{
	// 파일의 핸들을 연다.(핸들은 자동으로 닫힘)
	CEnsureCloseFile hFile = ::CreateFile(filename, GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile) {		// 파일 핸들 에러
		DWORD dwCode = ::GetLastError();

		// 파일이 없어서 발생한다면 구지 알릴 필요가 없다
		if(ERROR_FILE_NOT_FOUND!=dwCode) {
			TCHAR ErrMsg[1024] = {0,};
			Win::TranslateErrorCode(dwCode, ErrMsg, 1024);
			MessageBox(ErrMsg, TEXT("에러"));
		}
		return;								// 종료
	}

	// BOM 존재 확인
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
	// 핸들로부터 데이터를 읽어서 리스트뷰에 입력
	if(isUnicode) LoadWithUnicode(hFile);
	// TODO : 비유니코드에서 데이터를 불러오기
}

UINT CSRTView::GetSelectedCountWithSum(WTL::CString& str) const
{
	UINT selCnt;
	int items;
	TCHAR strDate[SIZE_TIME]={0};
	MYTIME mt;

	ZeroMemory(&mt, sizeof(mt));

	// 선택한 항목의 개수를 가져오고
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

// 이전 추가된 레코드를 마무리
// 반환값 : 날짜가 변경되었는지 여부(변경시 true)
bool CSRTView::CompleteBefore(LONG index, LPCTSTR time)
{
	bool isDateChanged = false;
	TCHAR strDateBefore[SIZE_TIME]={0};
	TCHAR strDateDiff[SIZE_TIME]={0};
	// 1. 이전 항목: 종료 시간 기록
	this->SetItemText(index, 4, time);

	// 2. 이전 항목: 수행 시간 계산 & 기록
	//	2-1.시작시간 가져오기
	this->GetItemText(index, 3, strDateBefore, SIZE_TIME);
	//	2-2. 차이 구하기
	isDateChanged = this->DiffStringTime(time, strDateBefore, strDateDiff);
	//	2-3. 결과 기록하기
	this->SetItemText(index,5,strDateDiff);

	return isDateChanged;
}

// 새로운 레코드를 추가(일부)
void CSRTView::NewInsert(LONG index, LPCTSTR job, LPCTSTR group, LPCTSTR time)
{
	TCHAR seqNumber[SIZE_SEQ]={0};	// 일련번호(문자형<- 숫자)
	wsprintf(seqNumber, TEXT("%d"), index+1);	// 일련번호

	// - 첫번째 컬럼 -	
	this->InsertItem(index, seqNumber);
	// - 두번째 컬럼 -
	this->SetItem(index, 1, LVIF_TEXT, job, 0, 0, 0, 0);
	// - 세번째 컬럼 -
	this->SetItem(index, 2, LVIF_TEXT, group, 0, 0, 0, 0);
	// - 네번째 컬럼 -
	this->SetItem(index, 3, LVIF_TEXT, time, 0, 0, 0, 0);
}

// 리스트뷰에 레코드를 추가(행 전체)
void CSRTView::InsertOneRecord(LPCWSTR seq, LPCWSTR job, LPCWSTR group,
		LPCWSTR s_time, LPCWSTR e_time, LPCWSTR lap_time)
{
	int idx = this->GetItemCount();	// 입력할 위치 찾음

#ifdef _UNICODE
	// - 첫번째 컬럼 -
	this->InsertItem(idx, seq);
	// - 두번째 컬럼 -
	if(NULL!=job) this->SetItem(idx, 1, LVIF_TEXT, job, 0, 0, 0, 0);
	// - 세번째 컬럼 -
	if(NULL!=group) this->SetItem(idx, 2, LVIF_TEXT, group, 0, 0, 0, 0);
	// - 네번째 컬럼 -
	if(NULL!=s_time) this->SetItem(idx, 3, LVIF_TEXT, s_time, 0, 0, 0, 0);

	// - 다섯번째 컬럼 -
	if(NULL!=e_time) this->SetItem(idx, 4, LVIF_TEXT, e_time, 0, 0, 0, 0);
	// - 여섯번째 컬럼 -
	if(NULL!=lap_time) this->SetItem(idx, 5, LVIF_TEXT, lap_time, 0, 0, 0, 0);
#else
	USES_CONVERSION;
	// - 첫번째 컬럼 -
	this->InsertItem(idx, W2T(seq));
	// - 두번째 컬럼 -
	if(NULL!=job) this->SetItem(idx, 1, LVIF_TEXT, W2T(job), 0, 0, 0, 0);
	// - 세번째 컬럼 -
	if(NULL!=group) this->SetItem(idx, 2, LVIF_TEXT, W2T(group), 0, 0, 0, 0);
	// - 네번째 컬럼 -
	if(NULL!=s_time) this->SetItem(idx, 3, LVIF_TEXT, W2T(s_time), 0, 0, 0, 0);

	// - 다섯번째 컬럼 -
	if(NULL!=e_time) this->SetItem(idx, 4, LVIF_TEXT, W2T(e_time), 0, 0, 0, 0);
	// - 여섯번째 컬럼 -
	if(NULL!=lap_time) this->SetItem(idx, 5, LVIF_TEXT, W2T(lap_time), 0, 0, 0, 0);
#endif
}

// 문자열 시간의 차이를 구함
bool CSRTView::DiffStringTime(LPCTSTR postStringTime,LPCTSTR preStringTime,TCHAR* diffStringTime)
{
	// 문자열 -> SYSTEMTIME에 넣고
	SYSTEMTIME stPost, stPre;
	StringTimeToSystemTime(postStringTime,stPost);
	StringTimeToSystemTime(preStringTime,stPre);

	// FILETIME으로 변환한 이후
	FILETIME ftPost, ftPre;
	::SystemTimeToFileTime(&stPost, &ftPost);
	::SystemTimeToFileTime(&stPre, &ftPre);

	// 차이를 64비트 정수로 해서 계산
	ULARGE_INTEGER ulPost, ulPre, ulDiff;
	ulPost.HighPart	= ftPost.dwHighDateTime;
	ulPost.LowPart	= ftPost.dwLowDateTime;

	ulPre.HighPart	= ftPre.dwHighDateTime;
	ulPre.LowPart	= ftPre.dwLowDateTime;

	bool isPositive = (ulPost.QuadPart >= ulPre.QuadPart);
	if(isPositive) ulDiff.QuadPart	= ulPost.QuadPart - ulPre.QuadPart;
	else ulDiff.QuadPart = ulPre.QuadPart - ulPost.QuadPart;

	// 계산 결과를 FILETIME을 SYSTEMTIME으로 변환
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

// 리스트뷰의 내용을 유니코드로 저장
void CSRTView::SaveWithUnicode(HANDLE hFile)
{
	DWORD dwWritten;	// 단지 변수로 사용하기 위함

	// BOM 기록
	::WriteFile(hFile, BOM_UTF16LE, SIZE_OF_BOM_UTF16LE, &dwWritten, NULL);

	// 현재 아이템 개수
	int idx = this->GetItemCount();
	for(int i=0; i<idx; ++i) {
		for(int j=0; j<SUB_ITEM_NUM; ++j) {
			if(0!=j) ::WriteFile(hFile, TabCharWide, sizeof(WCHAR), &dwWritten, NULL);	// 탭문자

			BSTR bstrText = NULL;
			this->GetItemText(i, j, bstrText);
			if(NULL!=bstrText) {
				::WriteFile(hFile, bstrText, wcslen(bstrText)*sizeof(OLECHAR), &dwWritten, NULL);
				::SysFreeString(bstrText);
			}
		}	
		::WriteFile(hFile, CrLfWide, SIZE_OF_TWO_CHAR*sizeof(WCHAR), &dwWritten, NULL);	// 줄바꿈
	}
}

// 유니코드의 파일을 리스트뷰로 불러옴
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
			getTokensWithVector<wstring>(line, L"\t", vec);	// 한줄을 탭문자로 분리
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
				default:	// 탭 구분되어 나온 문자들이 6개이상
					this->InsertOneRecord(vec[0].c_str(), vec[1].c_str(), vec[2].c_str(), vec[3].c_str(), vec[4].c_str(), vec[5].c_str());
					break;
				}
			}
		}
	}
}
