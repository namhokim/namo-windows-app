/************** Begin file ODBC_Connection.h ***************************************/
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
** ODBC_Connection : Wrapper Class declare for ODBC.
** Author : Namho Kim
**
*/

#pragma once

#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <vector>

///////////////////////////////////////////////////////////////////////////////

//#ifndef _TSTRING_H_
//#define _TSTRING_H_
//
//#include <string>
//#include <tchar.h>	// for _T
//
//namespace std {
//	typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> >	tstring;
//	typedef basic_stringstream<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstringstream;
//}
//#endif	/* _TSTRING_H_ */

///////////////////////////////////////////////////////////////////////////////

namespace DB {
	typedef struct _DiagnosticRecordW {
		SQLWCHAR szSqlState[5+1];
		SQLINTEGER lNativeError;
		std::wstring szErrorMsg;
	} DiagnosticRecordW, *lpDiagnosticRecordW;

	typedef std::vector<DiagnosticRecordW>	DiagRecVecW;

	class ODBC_Connection
	{
	public:
		ODBC_Connection(void);
		~ODBC_Connection(void);

		// 연결 & 종료 관련
		bool openWithDsnFile(LPTSTR dns_filename);	// T문자열 용
		bool openWithDsnFileW(LPWSTR dns_filename);	// UNICODE 용
		bool openWithDsnFileA(LPSTR  dns_filename);	// ANSI문자용

		bool open(LPTSTR	dns_filename);	// T문자열 용
		bool openW(LPWSTR	dns_filename);	// UNICODE 용
		bool openA(LPSTR	dns_filename);	// ANSI문자용

		void close();

		// 명령 실행 관련
		bool sqlExecDirect(LPTSTR	szSQL);	// T문자열 용
		bool sqlExecDirectW(LPWSTR	szSQL);	// UNICODE 용
		bool sqlExecDirectA(LPSTR	szSQL);	// ANSI문자용

		void closeExeute();
		bool bindCol(SQLUSMALLINT colNum, SQLSMALLINT type, SQLPOINTER value, SQLINTEGER bufLen, SQLINTEGER* len_Ind);
		bool sqlFetch();

		// 연결문자열 관련
		void getConnStrW(__out std::wstring& str);
		void getConnStrA(__out std::string& str);

		// 최종 SQL 수행 코드
		SQLRETURN getLastReturn()			{	return m_sqlReturn;			}

		// diagnostic record 관련
		bool		isExistLastDiag()		{	return m_diagRec.empty();	}
		DWORD		getLastDiagRecNumber()	{	return m_diagRec.size();	}
		DiagRecVecW	getLastDiagRecVecW()	{	return m_diagRec;			}
		bool		getErrorMsgAtW(__in DWORD i, __out std::wstring& str);
		bool		getErrorMsgAtA(__in DWORD i, __out std::string& str);
	private:
		// 핸들
		SQLHENV m_hEnv;
		SQLHDBC m_hDbc;
		SQLHSTMT m_hStmt;

		// 연결 설정 변수
		SQLWCHAR OutCon[1024];	// Applications should allocate at least 1,024 characters for this buffer.
								// ref. http://msdn.microsoft.com/en-us/library/ms715433(VS.85).aspx
		SQLSMALLINT cbOutCon;

		// 패치 수행 여부
		bool m_isFetched;

		// 수행 결과 정보
		SQLRETURN m_sqlReturn;
		DiagRecVecW m_diagRec;

		// 메소드
		void setLocalDiagRect(SQLSMALLINT fHandleType, SQLHANDLE handle);
	};

}	/* namespace DB */