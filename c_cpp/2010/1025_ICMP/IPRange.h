#pragma once
#include <string>

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
private:
	unsigned long m_ul_IPBegin;	// plus offset
	unsigned long m_ul_IPEnd;
	unsigned long m_currPos;
	// 변환관련
	unsigned int	str2uint(std::string ip);
	std::string		uint2str(unsigned long ip);
	unsigned long	getBeginIP(unsigned long ul_ip, unsigned long ul_subnet){	return (ul_ip & ul_subnet)+1;	  }	// 출발 주소 구하기
	unsigned long	getEndIP  (unsigned long ul_ip, unsigned long ul_subnet){	return (ul_ip | (~ul_subnet))-1;}	// 마지막 주소 구하기
};

bool IsICMP_OK(const std::string& ip);
