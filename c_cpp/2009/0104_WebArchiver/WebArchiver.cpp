#include "WebArchiver.h"
#include "../HttpRequest/HttpRequest.h"	// for URI_TYPE_HTTP case
#include "../FileManager/FileManager.h"	// for URI_TYPE_FILE, URI_TYPE_FILEPATH case
#include "../Util/UTF8Conv.h"		// for uriDecoding method

using namespace std;

WebArchiver::WebArchiver(void)
: m_uriProtocolType(URI_TYPE_UNKNOWN), m_uriEncodingType(UTF_8), m_contentEncodingType(UTF_8)
{
}

WebArchiver::~WebArchiver(void)
{
}

string WebArchiver::readString(string uri)
{
	string retString("");
	m_uri = uri;
	m_uriProtocolType = resolveURIType(uri);
	switch(m_uriProtocolType) {
		case URI_TYPE_FILE:
			retString = getFileString(resolveFileName(uri));
			break;
		case URI_TYPE_FILEPATH:
			retString = getFileString(uri);
			break;
		case URI_TYPE_HTTP:
			retString = getHttpString(uri);
			break;
		default:
		case URI_TYPE_UNKNOWN:
			break;
	}
	return retString;
}

// readString -> resolveURIType
int WebArchiver::resolveURIType(string uri)
{
	std::allocator<string>::size_type pos = uri.find("://");
	if(pos != string::npos) {
		string typeString = uri.substr(0, pos);
		if(typeString.compare("http") == 0) return URI_TYPE_HTTP;		// http://으로 시작
		else if(typeString.compare("ftp") == 0) return URI_TYPE_FTP;	// ftp://으로 시작
		else if(typeString.compare("file") == 0) return URI_TYPE_FILE;	// file:///으로 시작
	}
	else {
		pos = uri.find(":\\");
		if(pos != string::npos) return URI_TYPE_FILEPATH;					// ?:\으로 시작(?는 C,D,E 등 드라이브이름일 가능성)
	}
	return URI_TYPE_UNKNOWN;
}

// readString -> getHttpString
string WebArchiver::getHttpString(string uri)
{
	if(uri.empty()) return "There was no URI.";
	else
	{
		URI dividedURI;
		if(getHostURI(uri, &dividedURI) != true) return "";

		HttpRequest httpReq;
		httpReq.open(dividedURI.host);
		return httpReq.requestText(dividedURI.object);
	}
}

string WebArchiver::readHttpPartially(string uri, unsigned int limitByte)
{
	URI dividedURI;
	if(getHostURI(uri, &dividedURI) != true) return "";

	HttpRequest httpReq;
	httpReq.open(dividedURI.host);
	return httpReq.requestText(dividedURI.object, "GET", true, limitByte);
}

// readString -> resolveFileName -> resolveFileName
// readString ->									-> getFileString
string WebArchiver::getFileString(string filename)
{
	if(filename.empty()) return "There was no filename.";
	else 
	{
		FileManager fm;
		return fm.readFileA(filename);
	}
}

std::string WebArchiver::resolveFileName(string uri)
{
	if(uri.empty()) return "";
	else {
		string removedProtocolName = uri.substr(8, uri.size());
		char buffer[1024];
		strcpy_s(buffer, 1024, removedProtocolName.c_str());
		char *currPtr = buffer;
		while(*currPtr != '\0') {
			if(*currPtr == '/') *currPtr = '\\';
			currPtr++;
		}
		return uriDecoding(buffer, m_uriEncodingType);	// URI decoding
	}
}

std::string WebArchiver::uriDecoding(string uri, unsigned long code)
{
	const int MaxURISize = 1024, CodeSize = 2 + 1, NullString = 0x00;
	// variation
	char inputByte[MaxURISize], outputByte[MaxURISize], tmp_Code[CodeSize], *currPtr;
	int currentState, writeIndex;

	// initialize
	strcpy_s(inputByte, MaxURISize, uri.c_str());
	currPtr = inputByte;
	currentState = STATE_NORMAL;
	memset(tmp_Code, NullString, CodeSize);
	writeIndex = 0;

	// preprocessing
	while(*currPtr != '\0') {
		if(currentState == STATE_NORMAL) {
			if(*currPtr == '%') currentState = STATE_ESCAPE1;	// %AB에서 %로 진입
			else outputByte[writeIndex++] = *currPtr;
		}
		else if(currentState == STATE_ESCAPE1) {	// %AB에서 A를 버퍼에 담음
			tmp_Code[0] = (char)*currPtr;
			currentState = STATE_ESCAPE2;
		}
		else if(currentState == STATE_ESCAPE2) {	// %AB에서 B를 버퍼에 담고 코드로 처리
			tmp_Code[1] = (char)*currPtr;

			long charCode = strtol(tmp_Code, NULL, 16);
			outputByte[writeIndex++] = (char)charCode;
			currentState = STATE_NORMAL;
		}
		currPtr++;
	}
	outputByte[writeIndex] = (char)NullString;
	
	// 코드 처리(기본값: UTF-8)
	UTF8_CONVERSION;
	switch(code) {
		case UTF_8:
			return UTF82A((char *)outputByte);
		default:
		case EUC_KR:
			return (char *)outputByte;
	}
}

