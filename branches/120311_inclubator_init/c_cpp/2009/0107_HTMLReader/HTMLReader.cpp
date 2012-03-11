// HTMLReader.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "HTMLReader.h"
#include "LiteHTMLReader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ������ ���� ���α׷� ��ü�Դϴ�.
CWinApp theApp;

using namespace std;

class CEventHandler : public ILiteHTMLReaderEvents
{
private:
	void BeginParse(DWORD dwAppData, bool &bAbort)
	{
		;
	}
    void StartTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
	{
		if(pTag->getTagName().Compare("BODY") ==0) {
			cout << (pTag->getAttributes())->->getCount() << endl;
		}
	}
    void EndTag(CLiteHTMLTag *pTag, DWORD dwAppData, bool &bAbort)
	{
		//cout << pTag->getTagName() << endl;
	}
    void Characters(const CString &rText, DWORD dwAppData, bool &bAbort)
	{
		;
	}
    void Comment(const CString &rComment, DWORD dwAppData, bool &bAbort)
	{
		;
	}
    void EndParse(DWORD dwAppData, bool bIsAborted)
	{
		;
	}
};

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC�� �ʱ�ȭ�մϴ�. �ʱ�ȭ���� ���� ��� ������ �μ��մϴ�.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���� �ڵ带 �ʿ信 ���� �����մϴ�.
		_tprintf(_T("�ɰ��� ����: MFC�� �ʱ�ȭ���� ���߽��ϴ�.\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: ���� ���α׷��� ������ ���⿡�� �ڵ��մϴ�.
		CLiteHTMLReader theReader;
		CEventHandler theEventHandler;
		theReader.setEventHandler(&theEventHandler);

		TCHAR   strToParse[] = _T("<HTML>"
			"<HEAD>"
			"<TITLE>"
			"<!-- title goes here -->"
			"</TITLE>"
			"</HEAD>"
			"<BODY LEFTMARGIN=\"15px\">This is a sample HTML document.</BODY>"
			"</HTML>");
		theReader.Read(strToParse);


	}

	return nRetCode;
}
