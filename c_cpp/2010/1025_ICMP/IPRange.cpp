#include "IPRange.h"

#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <Ipexport.h>	// for in_addr Structure
#include <Icmpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

#include <algorithm>	// for setIP(3) 's swap
#include <sstream>

using namespace std;

IPRange::IPRange(void)
{
	m_ul_IPEnd	 = 1;
	m_ul_IPBegin = 2;	// cross 해서 갋을 넣음
	m_currPos	 = 3;	// illear pos
}
IPRange::IPRange(string ip1, string ip2, bool isSubnetMask)
{
	setIP(ip1, ip2, isSubnetMask);
}

IPRange::~IPRange(void)
{
}

void IPRange::setIP(std::string ip1, std::string ip2, bool isSubnetMask)
{
	unsigned long ul_ip1 = str2uint(ip1.c_str());
	unsigned long ul_ip2 = str2uint(ip2.c_str());

	if(ul_ip1 > ul_ip2)	std::swap<unsigned long>(ul_ip1, ul_ip2);	// swap : algorithm

	if(isSubnetMask == false) {	// 입력 : ip 2개
		m_ul_IPBegin	= ul_ip1;
		m_ul_IPEnd		= ul_ip2;
	}
	else {	// 입력: 한 네트워크의 ip와 서브넷
		m_ul_IPBegin	= getBeginIP(ul_ip1, ul_ip2);
		m_ul_IPEnd		= getEndIP(ul_ip1, ul_ip2);
	}
	initPos();	//	m_currPos = 0;
}

// Position 관련
bool IPRange::setPos(unsigned long pos)
{
	if(beginPos() <= pos  && pos <= endPos()) {	// check range
		m_currPos = pos;
		return false;
	}
	else return true;
}

std::string	IPRange::next()
{
	unsigned long retIP = m_ul_IPBegin + m_currPos;
	if(retIP > m_ul_IPEnd) return "";
	else {
		++m_currPos;
		return uint2str(retIP);
	}
}

// 변환관련
unsigned int IPRange::str2uint(std::string ip)
{
	return htonl(inet_addr(ip.c_str()));	// Little Endian
}

std::string IPRange::uint2str(unsigned long ip)
{
	unsigned short s1, s2, s3, s4;
	s1 = (ip & 0xff000000L) >> 24;
	s2 = (ip & 0x00ff0000L) >> 16;
	s3 = (ip & 0x0000ff00L) >> 8;
	s4 = (ip & 0x000000ffL);

	std::stringstream ss;
	ss << s1 << "." << s2 << "." << s3 << "." << s4;
	return ss.str();

	// inet_ntoa을 하는 과정에서 메모리 누수와 핸들 증가가 관찰되어 직접 만들었음
	//struct in_addr tmp_in_addr;
	//tmp_in_addr.s_addr = ntohl(ip);	// Little Endian -> Big Endian(TCP/IP network byte order)

	//return inet_ntoa(tmp_in_addr);
}

bool IsICMP_OK(const std::string& ip)
{
	unsigned long ipaddr = INADDR_NONE;
	ipaddr = inet_addr(ip.c_str());
	if (ipaddr == INADDR_NONE) {
		return false;
	}

	HANDLE hIcmpFile = ::IcmpCreateFile();
	if (hIcmpFile == INVALID_HANDLE_VALUE) {
		return false;
	}

	char SendData[32] = {'a','b','c','d','e','f','g','h','i','k','j','l','m','n','o','p','q','r','s','t','u','v','w','a','b','c','d','e','f','g','h','i'};
	const DWORD ReplySize = sizeof(ICMP_ECHO_REPLY)+sizeof(SendData);
	char ReplayBuf[ReplySize] = {0};

	DWORD dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData), 
									NULL, &ReplayBuf, ReplySize, 500);

	::IcmpCloseHandle(hIcmpFile);


	return (dwRetVal!=0);
}