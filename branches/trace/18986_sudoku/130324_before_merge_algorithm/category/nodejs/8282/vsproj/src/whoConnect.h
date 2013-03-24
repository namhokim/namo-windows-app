#pragma once

#include <string>		// for std::string, std::wstring
#include "json/json.h"

namespace wts {

	// 현재 접속한 사용자의 정보 획득
	int ShowCurrentConnectedUser(
		Json::Value& out
		);

	// 특정 세션ID에 메세지를 전송
	int SendMessageToSesstionID(
		unsigned int SessionId,
		const wchar_t* message
		);

	// 특정 세션ID의 접속을 끊음
	int DisconnectSession(
		unsigned int SessionId
		);

	// 접속 이벤트(연결,연결끊김) 콜백함수(블록함수)
	unsigned int WaitForEvent(unsigned int* pStausCode, std::string& errorMsg);

} // end of namespace wts
