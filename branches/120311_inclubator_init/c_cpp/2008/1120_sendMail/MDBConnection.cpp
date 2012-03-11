#include "MDBConnection.h"

namespace namo {
MDBConnection::MDBConnection(void)
:isOpenedDB(false)
{
	CoInitialize(S_OK);
}

MDBConnection::~MDBConnection(void)
{
	if(isOpenedDB) MDBConnection::close();
	CoUninitialize();
}

bool MDBConnection::open(_STRING mdbFileName)
{
	CLSID clsid = {0xdee35070L,0x506b,0x11cf,{0xb1,0xaa,0x0,0xaa,0x0,0xb8,0xde,0x95}};
	HRESULT hr = m_ds.Open(clsid, mdbFileName.c_str(),NULL, NULL, DB_MODE_SHARE_EXCLUSIVE);
	if(SUCCEEDED(hr)) {
		m_session.Open(m_ds);
	}
	if(SUCCEEDED(hr)) isOpenedDB = true;
	else {
		isOpenedDB = false;
		setLastErrorMsg();
	}
	return isOpenedDB;
}

void MDBConnection::close()
{
	m_session.Close();
	m_ds.Close();
	isOpenedDB = false;
}

std::wstring MDBConnection::getLastErrorMsg() {
	return lastErrorMsg;
}

void MDBConnection::setLastErrorMsg() {
	IErrorInfo* pErrInfo;
	HRESULT hr = ::GetErrorInfo(0, &pErrInfo);
	if(SUCCEEDED(hr))
	{
		BSTR bstrErrDescription;
		pErrInfo->GetDescription(&bstrErrDescription);
		lastErrorMsg = bstrErrDescription;
		pErrInfo->Release();
		::SysFreeString(bstrErrDescription);
	}
	else lastErrorMsg = L"Error catch failed.";
}

void MDBConnection::setLastErrorMsg(std::wstring msg)
{
	lastErrorMsg = msg;
}

std::wstring MDBConnection::getDeviceCounterString()
{
	std::wstring resultDeviceCounter(L"");

	if(!isOpenedDB) {
		setLastErrorMsg(L"DB not opened");
		return resultDeviceCounter;
	}
	CCommand<CDynamicAccessor, CRowset, CNoMultipleResults> cmd;
	HRESULT hr = cmd.Open(m_session, getQuery(QUERY_GET_DEVICECOUNTER).c_str(), NULL, NULL, DBGUID_DEFAULT, true);
	if(FAILED(hr)) {
		setLastErrorMsg();
		return resultDeviceCounter;
	}
	WCHAR szStringBuffer[255];
	ULONG columnCount = cmd.GetColumnCount();
	// title
	resultDeviceCounter.append(L"#\n");
	for(ULONG i = 0 ; i< columnCount; i++) {
		resultDeviceCounter.append(L"\"");
		resultDeviceCounter.append(cmd.GetColumnName(i+1));
		resultDeviceCounter.append(L"\"");
		if(i<columnCount-1) resultDeviceCounter.append(L",");
		else resultDeviceCounter.append(L"\n");
	}
	DBTYPE columnType;
	// data field
	while(cmd.MoveNext() == S_OK) {
		for(ULONG i = 0 ; i< columnCount; i++) {
			resultDeviceCounter.append(L"\"");
			cmd.GetColumnType(i+1, &columnType);
			if(columnType == DBTYPE_I4) {
				wsprintfW(szStringBuffer, L"%d", *((INT *)cmd.GetValue(i+1)));
			}
			else  wsprintfW(szStringBuffer, L"%s", (WCHAR *)cmd.GetValue(i+1));
			resultDeviceCounter.append(szStringBuffer);
			resultDeviceCounter.append(L"\"");
			if(i<columnCount-1) resultDeviceCounter.append(L",");
			else resultDeviceCounter.append(L"\n");
		}
	}
	cmd.Close();
	return resultDeviceCounter;
}

_STRING MDBConnection::getQuery(int queryEnum)
{
	switch(queryEnum) {
		case QUERY_GET_DEVICECOUNTER:
			return _T("SELECT ")
				_T("d.ipAddress, d.modelName, d.phyaddr, d.serialNumber, ")
				_T("dc.CTotalCount, dc.CopyCountertotal, dc.CopyCounterblack, dc.CopyCounterfull, dc.CopyCountermono, dc.CopyCountertwin, dc.PrinterCountertotal, dc.PrinterCounterblack, dc.PrinterCounterfull, dc.PrinterCountermono, dc.PrinterCountertwin, dc.LevelPrintColor, dc.FaxCountertotal, dc.FaxCounterblack, dc.FaxCountermono, dc.sendMonoTotal, dc.sendColorTotal, dc.faxSendCounterblack, dc.scanSendCounterblack, dc.scanSendCounterfull ")
				_T("FROM device AS d, deviceCounter AS dc WHERE d.phyaddr=dc.phyaddr;");
		default:
			return _T("");
	}
}

} // namespace namo