#pragma once
#include "unicode.h"
#include <atldbcli.h>

enum QUERY_LIST_ENUM
{
	QUERY_GET_DEVICECOUNTER	= 0
};

namespace namo {
class MDBConnection
{
public:
	MDBConnection(void);
	~MDBConnection(void);
	bool open(_STRING mdbFileName);
	void close();
	bool insertOne() {	// for TEST
		if(!isOpenedDB) {
			setLastErrorMsg(L"DB not opened");
			return false;
		}
		CCommand<CNoAccessor, CNoRowset> cmd;
		_STRING strCommand(_T("insert into OID (id, item_category, item_name, item_oid, item_array, item_size, item_private) values (12, 'etc', 'test', '1.2.3', 8, 8, 8);"));
		DBROWCOUNT affectedRowCount = 1;
		HRESULT hr = cmd.Open(m_session, strCommand.c_str(), NULL, &affectedRowCount, DBGUID_DEFAULT, false);
		if(FAILED(hr)) setLastErrorMsg();

		std::cout << "affected :" << affectedRowCount << std::endl;
		return (SUCCEEDED(hr));
	}
	std::wstring getDeviceCounterString();
	std::wstring getLastErrorMsg();
private:
	bool isOpenedDB;
	CDataSource m_ds;
	CSession m_session;
	std::wstring lastErrorMsg;
	void setLastErrorMsg();
	void setLastErrorMsg(std::wstring msg);
	_STRING getQuery(int queryEnum); 
};
} // namespace namo