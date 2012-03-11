#pragma once
#include "unicode.h"

namespace namo {
class BuildCSV
{
public:
	BuildCSV(void);
	~BuildCSV(void);
	_STRING getFileName();
	bool make();
	bool remove();
	std::wstring getLastErrorMsg();
private:
	_STRING GetCSVName();
	_STRING outputFileName;
	bool isSuccessOutputCSV;
	bool saveToFile(std::wstring &content);
	std::string wcs_to_mbs(std::wstring const& str, std::locale const& loc = std::locale());
	std::wstring mbs_to_wcs(std::string const& str, std::locale const& loc = std::locale());
	std::wstring lastErrorMsg;
	void setLastErrorMsg(std::wstring msg);
};
} // namespace namo