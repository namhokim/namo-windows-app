#include "SearchingModule.h"
#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>

#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

typedef set< pair<string, string> > str_set;
typedef map<string, LIST_OF_DEVICES> net_map;

// �����ʱ�ȭ �� �ڵ� ���� ��ü
class winsock_auto_cleanup_imple;
class winsock_auto_cleanup
{
public:
    winsock_auto_cleanup();
    ~winsock_auto_cleanup();
private:
    std::auto_ptr<winsock_auto_cleanup_imple>    m_pImpl;
};

// �Լ� ���漱��
namespace network {
	VOID AddToNetworkSet(str_set& d_set, const std::string& ip, const std::string& subnet);
	VOID MakeNetworkMap(const str_set& s);
	
} /* namespace network */

// �Լ� ����
void GetChangeDevices(const LISTS_OF_DEVICE& devs_f)
{
	using namespace network;
	// WinSock
	winsock_auto_cleanup ws_auto;

	LISTS_OF_DEVICE::size_type pos;
	LISTS_OF_DEVICE::size_type size = devs_f.size();

	// ã�� ����(��Ʈ��ũ)�� ���ؼ� ��Ʈ��ũ SET�� �ִ´�
	str_set d_set;	// ��Ʈ��ũ ��
	for(pos=0; pos<size; ++pos) {
		LIST_OF_DEVICES aInfo = devs_f.at(pos);
		AddToNetworkSet(d_set, aInfo.ip, aInfo.subnet);
	}

	// ��Ʈ��ũ ���� �ۼ�
	MakeNetworkMap(d_set);
}

// �˰��� ��å
//#define	USE_ARP_IN_LOCALNETWORK_GET_MAC_SUBNET

////////////////////////////////////////////////////////////////////////////////////////////////////
class IPRange
{
public:
	IPRange(void);
	IPRange(std::string ip1, std::string ip2, bool isSubnetMask=false);
	void	setIP(std::string ip1, std::string ip2, bool isSubnetMask=false);
	~IPRange(void);
	// Position�� Ư�� ������ ����(������ �ʰ��� ��쿡�� ������ ���� �ʰ�, false����)
	bool			setPos(unsigned long pos);
	// Position�� ����(0���� ����)
	void			initPos()	{	setPos(0);			}
	// ���� Position�� ��ȯ
	unsigned long	currPos()	{	return m_currPos;	}
	// ���� Position�� ��ȯ(�׻� 0)
	unsigned long	beginPos()	{	return 0;}
	// ������ Position�� ��ȯ
	unsigned long	endPos()	{	return (m_ul_IPEnd - m_ul_IPBegin);	}
	// ���� ������ ����IP�� ��ȯ
	std::string		begin()		{	return uint2str(m_ul_IPBegin);	}
	// ���� Position�� IP�� ��ȯ�ϸ�, ���� Position�� �ϳ� ���� ��Ų��, �� ���� �ʰ� �Ҷ��� ���ڿ� ��ȯ
	std::string		next();
	// ���� ������ ������IP�� ��ȯ
	std::string		end()		{	return uint2str(m_ul_IPEnd);	}
	
	// ���� ������ ����IP�� unsigned long ���¸� ��ȯ
	unsigned long		beginUL()		{	return m_ul_IPBegin;	}
	// ���� ������ ������IP�� unsigned long ���¸� ��ȯ
	unsigned long		endUL()		{	return m_ul_IPEnd;	}
private:
	unsigned long m_ul_IPBegin;	// plus offset
	unsigned long m_ul_IPEnd;
	unsigned long m_currPos;
	unsigned long m_ulSubnetmask;
	// ��ȯ����
	unsigned int	str2uint(std::string ip);
	std::string		uint2str(unsigned long ip);
	unsigned long	getBeginIP(unsigned long ul_ip, unsigned long ul_subnet){	return (ul_ip & ul_subnet)+1;	  }	// ��� �ּ� ���ϱ�
	unsigned long	getEndIP  (unsigned long ul_ip, unsigned long ul_subnet){	return (ul_ip | (~ul_subnet));}	// ������ �ּ� ���ϱ�
};


