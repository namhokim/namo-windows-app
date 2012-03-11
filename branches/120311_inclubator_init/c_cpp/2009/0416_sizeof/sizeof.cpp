// sizeof.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <limits>
#include <iostream>
#include <string>

template <typename T>
void sizeInfo(std::string str)
{
	std::cout << "sizeof(" << str << ")\t: " << sizeof(T) << "Byte"
		<< "\t" << int(std::numeric_limits<T>::min()) << '(' << std::numeric_limits<T>::min() << ')'
		<< " ~ " << int(std::numeric_limits<T>::max()) << '(' << std::numeric_limits<T>::max() << ')' << '\n';
}

int _tmain(int argc, _TCHAR* argv[])
{
	sizeInfo<bool>("bool");
	sizeInfo<char>("char");
	sizeInfo<wchar_t>("wchar_t");
	sizeInfo<short>("short");
	sizeInfo<int>("int");
	sizeInfo<long>("long");
	sizeInfo<__int64>("__int64");
	sizeInfo<float>("float");
	sizeInfo<double>("double");
	sizeInfo<long double>("long double");
	sizeInfo<int*>("int*");
	sizeInfo<long*>("long*");
	sizeInfo<__int64*>("__int64*");
	//sizeInfo<void>("void");
	enum flag { x=1, y=2, z=4, e=8	};	// 0 ~ 15의 범위
	sizeInfo<flag>("enum flag");

	return 0;
}

