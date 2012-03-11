#pragma once
#include "StringUtil.h"
#include <fstream>
#include <map>

typedef std::map<_STRING, _STRING> stringKV;

class PJLReader
{
public:
	PJLReader(void);
	~PJLReader(void);
	bool open(_STRING filename);
	bool isOpened()			{	return m_inputStream.is_open();	}
	bool fillPJLinfo();									// �Ľ��� �Ѵ�
	stringKV getKVMap()		{	return m_ssMap;	}		// ����� ��ȯ
private:
	std::ifstream m_inputStream;	// �Է½�Ʈ��
	stringKV m_ssMap;				// ��� ��(���)
	namo::StringUtil su;
	std::string toA(std::wstring in)	{	return su.wcs_to_mbs(in,std::locale("korean"));	}
	std::wstring toW(std::string in)	{	return su.mbs_to_wcs(in,std::locale("korean"));	}
	// �Ľ̿� �޼ҵ�
	std::string removePreWords(std::string inString);					// @PJL�� ���ʿ��� ������ �����Ѵ�
	_STRING extractKeyAndValue(_STRING afterRemovedPreWords);	// Key = Value�� �����Ѵ�
	bool setKeyAndValue(_STRING keyEqualValue);					// m_ssMap�� key, value�� �߰��Ѵ�
};
