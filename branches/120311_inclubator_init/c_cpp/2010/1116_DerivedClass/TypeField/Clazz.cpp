#include "Clazz.h"
#include <iostream>

void print_employee(const Employee* e)
{
	switch(e->type) {
		case Employee::E:
			cout << e->family_name << '\t' << e->department << '\n';
			//
			break;
		case Employee::M:
			{
				cout << e->family_name << '\t' << e->department << '\n';
				//...
				const Manager* p = static_cast<const Manager*>(e);
				cout << "level" << p->level << '\n';
				//...
			}
			break;
	}
}

void print_list(const list<Employee*>& elist)
{
	list<Employee*>::const_iterator p;
	for(p=elist.begin(); p!=elist.end(); ++p) {
		print_employee(*p);
	}
}

//void print_employee(const Employee* e)
//{
//	cout << e->family_name << '\t' << e->department << '\n';
//	//...
//
//	if(e->type == Employee::M) {
//		const Manager* p = static_cast<const Manager*>(e);
//		cout << "level" << p->level << '\n';
//	}
//}