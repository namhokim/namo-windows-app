#include "MS_DOS_Header.h"
#include <iostream>
using namespace std;

int main()
{
	MS_DOS_Header ms;
	//ms.OpenA("emailRegex.exe");
	ms.Open(TEXT("Debug\\PortableExecutable.exe"));
	cout << ms.IsDosHeader() << endl;
	ms.Close();
	return 0;
}