bool WebArchiver::saveToFile(std::string uri, std::string filename)
{
	m_uriProtocolType = resolveURIType(uri);
	if(m_uriProtocolType != URI_TYPE_HTTP) return false;
	else
	{
		URI dividedURI;
		if(getHostURI(uri, &dividedURI) != true) return false;

		HttpRequest httpReq;
		httpReq.open(dividedURI.host);
		return httpReq.requestSaveToFile(dividedURI.object, filename);
	}
}

void WebArchiver::download()
{
	while(!uriList.empty()) {
		string currentProcessingURI = uriList.front();
		// processing
		processingHtml(currentProcessingURI);
		uriList.pop_front();
	}
}

void WebArchiver::processingHtml(string uri)
{
	bool isNeedToProcessing = uriMap[uri];
	if(isNeedToProcessing) {
		string htmlContent = getHttpString(uri);
		// 링크를 절대주소로 변환하면서 바이너리는 등재..
	}
	else {	// 처리 필요 없음(gif, jpg 등 바이너리 및 비html)
		string filename = uri.substr(uri.find_last_of("/")+1, uri.size());
		saveToFile(uri, replaceCorrectFileName(filename));
	}
}

string replaceLinkToAbsoluteAddress(string htmlContent)
{
	string returnHtml;
	return htmlContent;
}
string WebArchiver::replaceCorrectFileName(string object)	// 파일이름에 %AB 포함시 처리?
{
	if(object.empty()) return "index";
	else {
		char filename[MAX_PATH];
		char newFilename[MAX_PATH];
		char *currentPosition = filename;
		strcpy_s(filename, MAX_PATH, object.c_str());

		unsigned int i = 0;
		while(*currentPosition != '\0') {
			if(*currentPosition == '\\'
				|| *currentPosition == '/'
				|| *currentPosition == ':'
				|| *currentPosition == '*'
				|| *currentPosition == '?'
				|| *currentPosition == '\"'
				|| *currentPosition == '<'
				|| *currentPosition == '>'
				|| *currentPosition == '|') newFilename[i] = '_';
			else newFilename[i] = *currentPosition;
			currentPosition++;
			i++;
		}
		newFilename[i] = '\0';
		return newFilename;
	}
}

bool WebArchiver::isAbsoluteURI(string uri)
{
	std::allocator<string>::size_type pos = uri.find("://");
	if(pos != string::npos) return true;
	else return false;
}

bool WebArchiver::getHostURI(string absoluteURI, pURI dividedURI)
{
	if(isAbsoluteURI(absoluteURI) != true) return false;
	else {
		dividedURI->protocol = absoluteURI.substr(0, absoluteURI.find("://")+3);

		string removedProtocolName = absoluteURI.substr(absoluteURI.find("://")+3, absoluteURI.size());
		if(removedProtocolName.find("/") == string::npos) removedProtocolName.append("/");	// domain 주소만 넘어왔을 때
		dividedURI->host = removedProtocolName.substr(0, removedProtocolName.find("/"));
		dividedURI->object = removedProtocolName.substr(removedProtocolName.find("/"), removedProtocolName.size());
		
		return true;
	}
}

string WebArchiver::toAbsoluteURI(string currentURI, string targetURI)
{
	if(isAbsoluteURI(targetURI) == true) return targetURI;
	else if(isAbsoluteURI(currentURI) != true) return "";
	else {
		URI uri;
		string resURI("");
		if(getHostURI(currentURI, &uri) != true) return "";
		else {
			resURI = uri.protocol;			// http://
			resURI.append(uri.host);		// http://hostname.com
		}

		if(targetURI.find("/") == string::npos) {	// '/'가 비포함
			resURI.append(uri.object.substr(0, uri.object.find_last_of("/")+1));
			resURI.append(targetURI);
			return resURI;
		}
		else {
			if(targetURI.c_str()[0] == '/') {		// '/'로 시작하면
				resURI.append(targetURI);			// currentURI(host) <==[몽땅]== targetURI
				return resURI;
			}
			else if(targetURI.size()>=2				// './'로 시작
				&& targetURI.c_str()[0] == '.' && targetURI.c_str()[1] == '/') {
				return toAbsoluteURI(currentURI, targetURI.substr(2, targetURI.size()));
			}
			else if(targetURI.size()>=3 &&			// '../'로 시작
				targetURI.c_str()[0] == '.' && targetURI.c_str()[1] == '.' && targetURI.c_str()[2] == '/') {
				string newCurrentURI = currentURI.substr(0, currentURI.find_last_of("/"));
				return toAbsoluteURI(newCurrentURI, targetURI.substr(3, targetURI.size()));
			}
			else {									// '~~/~~/~~' 형태
				string newCurrentURI = resURI;
				newCurrentURI.append(uri.object.substr(0, uri.object.find_last_of("/")+1));
				newCurrentURI.append(targetURI.substr(0, targetURI.find("/")+1));			// currentURI <==[~~/]== targetURI
				return toAbsoluteURI(newCurrentURI, targetURI.substr(targetURI.find("/")+1, targetURI.size()));
			}
		}
	}
}

