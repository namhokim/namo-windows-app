// Class.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
#include <sstream>
#include <iostream>
#include <oledb.h>

using namespace std;

bool leapyear(int y)
{
	if( ((y%4) == 0) && ((y%100) == 0) ) return true;
	else return false;
}
class Date {
	int d, m, y;
	static Date default_date;
	mutable bool cache_valid;
	mutable string cache;
	void compute_cache_value() const;
public:
	Date(int d=0, int m=0, int y=0);
	static void set_default(int dd, int mm, int yy);
	int day()	const {	return d;	}
	int month()	const {	return m;	}
	int year()	const {	return y;	}
	Date& Date::add_year(int n);
	string string_rep() const;
};
Date Date::default_date(16,12,1770);

Date::Date(int dd, int mm, int yy)
: cache_valid(false)
{
	d = dd ? dd : default_date.d;
	m = mm ? mm : default_date.m;
	y = yy ? yy : default_date.y;
}	

void Date::set_default(int d, int m, int y)
{
	default_date = Date(d, m, y);
}

Date& Date::add_year(int n)
{
	if(d==29 && m==2 && !leapyear(y+n)) {
		d = 1;
		m = 3;
	}
	y += n;
	return *this;
}

string Date::string_rep() const
{
	if(cache_valid == false) {
		/*Date* th = const_cast<Date*>(this);
		th->compute_cache_value();
		th->cache_valid = true;*/
		compute_cache_value();
		cache_valid = true;
	}
	return cache;
}

void Date::compute_cache_value() const
{
	stringstream ss;
	ss << y << "/" << m << "/" << d;
	cache.assign(ss.str());
}

int _tmain(int argc, _TCHAR* argv[])
{
	Date bD;
	cout << bD.year() << endl;
	bD.add_year(2).add_year(3);
	cout << bD.year() << endl;
	cout << bD.string_rep() << endl;
	
	const Date d2;
	cout << d2.string_rep() << endl;
	return 0;
}

