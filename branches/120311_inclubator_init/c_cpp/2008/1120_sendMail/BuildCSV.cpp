#include "BuildCSV.h"
#include "Config.h"
#include "MDBConnection.h"
#include <vector>

namespace namo {
BuildCSV::BuildCSV(void)
:outputFileName(_T("")), isSuccessOutputCSV(false), lastErrorMsg(L"")
{
}

BuildCSV::~BuildCSV(void)
{
}

_STRING BuildCSV::getFileName() {
	return outputFileName;
}

bool BuildCSV::make()
{
	_STRING csvFileName = BuildCSV::GetCSVName();

	// DB 쿼리를 통해서
	MDBConnection db;
	Config config;
	
	bool resultConn = db.open(config.getMDBPathAndFileName());
	if(!resultConn) BuildCSV::setLastErrorMsg(db.getLastErrorMsg());
	// 데이터를 가져오는 쿼리 (객체의 리스트 형태 든)
	std::wstring content= db.getDeviceCounterString();
	db.close();

	if(content.size() == 0) isSuccessOutputCSV = false;
	else {	// 파일 저장
		isSuccessOutputCSV = saveToFile(content);
	}
	
	// 그것을 CSV로 가공 파일로 출력
	return isSuccessOutputCSV;
	// 최종 결과가 false이면 csvFileName는 ""로 다시 바뀌어야 한다.(아니면 나중에 set하던지)
}

bool BuildCSV::remove()
{
	if(outputFileName.size() > 0) {
		return (DeleteFile(outputFileName.c_str())==TRUE) ? true : false;
	}
	return false;
}

//#include <windows.h>	// for GetLocalTime
_STRING BuildCSV::GetCSVName()
{
	_STRING resultPathAndFilename;

	// get Current Path
	TCHAR currentPath[MAX_PATH];
	_TGETCWD(currentPath,MAX_PATH);
	resultPathAndFilename = currentPath;
	resultPathAndFilename.append(_T("\\"));

	const int dataLength = 255;
	_TCHAR sTime [dataLength];
	// get File Name
	SYSTEMTIME st;
	GetLocalTime(&st);
	wsprintf(sTime,_T("%d%02d%02d%d%d%d%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	_STRING currentTime(sTime);

	resultPathAndFilename.append(currentTime.substr(2,13).append(_T(".csv")));
	
	return resultPathAndFilename;
}

bool BuildCSV::saveToFile(std::wstring &content)
{
	HANDLE hFile;
	_STRING filename(GetCSVName());
	hFile = CreateFile(filename.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN, NULL);
	if(INVALID_HANDLE_VALUE == hFile) return FALSE;

	DWORD writtenLength;
	std::string outputContentMultiByte = wcs_to_mbs(content);
	BOOL resWriteFile = WriteFile(hFile, 
		outputContentMultiByte.c_str(), (DWORD)outputContentMultiByte.size(), &writtenLength, NULL);
	if(resWriteFile) {
		outputFileName = filename;
	}
	else outputFileName = _T("");
	CloseHandle(hFile);
	
	return (resWriteFile==TRUE) ?  true : false;
}

std::string
BuildCSV::wcs_to_mbs(std::wstring const& str, std::locale const& loc)
{
    typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
    codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
    std::mbstate_t state = 0;
    std::vector<char> buf((str.size() + 1) * codecvt.max_length());
    wchar_t const* in_next = str.c_str();
    char* out_next = &buf[0];
    codecvt_t::result r = codecvt.out(state,
        str.c_str(), str.c_str() + str.size(), in_next,
        &buf[0], &buf[0] + buf.size(), out_next);
    return std::string(&buf[0]);
}

std::wstring
BuildCSV::mbs_to_wcs(std::string const& str, std::locale const& loc)
{
    typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
    codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
    std::mbstate_t state = 0;
    std::vector<wchar_t> buf(str.size() + 1);
    char const* in_next = str.c_str();
    wchar_t* out_next = &buf[0];
    codecvt_t::result r = codecvt.in(state,
        str.c_str(), str.c_str() + str.size(), in_next,
        &buf[0], &buf[0] + buf.size(), out_next);
    return std::wstring(&buf[0]);
}

std::wstring BuildCSV::getLastErrorMsg()
{
	return lastErrorMsg;
}

void BuildCSV::setLastErrorMsg(std::wstring msg)
{
	lastErrorMsg = msg;
}
} // namespace namo