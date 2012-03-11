// BeginCom.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <ObjBase.h>
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr;
	// COM 라이브러리 초기화
	hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if(FAILED(hr)) {
		cout << "초기화 실패\n";
		return 0;
	}
	
	// COM 객체의 CLSID(class identifier) 구하기
	CLSID clsid;	//CLSID clsid = {0xdee35070L,0x506b,0x11cf,{0xb1,0xaa,0x0,0xaa,0x0,0xb8,0xde,0x95}};
	hr = ::CLSIDFromProgID(L"Microsoft.Jet.OLEDB.4.0", &clsid);
	if(FAILED(hr)) {
		cout << "레지스트리에 등록되지 않았음\n";
		return 0;
	}
	
	// COM 객체 인스턴스 생성 :: CoCreateInstance() -> CoGetClassObject()
	IUnknown* pUnk = NULL;
	hr = ::CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IUnknown, (void**)&pUnk);
	// 레지스트리에서 COM 객체의 경로명을 구한다 :: CoGetClassObject() -> CoLoadLibrary()
	//													 
	IClassFactory* pClassFactory = NULL;
	::CoGetClassObject(clsid, CLSCTX_ALL, NULL, IID_IClassFactory, (void**)&pClassFactory);
	pClassFactory->CreateInstance(NULL, IID_IUnknown, (void**)&pUnk);
	pClassFactory->Release();

	if(FAILED(hr)) {
		cout << "인스턴스를 생성할 수 없음\n";
		return 0;
	}
	return 0;
}

