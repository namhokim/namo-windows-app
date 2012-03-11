// RunCheck.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <string>
#include <vector>

using namespace std;

const int buf_len = 26+1;
const size_t init_alloc_size = 1;

inline string GetCurrTimeA();
inline wstring GetCurrTimeW();
string GetLogMessage(const char * pszFormat = NULL, ...);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	ofstream w("C:\\RunCheck.log", ios::out | ios::app);
	if(w.fail()) {
		return EXIT_FAILURE;
	}

	w << GetLogMessage("excuted") << endl;

	w.close();

	return EXIT_SUCCESS;
}


inline string GetCurrTimeA() {
	// Get Time
	SYSTEMTIME st;
	::GetLocalTime(&st);

	// Format
	CHAR buf[buf_len] = {0};
	wsprintfA(buf, "%04d-%02d-%02d %02d:%02d:%02d.%03d - ",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	// Return
	return buf;
}

inline wstring GetCurrTimeW() {
	// Get Time
	SYSTEMTIME st;
	::GetLocalTime(&st);

	// Format
	WCHAR buf[buf_len] = {0};
	wsprintfW(buf, L"%04d-%02d-%02d %02d:%02d:%02d.%03d - ",
		st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	// Return
	return buf;
}

#include <strsafe.h>
string GetLogMessage(const char * pszFormat, ...)
{
	string msg(GetCurrTimeA());
	// Parameter check
	if(NULL==pszFormat) {
		return msg;
	}

	// start of format argument list
	va_list argList;
	va_start(argList, pszFormat);

	// Convert
	size_t buf_size = init_alloc_size;
	size_t size_str = STRSAFE_MAX_CCH;
	HRESULT hr = S_FALSE;

	while ( S_OK != hr ) {	// ���������� ��ȯ�� �� �� ���� �ݺ�����
		std::vector<char> buf(buf_size);
		size_t remaining;

		hr = ::StringCchVPrintfExA(
			reinterpret_cast<STRSAFE_LPSTR>(&buf[0]),
			buf_size,
			NULL,
			&remaining,
			STRSAFE_NO_TRUNCATION, 
			pszFormat, argList
			);

		if ( SUCCEEDED(hr) ) {	// ����
			size_str = buf_size - remaining;
			std::string szOutput( buf.begin(), (buf.begin()+size_str) );
			msg.append(&buf[0], buf.size());
			break;
		} else if ( STRSAFE_E_INSUFFICIENT_BUFFER==hr ) {
			buf_size *= 2;	// 2��� ���۱��� ����
		}else {
			// STRSAFE_E_INVALID_PARAMETER
			::OutputDebugStringA( "Namo:OutputDebugStringA Error" );	// ���� �Լ�(Windows API)
			break;
		}
	}

	// end of format argument list
	va_end(argList);

	return msg;
}
