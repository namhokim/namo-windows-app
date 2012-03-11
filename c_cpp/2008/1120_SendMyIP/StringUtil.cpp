#include "StringUtil.h"
#include <vector>

std::string
wcs_to_mbs(std::wstring const& str, std::locale const& loc)
{
    typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
    codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
    std::mbstate_t state = 0;
    std::vector<char> buf((str.size() + 1) * codecvt.max_length());
    wchar_t const* in_next = str.c_str();
    char* out_next = &buf[0];
    codecvt_t::result r = codecvt.out(state,
        str.c_str(), str.c_str() + str.size(), in_next,
        &buf[0], &buf[0] + buf.size(), out_next);
    return std::string(&buf[0]);
}

std::wstring
mbs_to_wcs(std::string const& str, std::locale const& loc)
{
    typedef std::codecvt<wchar_t, char, std::mbstate_t> codecvt_t;
    codecvt_t const& codecvt = std::use_facet<codecvt_t>(loc);
    std::mbstate_t state = 0;
    std::vector<wchar_t> buf(str.size() + 1);
    char const* in_next = str.c_str();
    wchar_t* out_next = &buf[0];
    codecvt_t::result r = codecvt.in(state,
        str.c_str(), str.c_str() + str.size(), in_next,
        &buf[0], &buf[0] + buf.size(), out_next);
    return std::wstring(&buf[0]);
}