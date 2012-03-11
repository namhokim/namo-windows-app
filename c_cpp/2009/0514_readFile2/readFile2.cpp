// readFile2.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include "UTF8Conv.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::ifstream file("20090513190919266.xml");	// 파일을 읽어들여

	if(file) {										// 제대로 열렸다(객체 생성)
		std::string aLineUTF8;						// UTF-8 한줄을 담을 버퍼 문자열

		while(std::getline(file, aLineUTF8)) {
			UTF8_CONVERSION
			std::string aLine;						// ANSI string을 담을 버퍼 문자열
			aLine.assign(UTF82A(aLineUTF8.c_str()));// 변환
			
			// UTF-8을 ANSI string으로 변환한 것을 사용
			std::cout << aLine << std::endl;	// 여기서는 단순히 화면에 출력
		}
	}
	return 0;
}

