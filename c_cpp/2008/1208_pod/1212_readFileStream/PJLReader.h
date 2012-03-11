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
	bool fillPJLinfo();									// 파싱을 한다
	stringKV getKVMap()		{	return m_ssMap;	}		// 결과맵 반환
private:
	std::ifstream m_inputStream;	// 입력스트림
	stringKV m_ssMap;				// 출력 맵(결과)
	namo::StringUtil su;
	std::string toA(std::wstring in)	{	return su.wcs_to_mbs(in,std::locale("korean"));	}
	std::wstring toW(std::string in)	{	return su.mbs_to_wcs(in,std::locale("korean"));	}
	// 파싱용 메소드
	std::string removePreWords(std::string inString);					// @PJL에 불필요한 문장을 제거한다
	_STRING extractKeyAndValue(_STRING afterRemovedPreWords);	// Key = Value를 추출한다
	bool setKeyAndValue(_STRING keyEqualValue);					// m_ssMap에 key, value를 추가한다
};