/******************************************************************************
Module From:  EnsureCleanup.h
Notices: Copyright (c) 2007 Jeffrey Richter & Christophe Nasarre
Purpose: These classes ensure object cleanup when an object goes out of scope.
         See Appendix A.
******************************************************************************/


// Data type representing the address of the object's cleanup function.
// I used UINT_PTR so that this class works properly in 64-bit Windows.
typedef VOID (WINAPI* PFNENSURECLEANUP)(UINT_PTR);

// Each template instantiation requires a data type, address of cleanup 
// function, and a value that indicates an invalid value.
template<class TYPE, PFNENSURECLEANUP pfn, UINT_PTR tInvalid = NULL> 
class CEnsureCleanup {
public:
   // Default constructor assumes an invalid value (nothing to cleanup)
   CEnsureCleanup() { m_t = tInvalid; }

   // This constructor sets the value to the specified value
   CEnsureCleanup(TYPE t) : m_t((UINT_PTR) t) { }

   // The destructor performs the cleanup.
   ~CEnsureCleanup() { Cleanup(); }

   // Helper methods to tell if the value represents a valid object or not..
   BOOL IsValid() { return(m_t != tInvalid); }
   BOOL IsInvalid() { return(!IsValid()); }

   // Re-assigning the object forces the current object to be cleaned-up.
   TYPE operator=(TYPE t) { 
      Cleanup(); 
      m_t = (UINT_PTR) t;
      return(*this);  
   }

   // Returns the value (supports both 32-bit and 64-bit Windows).
   operator TYPE() { 
      return (TYPE) m_t;
   }

   // Cleanup the object if the value represents a valid object
   void Cleanup() { 
      if (IsValid()) {
         // In 64-bit Windows, all parameters are 64-bits, 
         // so no casting is required
         pfn(m_t);         // Close the object.
         m_t = tInvalid;   // We no longer represent a valid object.
      }
   }
   
private:
   UINT_PTR m_t;           // The member representing the object
};

///////////////////////////////////////////////////////////////////////////////


// Macros to make it easier to declare instances of the template 
// class for specific data types.

#define MakeCleanupClass(className, tData, pfnCleanup) \
   typedef CEnsureCleanup<tData, (PFNENSURECLEANUP) pfnCleanup> className;

#define MakeCleanupClassX(className, tData, pfnCleanup, tInvalid) \
   typedef CEnsureCleanup<tData, (PFNENSURECLEANUP) pfnCleanup, \
   (INT_PTR) tInvalid> className;

///////////////////////////////////////////////////////////////////////////////
// Instances of the template C++ class for common data types.
MakeCleanupClass(CEnsureCloseHandle,        HANDLE,    CloseHandle);
MakeCleanupClassX(CEnsureCloseSocket, SOCKET, closesocket, INVALID_SOCKET);

namespace network {

	const BYTE ReqSysServices[]={0x30, 0x26, 0x02, 0x01, 0x00, 0x04, 0x06, 0x70, 0x75, 0x62, 0x6C, 0x69, 0x63, 0xA0, 0x19, 0x02, 0x01, 0x02, 0x02, 0x01, 0x00, 0x02, 0x01, 0x00, 0x30, 0x0E, 0x30, 0x0C, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x02, 0x01, 0x01, 0x07, 0x00, 0x05, 0x00};

	VOID AddToNetworkSet(str_set& d_set, const std::string& ip, const std::string& subnet)
	{
		// Network�� ���Ͽ�
		u_int i = htonl(inet_addr(ip.c_str()));		// Little Endian
		u_int s = htonl(inet_addr(subnet.c_str()));	// Little Endian
		u_int net = (i & s);

		// �߰��Ѵ�
		struct in_addr tmp_in_addr;
		tmp_in_addr.s_addr = ntohl(net);	// Little Endian -> Big Endian(TCP/IP network byte order)

		d_set.insert( pair<string, string>(inet_ntoa(tmp_in_addr),subnet) );
	}

