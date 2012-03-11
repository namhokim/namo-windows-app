/*
 * Copyright (c) 2010 by Namho Kim.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 */
#include "XmlReading.h"

#include <atlbase.h>
#include <XmlLite.h>
#include <iostream>

using namespace std;

#pragma comment(lib, "XmlLite.lib")

// XML �б�
void XmlReading(const wchar_t* pszFile)
{
	// �ǵ��� ����
	CComPtr<IXmlReader> reader;	// CComPtr Ŭ���� ���ø��� �������̽� �����Ͱ� �ﰢ �����ǵ��� �Ѵ�
	HRESULT hr = ::CreateXmlReader(IID_IXmlReader,
								reinterpret_cast<void**>(&reader),
								NULL);
	if(S_OK!=hr) return;

	// �Է����� ����� ����� ����
	CComPtr<IStream> stream;
	hr = ::SHCreateStreamOnFile(pszFile, STGM_READ, &stream);	// Create stream object
	hr = reader->SetInput(stream);
	if(S_OK!=hr) return;

	// �б� �۾�
	HRESULT result = S_OK;
	XmlNodeType nodeType = XmlNodeType_None;

	const WCHAR* pwszLocalName;
	UINT inline_level = 0;
	result = reader->Read(&nodeType);
	while (S_OK == result)
	{
		printf("%d\n", nodeType);
		switch(nodeType) {
			case XmlNodeType_Element:
				if(S_OK == reader->GetLocalName(&pwszLocalName, NULL)) {
					for(UINT i=0; i<inline_level; ++i) wcout << L" ";
					wcout << pwszLocalName << endl;
				}
				inline_level++;
				break;
			case XmlNodeType_Attribute:
				break;
			case XmlNodeType_Text:
				if(S_OK == reader->GetValue(&pwszLocalName, NULL)) {
					for(UINT i=0; i<inline_level; ++i) wcout << L" ";
					MessageBox(NULL, pwszLocalName, L"�˸�", MB_OK);
				}
				break;
			case XmlNodeType_CDATA:
				break;
			case XmlNodeType_ProcessingInstruction:
				break;
			case XmlNodeType_Comment:
				break;
			case XmlNodeType_DocumentType:
				break;
			case XmlNodeType_Whitespace:
				break;
			case XmlNodeType_EndElement:
				inline_level--;
				if(S_OK == reader->GetLocalName(&pwszLocalName, NULL)) {
					for(UINT i=0; i<inline_level; ++i) wcout << L" ";
					wcout << pwszLocalName << endl;
				}
				break;
			case XmlNodeType_XmlDeclaration:
				break;
		}
		result = reader->Read(&nodeType);
	}
	return;
}