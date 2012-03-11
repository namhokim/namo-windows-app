#pragma warning(disable:4996)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tchar.h>

#define XML_STATIC      /* for static linking   */

#if defined (UNICODE)
#  define XML_UNICODE_WCHAR_T
#endif

#include "expat.h"

//#if defined (UNICODE)
//#  pragma comment(lib, "libexpatMTw.lib")
//#else
//#  pragma comment(lib, "libexpatMT.lib")
//#endif

typedef struct NameBook
{
	int m_nSince;
	char m_szName[0xFF];
} NameBook;

static void XMLCALL StartElement(void *pUserData, const char *name, const char **atts)
{
	int i = 0;
	const char *pszAttrName = NULL;
	const char *pszAttrValue = NULL;
	NameBook *pNameBook = (NameBook *)pUserData;

	for (i = 0; atts[i]; i += 2)
	{
		pszAttrName = atts[i];
		pszAttrValue = atts[i+1];

		if ( _tcscmp(pszAttrName, _T("since")) == 0 && pszAttrValue )
			pNameBook->m_nSince = _tstoi(pszAttrValue);
	}
}

static void XMLCALL VisitData(void *pUserData, const char *s, int len)
{
	NameBook *pNameBook = (NameBook *)pUserData;

	if ( pNameBook && s && len > 0 )
	{
		_tcsncpy(pNameBook->m_szName, s, len);
		pNameBook->m_szName[len] = 0;
	}
}

static void XMLCALL EndElement(void *pUserData, const char *name)
{
	NameBook *pNameBook = (NameBook *)pUserData;
}

int main(int argc, char **argv)
{
	int bEof = 1;
	char xml_str[38336];
	FILE *fp;
	fp = fopen("XMLFile1.xml", "r+");
	fgets(xml_str,sizeof(xml_str), fp);

	const char *pszXMLDoc = _T(xml_str);
	printf("%s\n",xml_str);

	XML_Parser parser = NULL;
	NameBook nameBook;

	memset(&nameBook, 0x00, sizeof(NameBook));
	parser = XML_ParserCreate(NULL);

	XML_SetUserData(parser, (void *)&nameBook);
	XML_SetElementHandler(parser, StartElement, EndElement);
	XML_SetCharacterDataHandler(parser, VisitData);

	if (XML_Parse(parser, pszXMLDoc, _tcslen(pszXMLDoc), bEof) == XML_STATUS_ERROR)
	{
		_tprintf(_T("%s at line %u\n"), XML_ErrorString(XML_GetErrorCode(parser)),
			XML_GetCurrentLineNumber(parser));
		return 1;
	}

	_tprintf(_T("NameBook Since %d, Name : '%s'\n"), nameBook.m_nSince, nameBook.m_szName);
	XML_ParserFree(parser);


	fclose(fp);


}

