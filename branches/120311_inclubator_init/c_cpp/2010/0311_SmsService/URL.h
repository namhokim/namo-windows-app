#pragma once

#ifndef _NAMO_URI_H_
#define _NAMO_URI_H_

#include <string>

namespace namo {
	namespace SchemeType {
		extern const char * szHTTP;
		extern const char * szHTTPS;
		extern const char * szFTP;
		extern const char * szTELNET;
		extern const char * szSSH;
		extern const char * szPOP3;
	}

	// 참고 : RFC 1738
	// http://blog.5hoon.com/50
	std::string URIEscape(std::string uri);

	// 참고 : Handling Uniform Resource Locators
	//			(http://msdn.microsoft.com/en-us/library/aa384225(VS.85).aspx)
	// URL은 크게 다섯 부분으로 이루어짐
	// protocol://host:port/path?query#ref
	// 예. http://ibiblio.org:8080/javafaq/books/jnp/index.html?isbn=1565922069#toc
	// protocol - http
	// host		- ibiblio.org
	// port		- 8080
	// path		- /javafaq/books/jnp/index.html
	// query	- isbn=1565922069
	// ref		- toc
	class URL
	{
	public:
		URL(std::string uri);
		~URL(void);
		bool isAbsolute();

		std::string		getProtocol();
		std::string		getHost();
		unsigned short	getPort();
		std::string		getPath();
		std::string		getQuery();
		std::string		getRef();
	private:
		bool			m_isAbsolute;
		std::string		m_protocol;
		std::string		m_host;
		unsigned short	m_port;
		std::string		m_path;
		std::string		m_query;
		std::string		m_ref;
	};
}	// end of namespace namo

#endif