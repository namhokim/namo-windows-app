/************** Begin file ODBC_Connection.cpp ***************************************/
/*
** 2010 April 22
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** ODBC_Connection : Wrapper Class define for ODBC.
** Author : Namho Kim
**
*/

#include "ODBC_Connection.h"
#include <mbstring.h>	// for _mbslen
#include <locale>		// for std::codecvt in getErrorMsgAtA

namespace DB {

///////////////////////////////////////////////////////////////////////////////

ODBC_Connection::ODBC_Connection(void)
: m_hEnv(SQL_NULL_HENV), m_hDbc(SQL_NULL_HDBC), m_hStmt(SQL_NULL_HSTMT), m_isFetched(false)
{
	// Allocate environment handle
	// Returns : SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_INVALID_HANDLE, or SQL_ERROR
	m_sqlReturn = ::SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);

	// Set the ODBC version environment attribute
	if(SQL_SUCCESS==m_sqlReturn || SQL_SUCCESS_WITH_INFO==m_sqlReturn) {
		// Returns : SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, or SQL_INVALID_HANDLE
		m_sqlReturn = ::SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	} else {
		m_hEnv = SQL_NULL_HENV;
	}
}

ODBC_Connection::~ODBC_Connection(void)
{
	closeExeute();
	close();
	if(SQL_NULL_HENV!=m_hEnv) {
		// Returns : SQL_SUCCESS, SQL_ERROR, or SQL_INVALID_HANDLE
		::SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		m_hEnv = SQL_NULL_HENV;
	}
}

///////////////////////////////////////////////////////////////////////////////

bool ODBC_Connection::openWithDsnFile(LPTSTR dns_filename)
{
#ifdef	UNICODE
	return openWithDsnFileW(dns_filename);
#else
	return openWithDsnFileA(dns_filename);
#endif
}

bool ODBC_Connection::openWithDsnFileW(LPWSTR dns_filename)
{
	// Assure disconnect pre-connection
	close();

	// Allocate connection handle
	m_sqlReturn = ::SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
	if(SQL_SUCCESS==m_sqlReturn || SQL_SUCCESS_WITH_INFO==m_sqlReturn) {
		// Set login timeout to 5 seconds
		// Returns : SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, SQL_INVALID_HANDLE, or SQL_STILL_EXECUTING
		m_sqlReturn = SQLSetConnectAttrW(m_hDbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

		// Make FILEDSN Connection String
		std::wstring connStr(L"FILEDSN=");
		connStr.append(dns_filename);

		// Connect to data source
		// http://msdn.microsoft.com/en-us/library/ms715433(VS.85).aspx
		// Returns : SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NO_DATA, SQL_ERROR, SQL_INVALID_HANDLE, or SQL_STILL_EXECUTING
		m_sqlReturn = ::SQLDriverConnectW(m_hDbc, NULL,
			const_cast<SQLWCHAR *>(connStr.c_str()), connStr.size(), OutCon, 1024, &cbOutCon, SQL_DRIVER_NOPROMPT);

		if(SQL_SUCCESS_WITH_INFO==m_sqlReturn) {
			setLocalDiagRect(SQL_HANDLE_DBC, m_hDbc);
			return true;
		} else if(SQL_ERROR==m_sqlReturn) {
			setLocalDiagRect(SQL_HANDLE_DBC, m_hDbc);
			return false;
		} else {	// assume SQL_SUCCESS
			return true;
		}
	} else {
		setLocalDiagRect(SQL_HANDLE_ENV, m_hEnv);
		return false;
	}
}

bool ODBC_Connection::openWithDsnFileA(LPSTR dns_filename)
{
	// 문자열 변환 수행후 유니코드 메소드 호출
	int mbStrLen, wbStrLen;

	mbStrLen = ::strlen(dns_filename);
	// 길이 구하고
	wbStrLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, dns_filename, mbStrLen, NULL, 0);
	std::vector<WCHAR> vec_buf(wbStrLen+1);
	// 버퍼에 담아옴
	wbStrLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, dns_filename, mbStrLen, &vec_buf[0], mbStrLen);

	return openWithDsnFileW(&vec_buf[0]);
}

///////////////////////////////////////////////////////////////////////////////

bool ODBC_Connection::open(LPTSTR dns_filename)
{
#ifdef	UNICODE
	return openW(dns_filename);
#else
	return openA(dns_filename);
#endif
}

