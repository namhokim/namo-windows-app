#include "phyAddr.h"
#include <iostream>
#include <Windows.h>
int main()
{
	while(true) {
		Network::PhyAddrVec p = Network::GetMyPhyAddrs();
		std::cout << "number of size : " << p.size() << std::endl;
		Network::PhyAddrVec::iterator pos;
		for(pos = p.begin(); pos != p.end(); ++pos) {
			std::wcout << (*pos) << std::endl;
		}
		Sleep(3000);
	}
}
