#include "XmlReading.h"
#include <stdlib.h>		// for system()

// 메인함수
int main()
{
	XmlReading(L"Some_euc-kr.xml");
	//XmlReading(L"Some_utf8.xml");
	//XmlReading(L"Some_utf16.xml");

	system("pause");	// 콘솔 화면을 멈추기 위함
	return 0;
}

