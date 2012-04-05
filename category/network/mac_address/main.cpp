#include "phyAddr.h"
#include <string>	// also included in pyhAddr.h
#include <iostream>

using namespace std;

void basic();
void seperator();
void lowercase();

int main()
{
	basic();
	seperator();
	lowercase();

	system("pause");
	return 0;
}

void basic()
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
}

void seperator()
{
	string mac;
	if(Network::GetBasicMacAddress(mac, '\0')) {
		cout << "MAC (ANSI)\t: " << mac << endl;

		wstring mac_w;
		if(Network::GetBasicMacAddress(mac_w, L':')) {
			wcout << "MAC (UNICODE)\t: " << mac_w << endl;
		} else {
			wcout << "Failed" << endl;
		}
	} else {
		cout << "Failed" << endl;
	}
}

void lowercase()
{
	string mac;
	if(Network::GetBasicMacAddress(mac, '-', Network::ByLower)) {
		cout << "MAC (ANSI)\t: " << mac << endl;

		wstring mac_w;
		if(Network::GetBasicMacAddress(mac_w, L'-', Network::ByLower)) {
			wcout << "MAC (UNICODE)\t: " << mac_w << endl;
		} else {
			wcout << "Failed" << endl;
		}
	} else {
		cout << "Failed" << endl;
	}
}