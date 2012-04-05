#pragma once 

#ifndef _PHY_ADDR_H_
#define _PHY_ADDR_H_

#include <sstream>	// for std::basic_stringstream<_Elem,_Traits,_Alloc>
#include <iomanip>	// for setw, setfill
#include <string>
#include <vector>

namespace Network
{
	enum Case
	{
		ByUpper,
		ByLower,
	};

	// ���  : Default Gateway�� �����ϴ� NIC��
	//         ��Ʈ���� ���� ���� MAC �ּҸ� ���ͷ� ��ȯ
	// ��ȯ��: true - ����
	//         false - ����(MAC �ּ� ȹ�濡 ����)
	bool GetBestInterfaceAddress(std::vector<unsigned char>& out);


	// Route table �� Metric�� ���� ���� �������̽��� MAC�ּҸ� ��ȯ
	// �ѹ� �����Ŀ��� ������Ʈ������ �������Ƿ� ��ȭ�� ����.
	// ��ȯ��: true - ����
	//		   false - ����(MAC �ּ� ȹ�濡 ����)
	// ��:
	// 1) �⺻ȣ�� -> F0-DE-F1-D9-FD-15 (������ '-', �빮��)
	// std::string mac;
	// std::wstring mac_w;
	// Network::GetBasicMacAddress(mac);
	// Network::GetBasicMacAddress(mac_w);
	//
	// 2) ������ ����ȣ��(2��° ���ڿ� char �� wchar_t �� �� �Է�)
	//
	//   �����ڸ� ���ַ��� �� 0 �̳� '\0'(Ȥ�� L'\0')�� ����
	// string mac;
	// Network::GetBasicMacAddress(mac, '\0');
	//
	// 3) �ҹ��ڷ� �ۼ�(2��° ���ڿ� '-'(Ȥ�� L'-')�� �ְ�, 3��° ���ڿ� Network::ByUpper ����
	// string mac;
	// Network::GetBasicMacAddress(mac, '-', Network::ByLower);
	//
	template < typename _Elem >
	bool GetBasicMacAddress(
		std::basic_string<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> > & out,
		const _Elem seperator = '-',
		Case char_case = ByUpper )
	{
		const size_t first = 0;
		const _Elem  pad = '0';

		std::vector<unsigned char> mac;
		if(GetBestInterfaceAddress(mac)) {
			std::vector<unsigned char>::size_type pos, size=mac.size();
			for (pos=first; pos<size; ++pos) {
				if(first!=pos && seperator) out.push_back(seperator);
				
				std::basic_stringstream<_Elem, char_traits<_Elem>, allocator<_Elem> > ss;
				if(ByUpper==char_case) ss.setf(ios_base::uppercase);
				ss << std::setw(2) << std::setfill(pad) << std::hex << static_cast<short>(mac[pos]);
				out.append(ss.str());
			}
			return true;
		} else {
			return false;
		}
	}

} // end of namespace Network
#endif