bool ODBC_Connection::openW(LPWSTR dns_filename)
{
	// Assure disconnect pre-connection
	close();

	// Allocate connection handle
	m_sqlReturn = ::SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
	if(SQL_SUCCESS==m_sqlReturn || SQL_SUCCESS_WITH_INFO==m_sqlReturn) {
		// Set login timeout to 5 seconds
		// Returns : SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, SQL_INVALID_HANDLE, or SQL_STILL_EXECUTING
		m_sqlReturn = SQLSetConnectAttrW(m_hDbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

		// Connect to data source
		// http://msdn.microsoft.com/en-us/library/ms715433(VS.85).aspx
		// Returns : SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NO_DATA, SQL_ERROR, SQL_INVALID_HANDLE, or SQL_STILL_EXECUTING
		m_sqlReturn = ::SQLDriverConnectW(m_hDbc, NULL,
			dns_filename, wcslen(dns_filename), OutCon, 1024, &cbOutCon, SQL_DRIVER_NOPROMPT);

		if(SQL_SUCCESS_WITH_INFO==m_sqlReturn) {
			setLocalDiagRect(SQL_HANDLE_DBC, m_hDbc);
			return true;
		} else if(SQL_ERROR==m_sqlReturn) {
			setLocalDiagRect(SQL_HANDLE_DBC, m_hDbc);
			return false;
		} else {	// assume SQL_SUCCESS
			return true;
		}
	} else {
		setLocalDiagRect(SQL_HANDLE_ENV, m_hEnv);
		return false;
	}
}

bool ODBC_Connection::openA(LPSTR dns_filename)
{
	// 문자열 변환 수행후 유니코드 메소드 호출
	int mbStrLen, wbStrLen;

	mbStrLen = ::strlen(dns_filename);
	// 길이 구하고
	wbStrLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, dns_filename, mbStrLen, NULL, 0);
	std::vector<WCHAR> vec_buf(wbStrLen+1);
	// 버퍼에 담아옴
	wbStrLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, dns_filename, mbStrLen, &vec_buf[0], mbStrLen);

	return openW(&vec_buf[0]);
}

///////////////////////////////////////////////////////////////////////////////

void ODBC_Connection::close()
{
	closeExeute();
	if(SQL_NULL_HDBC!=m_hDbc) {
		::SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
		m_hDbc = SQL_NULL_HDBC;
	}
}

///////////////////////////////////////////////////////////////////////////////

bool ODBC_Connection::sqlExecDirect(LPTSTR szSQL)
{
#ifdef	UNICODE
	return sqlExecDirectW(szSQL);
#else
	return sqlExecDirectA(szSQL);
#endif
}

bool ODBC_Connection::sqlExecDirectW(LPWSTR szSQL)
{
	// 이전 수행 statement를 닫음
	closeExeute();

	// 명령 핸들을 할당
	m_sqlReturn = ::SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
	if(SQL_SUCCESS==m_sqlReturn) {
		// Returns : SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NEED_DATA, SQL_STILL_EXECUTING, SQL_ERROR, SQL_NO_DATA, SQL_INVALID_HANDLE, or SQL_PARAM_DATA_AVAILABLE
		m_sqlReturn = ::SQLExecDirectW( m_hStmt, (SQLWCHAR*)szSQL, (SQLINTEGER)::wcslen(szSQL) );
		if(SQL_SUCCESS == m_sqlReturn) {
			return true;
		} else if (SQL_SUCCESS_WITH_INFO==m_sqlReturn) {
			setLocalDiagRect(SQL_HANDLE_STMT, m_hStmt);
			return true;
		} else {
			return true;
		}
	} else if(SQL_SUCCESS_WITH_INFO==m_sqlReturn || SQL_ERROR==m_sqlReturn) {	// ?
		setLocalDiagRect(SQL_HANDLE_STMT, m_hStmt);
		return false;
	} else {
		return false;
	}
}

bool ODBC_Connection::sqlExecDirectA(LPSTR szSQL)
{
	// 문자열 변환 수행후 유니코드 메소드 호출
	int mbStrLen, wbStrLen;

	mbStrLen = ::strlen(szSQL);
	// 길이 구하고
	wbStrLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szSQL, mbStrLen, NULL, 0);
	std::vector<WCHAR> vec_buf(wbStrLen+1);
	// 버퍼에 담아옴
	wbStrLen = ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szSQL, mbStrLen, &vec_buf[0], mbStrLen);

	return sqlExecDirectW(&vec_buf[0]);
}

///////////////////////////////////////////////////////////////////////////////


void ODBC_Connection::closeExeute()
{
	// 커서를 닫아주고(만약 패치를 했다면)
	if(m_isFetched) {
		// Returns : SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, or SQL_INVALID_HANDLE
		::SQLCloseCursor(m_hStmt);
		m_isFetched = false;
	}

	if(SQL_NULL_HSTMT!=m_hStmt) {
		::SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
		m_hStmt = SQL_NULL_HSTMT;
	}
}

