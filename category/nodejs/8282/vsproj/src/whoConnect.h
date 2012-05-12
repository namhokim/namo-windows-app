#pragma once

#include <string>		// for std::string, std::wstring
#include "json/json.h"

namespace wts {

	// ���� ������ ������� ���� ȹ��
	int ShowCurrentConnectedUser(
		Json::Value& out
		);

	// Ư�� ����ID�� �޼����� ����
	int SendMessageToSesstionID(
		unsigned int SessionId,
		const wchar_t* message
		);

	// Ư�� ����ID�� ������ ����
	int DisconnectSession(
		unsigned int SessionId
		);

} // end of namespace wts