bool WebArchiver::setListUpURI(string uri)
{
	if(isExistUriInMap(uri) != true) {
		// 해당하는 uri가 html라면 true를 할당
		// 그외에는 false할당
		uriMap[uri] = isHtmlURI(uri);
		uriList.push_back(uri);
		return true;
	}
	else return false;
}

bool WebArchiver::isHtmlURI(string uri)	// 검증되지 않음(판단 기준)
{
	string tmpHttpReq = readHttpPartially(uri, 20);
	if(tmpHttpReq.find("html") != string::npos
		&& tmpHttpReq.find("<") != string::npos
		&& tmpHttpReq.find(">") != string::npos) return true;
	else return false;
}

bool WebArchiver::isExistUriInMap(string uri)
{
	return (uriMap.find(uri) != uriMap.end());
}

// test code
#ifndef  NDEBUG
#include <list>
void WebArchiver::testReadString()
{
	setURIEncoding(UTF_8);
	//cout << readString("C:\\Documents and Settings\\namo\\바탕 화면\\1226_개인프로젝트\\1228_WebCrawler\\MarkupLanguageParser\\GoogleKr.htm");
	//cout << readString("file:///C:/Documents and Settings/namo/바탕 화면/1226_개인프로젝트/1228_WebCrawler/MarkupLanguageParser/GoogleKr.htm");
	//cout << readString("file:///C:/Documents%20and%20Settings/namo/%EB%B0%94%ED%83%95%20%ED%99%94%EB%A9%B4/1226_%EA%B0%9C%EC%9D%B8%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8/1228_WebCrawler/MarkupLanguageParser/ssss%25/sss/GoogleKr.htm");
	//cout << readString("file:///C:/Documents%20and%20Settings/namo/%EB%B0%94%ED%83%95%20%ED%99%94%EB%A9%B4/1226_%EA%B0%9C%EC%9D%B8%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8/1228_WebCrawler/MarkupLanguageParser/GoogleKr.htm");
	cout << readString("http://www.google.co.kr/search?q=string+C%2B%2B+replace&ie=utf-8&oe=utf-8&aq=t&rls=org.mozilla:ko:official&client=firefox-a");
}

void WebArchiver::testReadHttpPartially()
{
	setURIEncoding(UTF_8);
	cout << readHttpPartially("http://i.kdaq.empas.com/imgs/qsi.tsp/5886657/0/1/A/키무라.bmp",10);
}

void WebArchiver::testSaveToFile()
{
	if(saveToFile("http://old.chinasisa.com/comu/comu_main.asp", "comu_main.html")) {
		cout << "written\n";
	}
	else cout << "failed\n";
}

void WebArchiver::testToAbsoluteURI()
{
	string currentURI = "http://old.chinasisa.com/comu/comu_main.asp";
	list<string> targetURI;

	// input
	targetURI.push_back("http://old.chinasisa.com/comu/comu_main.asp");
	targetURI.push_back("/img/com_main_img.jpg");
	targetURI.push_back("img/memo_go.gif");
	targetURI.push_back("././././comu_main.asp");
	targetURI.push_back("../css/../css/style.css");
	targetURI.push_back(".../css/style.css");
	targetURI.push_back("./css/./../../style.css");


	// print
	cout << "[testToAbsoluteURI()]\n";
	cout << "current URI : " << currentURI << endl << endl;
	while(!targetURI.empty()) {
		string oneURI = targetURI.front();
		cout << oneURI << "\n => " << toAbsoluteURI(currentURI, oneURI) << endl;
		targetURI.pop_front();
	}
}

void WebArchiver::testIsAbsoluteURI()
{
	list<string> testURI;

	// input
	testURI.push_back("http://www.japansisa.com/pop_up/comupop.htm");
	testURI.push_back("../comupop.htm");

	// print
	cout << "[testIsAbsoluteURI()]\n";
	while(!testURI.empty()) {
		string oneURI = testURI.front();
		if(isAbsoluteURI(oneURI)) cout << oneURI << " => absolute address\n";
		else cout << oneURI << " => not absolute address\n";
		testURI.pop_front();
	}
}

void WebArchiver::testURLStructure()
{
	URI uri;
	string targetURI ="http://www.japansisa.com/pop_up/comupop.htm";

	// print
	cout << "[testURLStructure()]\n";
	if(getHostURI(targetURI, &uri)) {
		cout << targetURI << "\n=>\n";
		cout << uri.protocol << endl;
		cout << uri.host << endl;
		cout << uri.object << endl;
	}
	else cout << "failed" << endl;
}
#endif