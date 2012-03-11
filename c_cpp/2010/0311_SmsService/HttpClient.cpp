#include "HttpClient.h"
#include "URL.h"

namespace namo {
	LPCSTR szDefUsrAgent	= "Namo HTTP Client Class";
	LPCSTR szHttpVer1_1		= "HTTP/1.1";
	LPCSTR szAcceptedAnyType[] = {"*/*", NULL};

	namespace HttpVerb {
		LPCSTR str[] = {"GET", "HEAD", "POST" ,"PUT", "DELETE"};
		enum idx {
			GET_HTTP, HEAD_HTTP, POST_HTTP, PUT_HTTP, DELETE_HTTP,
		};
	}

	// struct of HttpClientHandle
	HttpClientHandle::HttpClientHandle()
		: hInternet(NULL), hConnect(NULL), hRequest(NULL)
	{
	}

	HttpClientHandle::~HttpClientHandle()
	{	
		closeAllHandle();
	}

	void HttpClientHandle::closeAllHandle()
	{
		closeRequestHandle();
		closeConnectHandle();
		closeInternetHandle();
	}

	void HttpClientHandle::closeInternetHandle()
	{
		if(NULL!=hInternet) {
			::InternetCloseHandle(hInternet);
			hInternet = NULL;
		}
	}

	void HttpClientHandle::closeConnectHandle()
	{
		if(NULL!=hConnect) {
			::InternetCloseHandle(hConnect);
			hConnect = NULL;
		}
	}

	void HttpClientHandle::closeRequestHandle()
	{
		if(NULL!=hRequest) {
			::InternetCloseHandle(hRequest);
			hRequest = NULL;
		}	
	}


	// class of HttpClient
	HttpClient::HttpClient(std::string app_name)
		: m_handle(), m_status_code(0), m_content_length(0)
	{
		if(app_name.empty()) {
			m_app_name = szDefUsrAgent;
		} else {
			m_app_name = app_name;
		}
	}

	HttpClient::~HttpClient(void)
	{
	}

	void HttpClient::addHeader(std::string header_key, std::string header_value)
	{
		std::string h(header_key);
		h.append(":");
		h.append(header_value);
		h.append("\r\n");

		m_header_knv.push_back(h);
	}

	void HttpClient::clearHeader()
	{
		m_header_knv.clear();
	}

	void HttpClient::addContent(std::string content)
	{
		m_request_content.append(content);
	}

	void HttpClient::addContent(std::string content_key, std::string content_value)
	{
		if(!m_request_content.empty()) {
			m_request_content.append("&");
		}

		m_request_content.append(content_key);
		m_request_content.append("=");
		m_request_content.append(content_value);
	}

	void HttpClient::clearContent()
	{
		m_request_content.clear();
	}

	DWORD HttpClient::getStatusCode()
	{
		return m_status_code;
	}

	DWORD HttpClient::getContentLength()
	{
		return m_content_length;
	}

