#pragma once
#ifndef _NAMO_PERCENT_ENCODING_H_
#define	_NAMO_PERCENT_ENCODING_H_

#include <string>

namespace namo
{

	// General String to Percent-Encoding
	// Cf. RFC 3986
	//	Uniform Resource Identifier (URI): Generic Syntax
	std::string ToPercentEncoding(const std::string& str);

	// Before
	std::string URIEscape(const std::string& uri);

}	/* end of namespace namo */

#endif	/* end of _NAMO_PERCENT_ENCODING_H_ */