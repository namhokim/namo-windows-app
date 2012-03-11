// BeginCom.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <ObjBase.h>
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr;
	// COM ���̺귯�� �ʱ�ȭ
	hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if(FAILED(hr)) {
		cout << "�ʱ�ȭ ����\n";
		return 0;
	}
	
	// COM ��ü�� CLSID(class identifier) ���ϱ�
	CLSID clsid;	//CLSID clsid = {0xdee35070L,0x506b,0x11cf,{0xb1,0xaa,0x0,0xaa,0x0,0xb8,0xde,0x95}};
	hr = ::CLSIDFromProgID(L"Microsoft.Jet.OLEDB.4.0", &clsid);
	if(FAILED(hr)) {
		cout << "������Ʈ���� ��ϵ��� �ʾ���\n";
		return 0;
	}
	
	// COM ��ü �ν��Ͻ� ���� :: CoCreateInstance() -> CoGetClassObject()
	IUnknown* pUnk = NULL;
	hr = ::CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IUnknown, (void**)&pUnk);
	// ������Ʈ������ COM ��ü�� ��θ��� ���Ѵ� :: CoGetClassObject() -> CoLoadLibrary()
	//													 
	IClassFactory* pClassFactory = NULL;
	::CoGetClassObject(clsid, CLSCTX_ALL, NULL, IID_IClassFactory, (void**)&pClassFactory);
	pClassFactory->CreateInstance(NULL, IID_IUnknown, (void**)&pUnk);
	pClassFactory->Release();

	if(FAILED(hr)) {
		cout << "�ν��Ͻ��� ������ �� ����\n";
		return 0;
	}
	return 0;
}

