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

// XML ����
void XmlWriting(const wchar_t* pszFile)
{
	// �ۼ��� ����
    CComPtr<IXmlWriter> pWriter;	// CComPtr Ŭ���� ���ø��� �������̽� �����Ͱ� �ﰢ �����ǵ��� �Ѵ�
	HRESULT hr = ::CreateXmlWriter(IID_IXmlWriter,
									reinterpret_cast<void**>(&pWriter),
									NULL);
	if(S_OK!=hr) return;

	// ������� ����� ����� ����
	CComPtr<IStream> stream;
	// Create stream object
	hr = ::SHCreateStreamOnFileW(pszFile, STGM_WRITE | STGM_CREATE, &stream);
	hr = pWriter->SetOutput(stream);
	if(S_OK!=hr) return;

	// �ۼ��� �Ӽ� ����
	hr = pWriter->SetProperty(XmlWriterProperty_Indent, TRUE);	// ����� �б� ���ϵ��� XML ����� �鿩������

	// ���� �۾�
	hr = pWriter->WriteStartDocument(XmlStandalone_Omit);	// ��Ʈ���� XML ������ �ۼ��ϴ� �۾��� ó��
	hr = pWriter->WriteStartElement(0,									// ��ҿ� ���� ���ӽ����̽� ���λ�(�ɼ�)
									L"html",							// ����� ���� �̸��� ����
									L"http://www.w3.org/1999/xhtml");	// ���ӽ����̽� URI(�ɼ�)

	hr = pWriter->WriteStartElement(0, L"head", 0);						// <head>
	hr = pWriter->WriteElementString(0, L"title", 0, L"My Web Page");	// <title>
	hr = pWriter->WriteEndElement();									// </title></head>

	hr = pWriter->WriteStartElement(0, L"body", 0);					// <body>
	hr = pWriter->WriteElementString(0, L"p", 0, L"�ȳ��ϼ���!");	// <p>
	//  body�� html ��Ұ� ��������� ���� �ʰ� WriteEndDocument()�� ���� �ڵ����� ����(�ݴ� ���� ����)
	//hr = pWriter->WriteEndElement();								// </body>

	hr = pWriter->WriteEndDocument();								// </body></html>

	// ��� ���Ⱑ Ȯ���� ����
	hr = pWriter->Flush();

	return;
}