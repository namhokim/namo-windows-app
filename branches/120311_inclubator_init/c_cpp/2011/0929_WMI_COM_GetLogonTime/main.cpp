#include <iostream>
using namespace std;

#include <wbemidl.h>
# pragma comment(lib, "wbemuuid.lib")

HRESULT InitWMI();
HRESULT ObtainInitLocWMI(IWbemLocator **pLoc, IWbemServices **pSvc);

int main()
{
	//	HRESULT hr;

	// Initialize COM
	if( FAILED(InitWMI()) ) return 1;

	// Obtain the initial locator to WMI
	IWbemLocator *pLoc = 0;
	IWbemServices *pSvc = 0;
	if( FAILED(ObtainInitLocWMI(&pLoc, &pSvc)) ) return 1;

	// Execute Query
	// http://msdn.microsoft.com/en-us/library/windows/desktop/aa392107(v=vs.85).aspx
	// Win32_LogonSession class
	// http://msdn.microsoft.com/en-us/library/windows/desktop/aa394189(v=vs.85).aspx
	IEnumWbemClassObject* pEnumerator = NULL;
	pSvc->ExecQuery(
		L"WQL", 
		//L"SELECT * FROM Win32_LogonSession",
		L"SELECT StartTime FROM Win32_LogonSession WHERE LogonType = 2",	// (Interactive (2))
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
		NULL,
		&pEnumerator);

	// Get the data from the query
	IWbemClassObject *pclsObj;
	ULONG uReturn = 0;
	while (pEnumerator)
	{
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

		if(0 == uReturn)
		{
			break;
		}

		VARIANT vtProp;
		CIMTYPE vtType;
		LONG	lFlavor;

		// Get the value of the Name property
		// http://msdn.microsoft.com/en-us/library/windows/desktop/aa391442(v=vs.85).aspx
		hr = pclsObj->Get(
			L"StartTime",	// 컬럼(프로퍼티)명
			0,			// Reserved
			&vtProp,	// 받아올 값[OUT]
			&vtType,	// 받은값타입[OUT] : http://msdn.microsoft.com/en-us/library/windows/desktop/aa386309(v=vs.85).aspx
			&lFlavor);	// 선호 형태[OUT] : WBEM_FLAVOR_ORIGIN_SYSTEM (32) ...

		wcout << " StartTime : " << vtProp.bstrVal << endl;
		VariantClear(&vtProp);

		pclsObj->Release();
	}

	// Cleanup
	pEnumerator->Release();
	pSvc->Release();
	pLoc->Release();
	CoUninitialize();

#ifndef _DEBUG
	system("pause");
#endif

	return 0;
}

// Initializing COM for a WMI Application
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa390885(v=vs.85).aspx
// Setting the Default Process Security Level Using C++
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa393617(v=vs.85).aspx
HRESULT InitWMI()
{
	// Initialize COM parameters
	HRESULT hr;
	hr = CoInitializeEx(0, COINIT_MULTITHREADED); 
	if (FAILED(hr)) {
		cout << "Failed to initialize COM library. Error code = 0x" << hex << hr << endl; 
		return hr;
	}

	// Initialize COM process security
	hr =  CoInitializeSecurity(
		NULL,						// Security descriptor    
		-1,							// COM negotiates authentication service
		NULL,						// Authentication services
		NULL,						// Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,	// Default authentication level for proxies
		RPC_C_IMP_LEVEL_IMPERSONATE,// Default Impersonation level for proxies
		NULL,						// Authentication info
		EOAC_NONE,					// Additional capabilities of the client or server
		NULL);						// Reserved

	if (FAILED(hr))
	{
		cout << "Failed to initialize security. Error code = 0x" << hex << hr << endl;
		CoUninitialize();
		return hr;                  // Program has failed.
	}

	return S_OK;
}

// Creating a Connection to a WMI Namespace
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa389749(v=vs.85).aspx
// Setting the Security Levels on a WMI Connection
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa393619(v=vs.85).aspx
HRESULT ObtainInitLocWMI(IWbemLocator **pLoc, IWbemServices **pSvc)
{
	HRESULT hr;

	hr = CoCreateInstance(
		CLSID_WbemLocator,		// COM 객체의 CLSID
		0,						// 외부 COM 객체의 IUnknown 포인터
		CLSCTX_INPROC_SERVER,	// 서버 컨텍스트
		IID_IWbemLocator,		// 요청 인터페이스의 IID
		(LPVOID *) pLoc);		// 리턴된 인터페이스 포인터

	if (FAILED(hr))
	{
		cout << "Failed to create IWbemLocator object. Err code = 0x"
			<< hex << hr << endl;
		CoUninitialize();
		return hr;     // Program has failed.
	}

	// Connect to the root\default namespace with the current user.
	hr = (*pLoc)->ConnectServer(
		BSTR(L"ROOT\\CIMV2"),				// Namespace
		NULL, NULL, 0, NULL, 0, 0, pSvc);

	if (FAILED(hr))
	{
		cout << "Could not connect. Error code = 0x" << hex << hr << endl;
		(*pLoc)->Release();
		CoUninitialize();
		return hr;      // Program has failed.
	}

	// Set the proxy so that impersonation of the client occurs.
	hr = CoSetProxyBlanket(*pSvc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
		);

	if (FAILED(hr))
	{
		cout << "Could not set proxy blanket. Error code = 0x" << hex << hr << endl;
		(*pSvc)->Release();
		(*pLoc)->Release();     
		CoUninitialize();
		return hr;      // Program has failed.
	}

	return S_OK;
}