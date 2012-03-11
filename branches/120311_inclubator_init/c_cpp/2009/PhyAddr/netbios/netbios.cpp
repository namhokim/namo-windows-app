#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <strstream>
#include <string>

using namespace std;

#pragma comment(lib, "netapi32.lib")	// for _Netbios

#define bzero(thing,sz) memset(thing,0,sz)

bool GetAdapterInfo(int adapter_num, string &mac_addr)
{
	// Reset the LAN adapter so that we can begin querying it
	NCB Ncb;
	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBRESET;
	Ncb.ncb_lana_num = adapter_num;
	if (Netbios(&Ncb) != NRC_GOODRET) {
		mac_addr = "bad (NCBRESET): ";
		mac_addr += Ncb.ncb_retcode;
		return false;
	}

	// Prepare to get the adapter status block
	bzero(&Ncb,sizeof(Ncb));
	Ncb.ncb_command = NCBASTAT;
	Ncb.ncb_lana_num = adapter_num;
	strcpy((char *) Ncb.ncb_callname, "*");
	struct ASTAT
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER NameBuff[30];
	} Adapter;
	bzero(&Adapter,sizeof(Adapter));
	Ncb.ncb_buffer = (unsigned char *)&Adapter;
	Ncb.ncb_length = sizeof(Adapter);

	// Get the adapter's info and, if this works, return it in standard,
	// colon-delimited form.
	if (Netbios(&Ncb) == 0)
	{
		char acMAC[18];
		sprintf(acMAC, "%02X:%02X:%02X:%02X:%02X:%02X",
			int (Adapter.adapt.adapter_address[0]),
			int (Adapter.adapt.adapter_address[1]),
			int (Adapter.adapt.adapter_address[2]),
			int (Adapter.adapt.adapter_address[3]),
			int (Adapter.adapt.adapter_address[4]),
			int (Adapter.adapt.adapter_address[5]));
		mac_addr = acMAC;
		return true;
	}
	else
	{
		mac_addr = "bad (NCBASTAT): ";
		mac_addr += Ncb.ncb_retcode;
		return false;
	}
}

int main()
{
	// Get adapter list
	LANA_ENUM AdapterList;
	NCB Ncb;
	memset(&Ncb, 0, sizeof(NCB));
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (unsigned char *)&AdapterList;
	Ncb.ncb_length = sizeof(AdapterList);
	Netbios(&Ncb);

	// Get all of the local ethernet addresses
	string mac_addr;
	for (int i = 0; i < AdapterList.length; ++i)
	{
		if (GetAdapterInfo(AdapterList.lana[i], mac_addr))
		{
			cout << "Adapter " << int (AdapterList.lana[i]) <<
				"'s MAC is " << mac_addr << endl;
		}
		else
		{
			cerr << "Failed to get MAC address! Do you" << endl;
			cerr << "have the NetBIOS protocol installed?" << endl;
			break;
		}
	}
	return 0;
}


//---------------------------------------------------------------------------