	VOID GetAliveSNMP_Range(const std::string& ipStart, const std::string& ipEnd, vector<string>& ips)
	{
		const UINT SnmpPort = 161;	// SNMP Port�� �����̶�� ����
		const int nTimerUnitsPerMilliSecond = 10000;

		// IP Gen
		IPRange r(ipStart, ipEnd, false);
		u_long maxIPcount = r.endPos();
		if(maxIPcount == 0) return;

		vector<CEnsureCloseSocket> s_v(maxIPcount);
		vector<BOOL> b_v(maxIPcount);
		vector<CEnsureCloseSocket>::size_type pos;

		// ���Ÿ�̸� ����
		CEnsureCloseHandle hTimer = ::CreateWaitableTimer(NULL, FALSE, NULL);
		LARGE_INTEGER li;
		int timeout = Properties::GetSNMPTimeout();
		li.QuadPart = -(timeout * nTimerUnitsPerMilliSecond);

		::SetWaitableTimer(hTimer, &li, 0, NULL, NULL, FALSE);
		// ���ϻ���
		for(pos=0; pos<maxIPcount; ++pos) {
			int ret;

			s_v[pos] = socket(AF_INET, SOCK_DGRAM, 0);
			if(s_v[pos] == INVALID_SOCKET) {
				return;
			}
			
			// �ͺ��ŷ ���� ��ȯ
			u_long ul = 1;
			if( SOCKET_ERROR==ioctlsocket(s_v[pos], FIONBIO, &ul) ) {
				return;
			}

			// FLAG SET
			b_v[pos] = FALSE;

			// �ּ�
			SOCKADDR_IN sAddr;
			ZeroMemory(&sAddr, sizeof(sAddr));
			sAddr.sin_family = AF_INET;
			sAddr.sin_port	= htons(SnmpPort);
			string c_ip = r.next().c_str();
			sAddr.sin_addr.s_addr = inet_addr(c_ip.c_str());

			// ������ ������
			ret = sendto( s_v[pos], (const char *)ReqSysServices, (sizeof(ReqSysServices) / sizeof(BYTE)),
				0, (SOCKADDR *)&sAddr, sizeof(sAddr) );
			if(ret == SOCKET_ERROR) {
				DWORD dw = ::WSAGetLastError();
				return;
			}
			
		}

		// ������ �ޱ�
		u_long remainNum;
		DWORD dwTimer;
		do {
			remainNum = 0;

			for(pos=0; pos<maxIPcount; ++pos) {
				if(b_v[pos]) continue;

				int ret;
				int addrlen;
				SOCKADDR_IN cAddr;
				BYTE res[64] = {0};
				addrlen = sizeof(cAddr);

				ret = recvfrom(s_v[pos], (char *)res, 64, 0, (SOCKADDR *)&cAddr, &addrlen);

				if(ret == SOCKET_ERROR) {
					DWORD dw = GetLastError();	// WSAETIMEDOUT
					if(WSAEWOULDBLOCK!=dw) {
						b_v[pos] = TRUE;
					}
				} else {
					// �����迡 �߰�
					ips.push_back(inet_ntoa(cAddr.sin_addr));
					b_v[pos] = TRUE;
				}
				remainNum++;
			}

			dwTimer = ::WaitForSingleObjectEx(hTimer, 0, TRUE);
			
		} while(remainNum && WAIT_TIMEOUT==dwTimer);
	}

	VOID GetAliveSNMP(const std::string& ip, const std::string& subnet, vector<string>& ips)
	{
		const int max_concurrency_number = Properties::GetConcurrentConnect();

		struct in_addr tmp_in_addr;

		IPRange r(ip, subnet, true);
		u_long stIP = r.beginUL();
		u_long edIP = r.endUL()-1;	// ������ .255�� ��ε��ɽ�Ʈ �ּҶ� ����

		u_long i_st = stIP;
		u_long i_ed = i_st + max_concurrency_number;
		for( ; i_ed<=edIP; ) {
			tmp_in_addr.s_addr = ntohl(i_st);	// Little Endian -> Big Endian(TCP/IP network byte order)
			string startIP(inet_ntoa(tmp_in_addr));

			tmp_in_addr.s_addr = ntohl(i_ed-1);	// Little Endian -> Big Endian(TCP/IP network byte order)
			string endIP(inet_ntoa(tmp_in_addr));

			printf("GetAliveSNMP_Range: %s ~ %s\n", startIP.c_str(), endIP.c_str());
			GetAliveSNMP_Range(startIP, endIP, ips);

			i_st = i_ed;
			i_ed = i_st + max_concurrency_number;
		}

		if(i_st <= edIP) {
			tmp_in_addr.s_addr = ntohl(i_st);	// Little Endian -> Big Endian(TCP/IP network byte order)
			string startIP(inet_ntoa(tmp_in_addr));

			tmp_in_addr.s_addr = ntohl(edIP);	// Little Endian -> Big Endian(TCP/IP network byte order)
			string endIP(inet_ntoa(tmp_in_addr));

			printf("GetAliveSNMP_Range: %s ~ %s\n", startIP.c_str(), endIP.c_str());
			GetAliveSNMP_Range(startIP, endIP, ips);

		}

		return;
	}
	
