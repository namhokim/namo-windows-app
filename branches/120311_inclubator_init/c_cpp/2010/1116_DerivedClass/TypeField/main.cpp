#include "Clazz.h"

int main()
{
	Employee e;
	e.family_name = "Kim";
	e.first_name = "Namo";
	e.department = 101;
	print_employee(&e);

	Manager m;
	m.family_name = "Lee";
	m.first_name = "Sung-ooooong";
	m.department = 102;
	m.level = 2;
	m.group.push_back(&e);
	print_employee(&m);
	print_list(m.group);
	
	return 0;
}