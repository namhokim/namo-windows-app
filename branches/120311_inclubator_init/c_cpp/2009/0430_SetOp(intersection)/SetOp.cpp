// SetOp.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <string>
#include <set>
#include <iostream>
#include <algorithm>
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	set<string> s1, s2, s;
	s1.insert("dev");
	s1.insert("network");
	s1.insert("os");
	s2.insert("dev");
	s2.insert("network");
	s2.insert("db");
	set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),inserter(s, s.end()));
	
	return 0;
}

