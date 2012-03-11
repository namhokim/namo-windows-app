#include <iostream>
#include <fstream>

int main()
{
	std::wofstream logfile("log.txt");
	logfile.imbue(std::locale("kor"));
	const std::wstreambuf * const orig = std::wclog.rdbuf();
	std::wclog << L"print�ѱ�1" << std::endl;
	std::wclog.rdbuf(logfile.rdbuf());
	std::wclog << L"print�ѱ�2" << std::endl;
	return 0;
}