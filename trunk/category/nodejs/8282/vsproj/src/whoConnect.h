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

} // end of namespace wts
