#include "ODBC_Connection.h"
#include <tchar.h>

LPTSTR ConnStr = TEXT("Description=KT_DB;DRIVER=SQL Server;SERVER=SINDO3;UID=sa;PWD=solution;APP=Microsoft Data Access Components;WSID=김남호;DATABASE=SINDOH;Network=DBMSSOCN");
LPWSTR ConnStrW = L"Description=KT_DB;DRIVER=SQL Server;SERVER=SINDO3;UID=sa;PWD=solution;APP=Microsoft Data Access Components;WSID=김남호;DATABASE=SINDOH;Network=DBMSSOCN";
LPSTR ConnStrA = "Description=KT_DB;DRIVER=SQL Server;SERVER=SINDO3;UID=sa;PWD=solution;APP=Microsoft Data Access Components;WSID=김남호;DATABASE=SINDOH;Network=DBMSSOCN";

LPTSTR Q_select = TEXT("SELECT user_id, user_name, password, dept_id, email FROM SINDOH.dbo.user_info;");
LPWSTR Q_selectW = L"SELECT user_id, user_name, password, dept_id, email FROM SINDOH.dbo.user_info;";
LPSTR Q_selectA = "SELECT user_id, user_name, password, dept_id, email FROM SINDOH.dbo.user_info;";
void DB_get_test();

#define	CONVERT_APP

#ifdef CONVERT_APP

#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 파일입력 다이얼로그
	TCHAR lpstrFile[MAX_PATH] = {0, };

	OPENFILENAME ofn;
	::memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;	// 바탕화면
	ofn.lpstrFilter = TEXT("DSN파일(*.dsn)\0*.dsn\0모든 파일(*.*)\0*.*\0");
	ofn.lpstrFile = lpstrFile;
	ofn.nMaxFile = MAX_PATH;

	if (::GetOpenFileName(&ofn)) {
		DB::ODBC_Connection conn;
		if(conn.openWithDsnFile(lpstrFile)) {
			std::string con_str;
			conn.getConnStrA(con_str);
			MessageBoxA(NULL, con_str.c_str(),
				"연결성공 (Ctrl-C해서 메모장에 붙여넣으세요)", MB_OK);
		} else {
			TCHAR msg[1024] = {0, };
			wsprintf(msg, TEXT("연결코드 : %d\n에러내용 : \n%s"),
				conn.getLastReturn(), conn.getLastDiagRecVecW().at(0).szErrorMsg.c_str());
			MessageBox(NULL, msg, TEXT("에러"), MB_OK);
		}
	} else {
		MessageBox(NULL, TEXT("DSN 파일을 선택하세요"), TEXT("사용법"), MB_OK);
	}

#else	// 릴리즈 모드일 때는 DSN파일 입력 받아서 연결문자열 출력하는 윈도우 프로그램으로..

int _tmain(int argc, _TCHAR* argv[]) {
	DB_get_test();

#endif
	return 0;
}

// 디버그 모드일 때만 아래 함수
void DB_get_test()
{
	DB::ODBC_Connection conn;
//	if(conn.openWithDsnFile(TEXT("C:\\input\\sqlsv.dsn"))) {
	if(conn.open(ConnStr)) {
		conn.sqlExecDirect(Q_select);

		SQLCHAR user_id[50];
		SQLWCHAR user_name[50];
		SQLINTEGER lUserId, lUserName;
		conn.bindCol(1, SQL_C_CHAR, user_id, sizeof(user_id), &lUserId);
		conn.bindCol(2, SQL_C_WCHAR, user_name, sizeof(user_name), &lUserName);
		while(conn.sqlFetch()) {
			printf("%s\n", user_id);
		}
		conn.closeExeute();
		conn.close();
	}
}