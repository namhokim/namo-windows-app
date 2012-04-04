#pragma once 

#ifndef _PHY_ADDR_H_
#define _PHY_ADDR_H_

#include <string>

namespace Network
{

	// Route table 중 Metric이 가장 적은 인터페이스의 MAC주소를 반환
	// 한번 수행후에는 레지스트리에서 가져오므로 변화가 없다.
	// 반환값: true - 성공
	//		   false - 실패(MAC 주소 획득에 실패)
	bool GetBasicMacAddress(std::string& out);
	bool GetBasicMacAddress(std::wstring& out);

} // end of namespace Network
#endif