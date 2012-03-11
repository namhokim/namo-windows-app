/*
 * Copyright (c) 2010 by Namho Kim.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */

#include "XmlWriting.h"

#include <atlbase.h>
#include <XmlLite.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "XmlLite.lib")

// XML 쓰기
void XmlWriting(const wchar_t* pszFile)
{
	// 작성기 생성
    CComPtr<IXmlWriter> pWriter;	// CComPtr 클래스 템플릿은 인터페이스 포인터가 즉각 해제되도록 한다
	HRESULT hr = ::CreateXmlWriter(IID_IXmlWriter,
									reinterpret_cast<void**>(&pWriter),
									NULL);
	if(S_OK!=hr) return;

	// 출력으로 사용할 저장소 지정
	CComPtr<IStream> stream;
	// Create stream object
	hr = ::SHCreateStreamOnFileW(pszFile, STGM_WRITE | STGM_CREATE, &stream);
	hr = pWriter->SetOutput(stream);
	if(S_OK!=hr) return;

	// 작성기 속성 수정
	hr = pWriter->SetProperty(XmlWriterProperty_Indent, TRUE);	// 사람이 읽기 편하도록 XML 출력을 들여쓰기함

	// 쓰기 작업
	hr = pWriter->WriteStartDocument(XmlStandalone_Omit);	// 스트림에 XML 선언을 작성하는 작업을 처리
	hr = pWriter->WriteStartElement(0,									// 요소에 대한 네임스페이스 접두사(옵션)
									L"html",							// 요소의 로컬 이름을 지정
									L"http://www.w3.org/1999/xhtml");	// 네임스페이스 URI(옵션)

	hr = pWriter->WriteStartElement(0, L"head", 0);						// <head>
	hr = pWriter->WriteElementString(0, L"title", 0, L"My Web Page");	// <title>
	hr = pWriter->WriteEndElement();									// </title></head>

	hr = pWriter->WriteStartElement(0, L"body", 0);					// <body>
	hr = pWriter->WriteElementString(0, L"p", 0, L"안녕하세요!");	// <p>
	//  body와 html 요소가 명시적으로 닫지 않고 WriteEndDocument()에 의해 자동으로 닫음(닫는 것이 좋음)
	//hr = pWriter->WriteEndElement();								// </body>

	hr = pWriter->WriteEndDocument();								// </body></html>

	// 모든 쓰기가 확실히 수행
	hr = pWriter->Flush();

	return;
}