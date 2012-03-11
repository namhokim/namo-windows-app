#pragma once

#include <string>
#include <list>

using namespace std;

class Date {
public:
	enum Month {jan=1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec};
};

struct Employee {
	enum Empl_type { M, E};
	Empl_type type;

	Employee():type(E) {}
	string first_name, family_name;
	char middle_initial;

	Date hiring_date;
	short department;
	//
};

struct Manager : public Employee {
	Manager() { type=M;	};
	list<Employee*> group;
	short level;
	//
};

void print_employee(const Employee*);
void print_list(const list<Employee*>&);