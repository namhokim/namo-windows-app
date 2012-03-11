#include "URL.h"
#include <iostream>
#include <map>
#include <algorithm>

namespace namo {
	namespace Deliminator {
		const char * szScheme = "://";
		const char * szPort = ":";
		const char * szPath = "/";
		const char * szQuery = "?";
		const char * szRef = "#";
	}

	namespace SchemeType {
		const char * szHTTP		= "http";	// 80
		const char * szHTTPS	= "https";	// 443
		const char * szFTP		= "ftp";	// 21
		const char * szTELNET	= "telnet";	// 23
		const char * szSSH		= "ssh";	// 22
		const char * szPOP3		= "pop3";	// 110

		unsigned short getPort(const char *scheme) {
			if(_stricmp(szHTTP, scheme)==0) return 80;
			if(_stricmp(szHTTPS, scheme)==0) return 443;
			if(_stricmp(szFTP, scheme)==0) return 21;
			if(_stricmp(szTELNET, scheme)==0) return 23;
			if(_stricmp(szSSH, scheme)==0) return 22;
			if(_stricmp(szPOP3, scheme)==0) return 110;
			return 0;
		}
	}

	std::string URIEscape(std::string uri)
	{
		std::string strBuf;
		strBuf.reserve(uri.size());

		std::string::size_type pos, size=uri.size();
		char cBuf[4];
		for(pos=0; pos<size; ++pos) {
			unsigned char uc = (unsigned char)uri.at(pos);
			if(::isalpha(uc) || ::isdigit(uc)
				|| ('$'==uc) || ('-'==uc) || ('_'==uc) || ('.'==uc)
				|| ('+'==uc) || ('!'==uc) || ('*'==uc) || ('\''==uc)
				|| ('('==uc) || (')'==uc) || (','==uc) ) {
					sprintf_s(cBuf, 4, "%c", uc);
			} else {
				sprintf_s(cBuf, 4, "%%%2X", uc);
			}
			strBuf.append(cBuf);
		}

		return strBuf;
	}

URL::URL(std::string uri)
:m_port(0)
{
	if(uri.empty()) return;

	// Protocol
	std::string::size_type pos = uri.find(Deliminator::szScheme);
	if(std::string::npos != pos) {
		m_protocol = uri.substr(0, pos);
		std::transform( m_protocol.begin(), m_protocol.end(), m_protocol.begin(), ::tolower );
		m_isAbsolute = true;
		uri = uri.substr(pos+strlen(Deliminator::szScheme));
		m_port = SchemeType::getPort(m_protocol.c_str());
	} else {
		m_isAbsolute = false;
	}

	// Host & Port 1
	pos = uri.find(Deliminator::szPort);
	if(std::string::npos != pos) {
		m_host = uri.substr(0, pos);
		uri = uri.substr(pos + ::strlen(Deliminator::szPort));
	}
	
	// Host & Port 2
	pos = uri.find(Deliminator::szPath);
	if(std::string::npos != pos) {
		if(m_host.empty()) {
			m_host = uri.substr(0, pos);
		} else {
			m_port = ::atoi(uri.substr(0, pos).c_str());
		}
		uri = uri.substr(pos);	// not pos+1 (because for include '/')
	} else {
		m_host = uri;
		return;
	}

	// Path
	pos = uri.find(Deliminator::szQuery);
	if(std::string::npos != pos) {
		m_path = uri.substr(0, pos);
		uri = uri.substr(pos+1);
	}

	// Query & Ref
	pos = uri.find(Deliminator::szRef);
	if(std::string::npos != pos) {
		if(m_path.empty()) {
			m_path = uri.substr(0, pos);
		} else {
			m_query = uri.substr(0, pos);
		}
		m_ref = uri.substr(pos+1);
	} else {
		if(m_path.empty()) {
			m_path = uri;
		} else {
			m_query = uri;
		}
	}
}

URL::~URL(void)
{
}

bool URL::isAbsolute()
{
	// 만약 스킴부분이 생략되어 있다면
	// 상대 URL이다
	return false;
}

std::string	URL::getProtocol()
{
	return m_protocol;
}
std::string	URL::getHost()
{
	return	m_host;
}
unsigned short URL::getPort()
{
	return m_port;
}

std::string	URL::getPath()
{
	return m_path;
}
std::string	URL::getQuery()
{
	return m_query;
}
std::string	URL::getRef()
{
	return m_ref;
}


}	// end of namespace namo