	VOID MakeNetworkMap(const str_set& s)
	{
		// ��Ʈ��ũ���� ����ִ� IP����� ���ϰ�
		vector<string> ips;
		str_set::const_iterator pos;
		for(pos=s.begin(); pos!=s.end(); ++pos) {
			GetAliveSNMP(pos->first, pos->second, ips);
		}
	}

} /* namespace network */


////////////////////////////////////////////////////////////////////////////////////////////////////
// Implement of IPRange
IPRange::IPRange(void)
{
	m_ul_IPEnd	 = 1;
	m_ul_IPBegin = 2;	// cross �ؼ� �E�� ����
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

	if(isSubnetMask == false) {	// �Է� : ip 2��
		m_ul_IPBegin	= ul_ip1;
		m_ul_IPEnd		= ul_ip2;
	}
	else {	// �Է�: �� ��Ʈ��ũ�� ip�� �����
		m_ul_IPBegin	= getBeginIP(ul_ip1, ul_ip2);
		m_ul_IPEnd		= getEndIP(ul_ip1, ul_ip2);
	}
	initPos();	//	m_currPos = 0;
}

// Position ����
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

// ��ȯ����
unsigned int IPRange::str2uint(std::string ip)
{
	return htonl(inet_addr(ip.c_str()));	// Little Endian
}
std::string IPRange::uint2str(unsigned long ip)
{
	struct in_addr tmp_in_addr;
	tmp_in_addr.s_addr = ntohl(ip);	// Little Endian -> Big Endian(TCP/IP network byte order)

	return inet_ntoa(tmp_in_addr);
}


namespace Properties
{
	int snmp_timeout = 300;
	int concurrentLimit = 3000;
	char start_IP[20], end_IP[20];

	void SetSNMPTimeout(int time)
	{
		snmp_timeout = time;
	}

	int GetSNMPTimeout()
	{
		return snmp_timeout;
	}
	void SetConcurrentConnect(int limit)
	{
		concurrentLimit = limit;
	}
	int GetConcurrentConnect()
	{
		return concurrentLimit;
	}

	void SetNetscanStartIP(const char *startIP)
	{
		lstrcpyA(start_IP, startIP);
	}
	const char * GetNetscanStartIP()
	{
		return start_IP;
	}
	void SetNetscanEndIP(const char *endIP)
	{
		lstrcpyA(end_IP, endIP);
	}
	const char * GetNetscanEndIP()
	{
		return end_IP;
	}
	bool IsSetCustomRange()
	{
		return (lstrlenA(start_IP)>0 && lstrlenA(end_IP)>0);
	}
}

class winsock_auto_cleanup_imple
{
private:
	WSADATA wsa;
	bool didWSAStartup;
public:
	winsock_auto_cleanup_imple():didWSAStartup(false) {
		// ���� �ʱ�ȭ
		if(WSAStartup(MAKEWORD(2,2), &wsa) == 0) didWSAStartup = true;
#ifdef	USE_ARP_IN_LOCALNETWORK_GET_MAC_SUBNET
		// ���� ��Ʈ��ũ ���� �ʱ�ȭ
		network::my::Init();
#endif
	}

	~winsock_auto_cleanup_imple() {
		if(didWSAStartup) WSACleanup();
	}
};


winsock_auto_cleanup::winsock_auto_cleanup()
:m_pImpl(new winsock_auto_cleanup_imple())
{
}

winsock_auto_cleanup::~winsock_auto_cleanup()
{
}