// http://msdn.microsoft.com/en-us/library/ms714556(VS.85).aspx
bool ODBC_Connection::bindCol(SQLUSMALLINT colNum, SQLSMALLINT type, SQLPOINTER value, SQLINTEGER bufLen, SQLINTEGER* len_Ind)
{
	// Returns : SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, or SQL_INVALID_HANDLE
	m_sqlReturn = ::SQLBindCol(m_hStmt, colNum, type, value, bufLen, len_Ind);
	if(SQL_SUCCESS==m_sqlReturn) {
		return true;
	} else if(SQL_SUCCESS_WITH_INFO==m_sqlReturn) {
		setLocalDiagRect(SQL_HANDLE_STMT, m_hStmt);
		return true;
	} else {
		setLocalDiagRect(SQL_HANDLE_STMT, m_hStmt);
		return false;
	}
}

bool ODBC_Connection::sqlFetch()
{
	// Returns : SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_NO_DATA, SQL_STILL_EXECUTING, SQL_ERROR, or SQL_INVALID_HANDLE
	m_sqlReturn = ::SQLFetch(m_hStmt);
	return (SQL_NO_DATA != m_sqlReturn);
}

///////////////////////////////////////////////////////////////////////////////

void ODBC_Connection::getConnStrW(__out std::wstring& str)
{
	str.reserve(wcslen(OutCon));
	str.assign(OutCon);
}

void ODBC_Connection::getConnStrA(__out std::string& str)
{
	// 내부 저장하고 있는 유니코드 문자열을 ANSI문자열로 변환해서 값 반환
	int wbStrLen, mbStrLen;

	wbStrLen = ::wcslen(OutCon);
	// 길이 구하고
	BOOL bUsedDefaultChar;
	mbStrLen = ::WideCharToMultiByte(CP_ACP, 0, OutCon, wbStrLen, NULL, 0, "?", &bUsedDefaultChar);
	std::vector<CHAR> vec_buf(mbStrLen+1);
	// 버퍼에 담아옴
	mbStrLen = ::WideCharToMultiByte(CP_ACP, 0, OutCon, wbStrLen, &vec_buf[0], mbStrLen, "?", &bUsedDefaultChar);

	str.reserve(mbStrLen);
	str.assign(vec_buf.begin(), vec_buf.end());
}

bool ODBC_Connection::getErrorMsgAtW(__in DWORD i, __out std::wstring& str)
{
	try {
		str.reserve(m_diagRec.at(i-1).szErrorMsg.size());
		str.assign(m_diagRec.at(i-1).szErrorMsg);
		return true;
	} catch(std::out_of_range) {
		return false;
	}
}

bool ODBC_Connection::getErrorMsgAtA(__in DWORD i, __out std::string& str)
{
	try {
		std::wstring& ref_wstr = m_diagRec.at(i-1).szErrorMsg;

		// 내부 저장하고 있는 유니코드 문자열을 ANSI문자열로 변환해서 값 반환
		int wbStrLen, mbStrLen;

		wbStrLen = ref_wstr.size();
		// 길이 구하고
		BOOL bUsedDefaultChar;
		mbStrLen = ::WideCharToMultiByte(CP_ACP, 0, ref_wstr.c_str(), wbStrLen, NULL, 0, "?", &bUsedDefaultChar);
		std::vector<CHAR> vec_buf(mbStrLen+1);
		// 버퍼에 담아옴
		mbStrLen = ::WideCharToMultiByte(CP_ACP, 0, ref_wstr.c_str(), wbStrLen, &vec_buf[0], mbStrLen, "?", &bUsedDefaultChar);

		str.reserve(mbStrLen);
		str.assign(vec_buf.begin(), vec_buf.end());
		return true;
	} catch(std::out_of_range) {
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// PRAVATE SCOPE:
void ODBC_Connection::setLocalDiagRect(SQLSMALLINT fHandleType, SQLHANDLE handle)
{
	SQLRETURN r;
	DiagnosticRecordW rec;
	SQLSMALLINT i, MsgLen;
	m_diagRec.clear();		// 빈 컨테이너로 만든다
	
	i = 1;

	// Returns : SQL_SUCCESS, SQL_SUCCESS_WITH_INFO, SQL_ERROR, or SQL_INVALID_HANDLE
	while ( SQL_NO_DATA !=
		(r = ::SQLGetDiagRecW(fHandleType, handle, i, rec.szSqlState, &(rec.lNativeError), NULL, 0, &MsgLen)) ) {
			std::vector<WCHAR> msg_buf(MsgLen);
			r = ::SQLGetDiagRecW(fHandleType, handle, i, rec.szSqlState, &(rec.lNativeError), &msg_buf[0], MsgLen, &MsgLen);
			rec.szErrorMsg.assign(msg_buf.begin(), msg_buf.end());
			m_diagRec.push_back(rec);
			i++;
	}
}

///////////////////////////////////////////////////////////////////////////////

}	/* namespace DB */