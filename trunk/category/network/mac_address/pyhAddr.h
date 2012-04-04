#pragma once 

#ifndef _PHY_ADDR_H_
#define _PHY_ADDR_H_

#include <string>

namespace Network
{

	// Route table �� Metric�� ���� ���� �������̽��� MAC�ּҸ� ��ȯ
	// �ѹ� �����Ŀ��� ������Ʈ������ �������Ƿ� ��ȭ�� ����.
	// ��ȯ��: true - ����
	//		   false - ����(MAC �ּ� ȹ�濡 ����)
	bool GetBasicMacAddress(std::string& out);
	bool GetBasicMacAddress(std::wstring& out);

} // end of namespace Network
#endif