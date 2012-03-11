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

		// ���� & ���� ����
		bool openWithDsnFile(LPTSTR dns_filename);	// T���ڿ� ��
		bool openWithDsnFileW(LPWSTR dns_filename);	// UNICODE ��
		bool openWithDsnFileA(LPSTR  dns_filename);	// ANSI���ڿ�

		bool open(LPTSTR	dns_filename);	// T���ڿ� ��
		bool openW(LPWSTR	dns_filename);	// UNICODE ��
		bool openA(LPSTR	dns_filename);	// ANSI���ڿ�

		void close();

		// ��� ���� ����
		bool sqlExecDirect(LPTSTR	szSQL);	// T���ڿ� ��
		bool sqlExecDirectW(LPWSTR	szSQL);	// UNICODE ��
		bool sqlExecDirectA(LPSTR	szSQL);	// ANSI���ڿ�

		void closeExeute();
		bool bindCol(SQLUSMALLINT colNum, SQLSMALLINT type, SQLPOINTER value, SQLINTEGER bufLen, SQLINTEGER* len_Ind);
		bool sqlFetch();

		// ���Ṯ�ڿ� ����
		void getConnStrW(__out std::wstring& str);
		void getConnStrA(__out std::string& str);

		// ���� SQL ���� �ڵ�
		SQLRETURN getLastReturn()			{	return m_sqlReturn;			}

		// diagnostic record ����
		bool		isExistLastDiag()		{	return m_diagRec.empty();	}
		DWORD		getLastDiagRecNumber()	{	return m_diagRec.size();	}
		DiagRecVecW	getLastDiagRecVecW()	{	return m_diagRec;			}
		bool		getErrorMsgAtW(__in DWORD i, __out std::wstring& str);
		bool		getErrorMsgAtA(__in DWORD i, __out std::string& str);
	private:
		// �ڵ�
		SQLHENV m_hEnv;
		SQLHDBC m_hDbc;
		SQLHSTMT m_hStmt;

		// ���� ���� ����
		SQLWCHAR OutCon[1024];	// Applications should allocate at least 1,024 characters for this buffer.
								// ref. http://msdn.microsoft.com/en-us/library/ms715433(VS.85).aspx
		SQLSMALLINT cbOutCon;

		// ��ġ ���� ����
		bool m_isFetched;

		// ���� ��� ����
		SQLRETURN m_sqlReturn;
		DiagRecVecW m_diagRec;

		// �޼ҵ�
		void setLocalDiagRect(SQLSMALLINT fHandleType, SQLHANDLE handle);
	};

}	/* namespace DB */