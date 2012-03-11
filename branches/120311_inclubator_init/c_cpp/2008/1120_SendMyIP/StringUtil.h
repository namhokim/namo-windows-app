#include <string>
std::string wcs_to_mbs(std::wstring const& str, std::locale const& loc = std::locale());
std::wstring mbs_to_wcs(std::string const& str, std::locale const& loc = std::locale());