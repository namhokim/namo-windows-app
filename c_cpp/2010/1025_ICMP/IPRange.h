#pragma once
#include <string>

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
private:
	unsigned long m_ul_IPBegin;	// plus offset
	unsigned long m_ul_IPEnd;
	unsigned long m_currPos;
	// ��ȯ����
	unsigned int	str2uint(std::string ip);
	std::string		uint2str(unsigned long ip);
	unsigned long	getBeginIP(unsigned long ul_ip, unsigned long ul_subnet){	return (ul_ip & ul_subnet)+1;	  }	// ��� �ּ� ���ϱ�
	unsigned long	getEndIP  (unsigned long ul_ip, unsigned long ul_subnet){	return (ul_ip | (~ul_subnet))-1;}	// ������ �ּ� ���ϱ�
};

bool IsICMP_OK(const std::string& ip);
