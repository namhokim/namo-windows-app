#pragma once

#ifndef _NAMO_HTTP_CLIENT_H
#define	_NAMO_HTTP_CLIENT_H

#include <string>
#include <vector>

//typedef struct _knv {
//	std::string k;
//	std::string v;
//} knv;

#define	WIN32_LEAN_AND_MEAN	// 불필요한 내용 배제
#include <Windows.h>
#include <Wininet.h>	// Minimum supported client : Windows 2000 Professional 
#pragma comment(lib, "Wininet.lib")

namespace namo {

	namespace HttpVerb {
		extern LPCSTR str[];
		extern enum idx;
	}

	// for auto close handle
	struct HttpClientHandle {
		HINTERNET hInternet, hConnect, hRequest;

		HttpClientHandle();
		~HttpClientHandle();
		void closeAllHandle();
		void closeInternetHandle();
		void closeConnectHandle();
		void closeRequestHandle();
	};


	class HttpClient
	{
	public:
		HttpClient(std::string app_name);
		~HttpClient(void);

		// Header
		void addHeader(std::string header_key, std::string header_value);
		void clearHeader();

		void addContent(std::string content);
		void addContent(std::string content_key, std::string content_value);
		void clearContent();
		DWORD getStatusCode();
		DWORD getContentLength();

		// HTTP Requests
		bool request(HttpVerb::idx verb, std::string url);
		bool requestGET(std::string url);
		bool requestHEAD(std::string url);
		bool requestPOST(std::string url);
		bool requestPUT(std::string url);
		bool requestDELETE(std::string url);

		std::string & getContent();
	private:
		std::string m_app_name;
		std::vector<std::string> m_header_knv;
		std::string m_request_content;
		HttpClientHandle m_handle;
		DWORD m_status_code, m_content_length;
		std::string m_response_content;

		void cleanHandle();
	};

}	// end of namespace namo
#endif