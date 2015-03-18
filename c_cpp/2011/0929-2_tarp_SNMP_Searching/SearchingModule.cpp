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

// 윈속초기화 및 자동 해제 객체
class winsock_auto_cleanup_imple;
class winsock_auto_cleanup
{
public:
    winsock_auto_cleanup();
    ~winsock_auto_cleanup();
private:
    std::auto_ptr<winsock_auto_cleanup_imple>    m_pImpl;
};

// 함수 전방선언
namespace network {
	VOID AddToNetworkSet(str_set& d_set, const std::string& ip, const std::string& subnet);
	VOID MakeNetworkMap(const str_set& s);
	
} /* namespace network */

// 함수 구현
void GetChangeDevices(const LISTS_OF_DEVICE& devs_f)
{
	using namespace network;
	// WinSock
	winsock_auto_cleanup ws_auto;

	LISTS_OF_DEVICE::size_type pos;
	LISTS_OF_DEVICE::size_type size = devs_f.size();

	// 찾을 범위(네트워크)를 구해서 네트워크 SET에 넣는다
	str_set d_set;	// 네트워크 셋
	for(pos=0; pos<size; ++pos) {
		LIST_OF_DEVICES aInfo = devs_f.at(pos);
		AddToNetworkSet(d_set, aInfo.ip, aInfo.subnet);
	}

	// 네트워크 맵을 작성
	MakeNetworkMap(d_set);
}

// 알고리즘 정책
//#define	USE_ARP_IN_LOCALNETWORK_GET_MAC_SUBNET

////////////////////////////////////////////////////////////////////////////////////////////////////
class IPRange
{
public:
	IPRange(void);
	IPRange(std::string ip1, std::string ip2, bool isSubnetMask=false);
	void	setIP(std::string ip1, std::string ip2, bool isSubnetMask=false);
	~IPRange(void);
	// Position을 특정 값으로 리셋(범위를 초과할 경우에는 설정이 되지 않고, false리턴)
	bool			setPos(unsigned long pos);
	// Position을 리셋(0으로 설정)
	void			initPos()	{	setPos(0);			}
	// 현재 Position을 반환
	unsigned long	currPos()	{	return m_currPos;	}
	// 시작 Position을 반환(항상 0)
	unsigned long	beginPos()	{	return 0;}
	// 마지막 Position을 반환
	unsigned long	endPos()	{	return (m_ul_IPEnd - m_ul_IPBegin);	}
	// 현재 설정된 시작IP를 반환
	std::string		begin()		{	return uint2str(m_ul_IPBegin);	}
	// 현재 Position의 IP를 반환하며, 현재 Position을 하나 증가 시킨다, 맨 끝을 초과 할때는 빈문자열 반환
	std::string		next();
	// 현재 설정된 마지막IP를 반환
	std::string		end()		{	return uint2str(m_ul_IPEnd);	}
	
	// 현재 설정된 시작IP의 unsigned long 형태를 반환
	unsigned long		beginUL()		{	return m_ul_IPBegin;	}
	// 현재 설정된 마지막IP의 unsigned long 형태를 반환
	unsigned long		endUL()		{	return m_ul_IPEnd;	}
private:
	unsigned long m_ul_IPBegin;	// plus offset
	unsigned long m_ul_IPEnd;
	unsigned long m_currPos;
	unsigned long m_ulSubnetmask;
	// 변환관련
	unsigned int	str2uint(std::string ip);
	std::string		uint2str(unsigned long ip);
	unsigned long	getBeginIP(unsigned long ul_ip, unsigned long ul_subnet){	return (ul_ip & ul_subnet)+1;	  }	// 출발 주소 구하기
	unsigned long	getEndIP  (unsigned long ul_ip, unsigned long ul_subnet){	return (ul_ip | (~ul_subnet));}	// 마지막 주소 구하기
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
		// Network를 구하여
		u_int i = htonl(inet_addr(ip.c_str()));		// Little Endian
		u_int s = htonl(inet_addr(subnet.c_str()));	// Little Endian
		u_int net = (i & s);

		// 추가한다
		struct in_addr tmp_in_addr;
		tmp_in_addr.s_addr = ntohl(net);	// Little Endian -> Big Endian(TCP/IP network byte order)

		d_set.insert( pair<string, string>(inet_ntoa(tmp_in_addr),subnet) );
	}

	VOID GetAliveSNMP_Range(const std::string& ipStart, const std::string& ipEnd, vector<string>& ips)
	{
		const UINT SnmpPort = 161;	// SNMP Port는 고정이라고 가정
		const int nTimerUnitsPerMilliSecond = 10000;

		// IP Gen
		IPRange r(ipStart, ipEnd, false);
		u_long maxIPcount = r.endPos();
		if(maxIPcount == 0) return;

		vector<CEnsureCloseSocket> s_v(maxIPcount);
		vector<BOOL> b_v(maxIPcount);
		vector<CEnsureCloseSocket>::size_type pos;

		// 대기타이머 생성
		CEnsureCloseHandle hTimer = ::CreateWaitableTimer(NULL, FALSE, NULL);
		LARGE_INTEGER li;
		int timeout = Properties::GetSNMPTimeout();
		li.QuadPart = -(timeout * nTimerUnitsPerMilliSecond);

		::SetWaitableTimer(hTimer, &li, 0, NULL, NULL, FALSE);
		// 소켓생성
		for(pos=0; pos<maxIPcount; ++pos) {
			int ret;

			s_v[pos] = socket(AF_INET, SOCK_DGRAM, 0);
			if(s_v[pos] == INVALID_SOCKET) {
				return;
			}
			
			// 넌블로킹 모드로 전환
			u_long ul = 1;
			if( SOCKET_ERROR==ioctlsocket(s_v[pos], FIONBIO, &ul) ) {
				return;
			}

			// FLAG SET
			b_v[pos] = FALSE;

			// 주소
			SOCKADDR_IN sAddr;
			ZeroMemory(&sAddr, sizeof(sAddr));
			sAddr.sin_family = AF_INET;
			sAddr.sin_port	= htons(SnmpPort);
			string c_ip = r.next().c_str();
			sAddr.sin_addr.s_addr = inet_addr(c_ip.c_str());

			// 데이터 보내기
			ret = sendto( s_v[pos], (const char *)ReqSysServices, (sizeof(ReqSysServices) / sizeof(BYTE)),
				0, (SOCKADDR *)&sAddr, sizeof(sAddr) );
			if(ret == SOCKET_ERROR) {
				DWORD dw = ::WSAGetLastError();
				return;
			}
			
		}

		// 데이터 받기
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
					// 응답기계에 추가
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
		u_long edIP = r.endUL()-1;	// 마지막 .255는 브로드케스트 주소라서 제외

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
		// 네트워크셋중 살아있는 IP목록을 구하고
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
		// 윈속 초기화
		if(WSAStartup(MAKEWORD(2,2), &wsa) == 0) didWSAStartup = true;
#ifdef	USE_ARP_IN_LOCALNETWORK_GET_MAC_SUBNET
		// 나의 네트워크 정보 초기화
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

