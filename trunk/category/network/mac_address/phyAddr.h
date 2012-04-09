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

	// 기능  : Default Gateway가 존재하는 NIC중
	//         메트릭이 가장 작은 MAC 주소를 벡터로 반환
	// 반환값: true - 성공
	//         false - 실패(MAC 주소 획득에 실패)
	bool GetBestInterfaceAddress(std::vector<unsigned char>& out);


	// Route table 중 Metric이 가장 적은 인터페이스의 MAC주소를 반환
	// 한번 수행후에는 레지스트리에서 가져오므로 변화가 없다.
	// 반환값: true - 성공
	//		   false - 실패(MAC 주소 획득에 실패)
	// 예:
	// 1) 기본호출 -> F0-DE-F1-D9-FD-15 (구분자 '-', 대문자)
	// std::string mac;
	// std::wstring mac_w;
	// Network::GetBasicMacAddress(mac);
	// Network::GetBasicMacAddress(mac_w);
	//
	// 2) 구분자 변경호출(2번째 인자에 char 나 wchar_t 의 값 입력)
	//
	//   구분자를 없애려면 값 0 이나 '\0'(혹은 L'\0')을 넣음
	// string mac;
	// Network::GetBasicMacAddress(mac, '\0');
	//
	// 3) 소문자로 작성(2번째 인자에 '-'(혹은 L'-')을 주고, 3번째 인자에 Network::ByUpper 지정
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
			out.clear();

			for (pos=first; pos<size; ++pos) {
				if(first!=pos && seperator) out.push_back(seperator);
				
				std::basic_stringstream<_Elem, std::char_traits<_Elem>, std::allocator<_Elem> > ss;
				if(ByUpper==char_case) ss.setf(std::ios_base::uppercase);
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