	void HttpClient::cleanHandle()
	{
		m_handle.closeAllHandle();
	}
	bool HttpClient::request(HttpVerb::idx verb, std::string url)
	{
		// Initializes an application's use of the WinINet functions.
		m_handle.hInternet =
			::InternetOpenA(m_app_name.c_str(),		// specifies the name of the application(version?)
				INTERNET_OPEN_TYPE_PRECONFIG,	// Type of access required(add?)
				NULL,							// the name of the proxy server(s)
				NULL,							// Proxy Bypass list
				0);								// Flags(fix)
		if(NULL == m_handle.hInternet) {
			return false;
		}

		namo::URL u(url);
		// Opens an File Transfer Protocol (FTP) or HTTP session for a given site.
		m_handle.hConnect = 
			::InternetConnectA(m_handle.hInternet,		// Handle returned by a previous call to InternetOpen(need!)
					u.getHost().c_str(),	// the host name of an Internet server(need!)
					u.getPort(),			// Transmission Control Protocol/Internet Protocol (TCP/IP) port on the server(need!)
					"GetUser",				// Username(need?)
					"GETPassword",			// Password(need?)
					INTERNET_SERVICE_HTTP,	// Type of service to access(fix)
					0,						// Options specific to the service used(fix)
			0);

		if(NULL==m_handle.hConnect) {
			cleanHandle();
			return false;
		}

		m_handle.hRequest =
			::HttpOpenRequestA(m_handle.hConnect,		// handle to an HTTP session returned by InternetConnect.
					HttpVerb::str[verb],	// HTTP Method
					u.getPath().c_str(),	// Target object
					szHttpVer1_1,			// HTTP version(fix)
					NULL,					// specifies the URL of the document from which the URL in the request
					szAcceptedAnyType,		// Accepts any type.
					NULL,					// Flags (GET/POST ÀÚµ¿:INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_RELOAD)
					NULL);					// Doesn't use any context value.
		if(NULL==m_handle.hRequest) {
			cleanHandle();
			return false;
		}

		// Attach headers
		std::vector<std::string>::size_type pos, size = m_header_knv.size();
		for (pos=0; pos<size; ++pos) {
			std::string &pHeader = m_header_knv.at(pos);
			::HttpAddRequestHeadersA(m_handle.hRequest, pHeader.c_str(), pHeader.size(), HTTP_ADDREQ_FLAG_ADD);
		}

		// Request HTTP
		BOOL reqResult = FALSE;
		if(m_request_content.empty()) {
			reqResult = ::HttpSendRequestA(m_handle.hRequest, NULL, 0, NULL, 0);
		} else {	// attach request content
			reqResult = ::HttpSendRequestA(m_handle.hRequest, NULL, 0, (LPVOID)m_request_content.data(), m_request_content.size());
		}

		// Response Header
		if(reqResult) {
			DWORD dwBufLen = sizeof(m_status_code);
			BOOL res = ::HttpQueryInfo(m_handle.hRequest,
				HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, (LPVOID)&m_status_code, &dwBufLen, NULL);

			dwBufLen = sizeof(m_content_length);
			res = ::HttpQueryInfo(m_handle.hRequest,
				HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, (LPVOID)&m_content_length, &dwBufLen, NULL);
		} else {
			m_status_code = 0;
			m_content_length = 0;
		}

		// if HTTP GET
		if(HttpVerb::GET_HTTP == verb) {
			m_response_content.clear();
			if(m_content_length>0) m_response_content.reserve(m_content_length);
			DWORD dwAvailableSize, dwNumberOfBytesRead;
			do {
				::InternetQueryDataAvailable(m_handle.hRequest, &dwAvailableSize, 0, 0);
				if(dwAvailableSize <= 0) break;

				std::vector<char> buf(dwAvailableSize);
				::InternetReadFile(m_handle.hRequest, (LPVOID)&buf[0], dwAvailableSize, &dwNumberOfBytesRead);

				if(dwNumberOfBytesRead > 0) {
					m_response_content.append(buf.begin(), buf.end());
				}
			} while (dwNumberOfBytesRead != 0);
		}

		cleanHandle();
		return true;
	}

	bool HttpClient::requestGET(std::string url)
	{
		return request(HttpVerb::GET_HTTP, url);
	}
	bool HttpClient::requestHEAD(std::string url)
	{
		return request(HttpVerb::HEAD_HTTP, url);
	}
	bool HttpClient::requestPOST(std::string url)
	{
		return request(HttpVerb::POST_HTTP, url);
	}
	bool HttpClient::requestPUT(std::string url)
	{
		return request(HttpVerb::PUT_HTTP, url);
	}
	bool HttpClient::requestDELETE(std::string url)
	{
		return request(HttpVerb::DELETE_HTTP, url);
	}

	std::string & HttpClient::getContent()
	{
		return m_response_content;
	}

}	// end of namespace namo