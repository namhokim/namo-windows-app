#include "phyAddr.h"
#include <string>	// also included in pyhAddr.h
#include <iostream>

using namespace std;

int main()
{
	string mac;
	if(Network::GetBasicMacAddress(mac)) {
		cout << "MAC (ANSI)\t: " << mac << endl;

		wstring mac_w;
		if(Network::GetBasicMacAddress(mac_w)) {
			wcout << "MAC (UNICODE)\t: " << mac_w << endl;
		} else {
			wcout << "Failed" << endl;
		}
	} else {
		cout << "Failed" << endl;
	}

	system("pause");
	return 0;
}