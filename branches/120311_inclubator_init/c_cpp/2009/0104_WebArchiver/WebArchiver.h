#pragma once
#include "../Util/StringUtil.h"
#include <map>
#include <list>

// resolveURIType
#define	URI_TYPE_HTTP		80
#define	URI_TYPE_FTP		21
#define	URI_TYPE_FILE		100
#define	URI_TYPE_FILEPATH	101
#define	URI_TYPE_UNKNOWN	65535

// uriDecoding
#define	STATE_NORMAL	0
#define	STATE_ESCAPE1	1
#define	STATE_ESCAPE2	2

typedef struct _URI
{
	std::string protocol;
	std::string host;
	std::string object;
} URI, *pURI;

enum CODE_PAGE_TYPE_
{
	UTF_8 = 0,
	EUC_KR = 1
};

class WebArchiver
{
public:
	WebArchiver(void);
	~WebArchiver(void);
	void setURIEncoding(int codeType)	{	m_uriEncodingType = codeType;	}
	std::string readString(std::string uri);
	std::string readHttpPartially(std::string uri, unsigned int limitByte);
	bool saveToFile(std::string uri, std::string filename);
	bool setStartPage(std::string uri)	{	return setListUpURI(uri);	}
	void download();
	bool isExistUriInMap(std::string uri);
private:
	// members
	std::string m_uri;
	int m_uriProtocolType, m_uriEncodingType, m_contentEncodingType;
	// tables
	std::map<std::string, bool> uriMap;
	std::list<std::string>		uriList;
	// methods
	int resolveURIType(std::string uri);
	std::string getHttpString(std::string uri);			// <- readString
	std::string getFileString(std::string filename);	// <- readString
	std::string resolveFileName(std::string uri);							// uri -> filename
	std::string uriDecoding(std::string uri, unsigned long code=EUC_KR);	// "%25" -> '%'
	bool isAbsoluteURI(std::string uri);
	bool getHostURI(std::string absoluteURI, pURI dividedURI);
	std::string toAbsoluteURI(std::string currentURI, std::string targetURI);
	bool setListUpURI(std::string uri);
	bool isHtmlURI(std::string uri);
	void processingHtml(std::string uri);
	std::string replaceLinkToAbsoluteAddress(std::string htmlContent);
	std::string replaceCorrectFileName(std::string object);

	// test code
#ifndef  NDEBUG
public:
	// public area
	void testReadString();
	void testReadHttpPartially();
	void testSaveToFile();
	// private area
	void testToAbsoluteURI();
	void testIsAbsoluteURI();
	void testURLStructure();
#endif	/* end of NDEBUG */
};
