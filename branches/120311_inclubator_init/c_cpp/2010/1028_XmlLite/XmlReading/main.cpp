#include "XmlReading.h"
#include <stdlib.h>		// for system()

// �����Լ�
int main()
{
	XmlReading(L"Some_euc-kr.xml");
	//XmlReading(L"Some_utf8.xml");
	//XmlReading(L"Some_utf16.xml");

	system("pause");	// �ܼ� ȭ���� ���߱� ����
	return 0;
}

