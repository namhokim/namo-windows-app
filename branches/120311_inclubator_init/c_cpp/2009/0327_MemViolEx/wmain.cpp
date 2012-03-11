#include <windows.h>

#define BAD_ADDRESS 0xBAADF00D

int __cdecl wmain (int argc, wchar_t* pArgs[])
{
	char* p = (char *)BAD_ADDRESS;

	*p='A';

	return 0;
}