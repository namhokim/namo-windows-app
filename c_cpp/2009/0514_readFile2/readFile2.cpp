// readFile2.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include "UTF8Conv.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::ifstream file("20090513190919266.xml");	// ������ �о�鿩

	if(file) {										// ����� ���ȴ�(��ü ����)
		std::string aLineUTF8;						// UTF-8 ������ ���� ���� ���ڿ�

		while(std::getline(file, aLineUTF8)) {
			UTF8_CONVERSION
			std::string aLine;						// ANSI string�� ���� ���� ���ڿ�
			aLine.assign(UTF82A(aLineUTF8.c_str()));// ��ȯ
			
			// UTF-8�� ANSI string���� ��ȯ�� ���� ���
			std::cout << aLine << std::endl;	// ���⼭�� �ܼ��� ȭ�鿡 ���
		}
	}
	return 0;
}

