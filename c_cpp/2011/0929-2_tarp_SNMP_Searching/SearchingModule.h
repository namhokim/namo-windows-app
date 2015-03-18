#pragma once

#include <string>
#include <vector>

typedef struct _LIST_OF_DEVICES {
	std::string ip;
	std::string mac;
	std::string subnet;
} LIST_OF_DEVICES, *LPLIST_OF_DEVICES;

typedef std::vector<LIST_OF_DEVICES> LISTS_OF_DEVICE;

// 리턴값: 변화가 된 개수(0: 변화없음, N; N개가 바뀜
void GetChangeDevices(
	const LISTS_OF_DEVICE& devs		// IN
);

namespace Properties
{
	void SetSNMPTimeout(int ms_time);
	int GetSNMPTimeout();
	void SetConcurrentConnect(int limit);
	int GetConcurrentConnect();

	void SetNetscanStartIP(const char *startIP);
	const char * GetNetscanStartIP();
	void SetNetscanEndIP(const char *endIP);
	const char * GetNetscanEndIP();
	bool IsSetCustomRange();
}
