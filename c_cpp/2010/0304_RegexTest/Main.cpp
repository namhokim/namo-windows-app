#include <string>
#include <regex>
#include <iostream>

using namespace std;

inline void Print(const string& str, const tr1::regex& reg);

int main()
{
	string f11("kthr_dept.csv");
	string f12("kthr_dept.cvs");
	string f13("kthr_user.csv");
	string f14("kthr_user.cvs");

	string f21("kthr_dept_20100222.csv");
	string f22("kthr_dept_20100222.cvs");
	string f23("kthr_user_20100222.csv");
	string f24("kthr_user_20100222.cvs");

	string f31("idms_dept.csv");
	string f32("idms_dept.cvs");
	string f33("idms_user.csv");
	string f34("idms_user.cvs");

	string f41("idms_dept_20100222.csv");
	string f42("idms_dept_20100222.cvs");
	string f43("idms_user_20100222.csv");
	string f44("idms_user_20100222.cvs");

	tr1::regex p1("(kthr|idms)_dept.(csv|cvs)");		// DEPT_ALL
	tr1::regex p2("(kthr|idms)_user.(csv|cvs)");		// USER_ALL
	tr1::regex p3("(kthr|idms)_dept_\\d{8}.(csv|cvs)");	// DEPT_ADD
	tr1::regex p4("(kthr|idms)_user_\\d{8}.(csv|cvs)");	// USER_ADD

	tr1::regex p = p4;
	Print(f11, p);
	Print(f12, p);
	Print(f13, p);
	Print(f14, p);

	Print(f21, p);
	Print(f22, p);
	Print(f23, p);
	Print(f24, p);

	Print(f31, p);
	Print(f32, p);
	Print(f33, p);
	Print(f34, p);

	Print(f41, p);
	Print(f42, p);
	Print(f43, p);
	Print(f44, p);
	

	return 0;
}

inline void Print(const string& str, const tr1::regex& reg)
{
	cout << str << ":\t" << tr1::regex_match(str, reg) << endl;
}