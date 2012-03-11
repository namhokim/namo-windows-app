#include "PJLReader.h"
#include <sstream>
#include <locale>

PJLReader::PJLReader(void)
{

}

PJLReader::~PJLReader(void)
{
	if(isOpened()) m_inputStream.close();
}

bool PJLReader::open(_STRING filename)
{
#ifdef _UNICODE
	m_inputStream.open(toA(filename).c_str());
#else
	m_inputStream.open(filename.c_str());
#endif
	return isOpened();
}

bool PJLReader::fillPJLinfo()
{
	bool res;

	if(!isOpened()) return false;

	// map에 key, value 세팅
	std::string stringLineBuffer;
	while(std::getline(m_inputStream, stringLineBuffer, '\n')) {
#ifdef _UNICODE
		res = setKeyAndValue(extractKeyAndValue(toW(removePreWords(stringLineBuffer))));
#else
		res = setKeyAndValue(extractKeyAndValue(removePreWords(stringLineBuffer)));
#endif
		
	}
	return true;
}

std::string PJLReader::removePreWords(std::string inString)
{
	std::string::size_type position = inString.find("@PJL ");
	if(position != std::string::npos) return inString.substr(position, inString.size());
	else return "";
}

_STRING PJLReader::extractKeyAndValue(_STRING afterRemovedPreWords)
{
	if(afterRemovedPreWords.empty()) return _T("");

	_STRINGSTREAM ss(afterRemovedPreWords);
	_STRING buffer;
	_STRING output(_T(""));
	unsigned int i = 1;
	const unsigned int afterCommand = 2;	// 두 번째 white space 다음이 Key = Value
	_STRING key;
	_STRING valueFirstWord;
	while(ss >> buffer)
	{
		switch(i) {
			case 3:
				key = buffer;
				break;
			case 5:
				valueFirstWord = buffer;
				break;
			default:
				break;
		}
		i++;

		//if(i > afterCommand) output.append(buffer);
	}
	output = key;
	output.append(_T("="));
	_STRING afterEqual = afterRemovedPreWords.substr(afterRemovedPreWords.find(_T("=")), afterRemovedPreWords.size());
	output.append(afterEqual.substr(afterEqual.find(valueFirstWord), afterEqual.size()));
	return output;
}

bool PJLReader::setKeyAndValue(_STRING keyEqualValue)
{
	const _STRING split_token(_T("="));
	_STRING::size_type position = keyEqualValue.find(split_token);
	if(position != _STRING::npos) {
		m_ssMap[keyEqualValue.substr(0, position)] = keyEqualValue.substr(position+split_token.size(), keyEqualValue.size());
		return true;
	}
	else return false;
}
