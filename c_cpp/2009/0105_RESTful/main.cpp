#include "HttpRequest.h"
#include <list>
using namespace std;

list<string> ParsingPoundSeperatedString(string httpReqStrings)
{
	list<string> resList;

	string::size_type firstPoundPosition = httpReqStrings.find_first_of("#");
	string::size_type lastPoundPosition = httpReqStrings.find_last_of("#");
	if(firstPoundPosition == lastPoundPosition
		&& firstPoundPosition == string::npos
		&& lastPoundPosition == string::npos) return resList;	// 빈 리스트 반환
	else {
		string fileList = httpReqStrings.substr(firstPoundPosition+1, lastPoundPosition-firstPoundPosition);
		while(!fileList.empty()) {
			string::size_type poundPosition = fileList.find("#");
			if(poundPosition == string::npos) break;
			string filename = fileList.substr(0, poundPosition);
			resList.push_back(filename);
			cout << filename << endl;
			fileList = fileList.substr(poundPosition + 1, fileList.size());
		}
	}

	return resList;
}
bool indexDownload()
{
	const string hostAddress("10.15.102.17");
	const unsigned short port = 8080;
	const string reqObject("/ScanBox/showListServlet?filename=/hdd/ts/sdk/dsdk/xlet/285475081/home/Scan_Box/신애");
	const string baseString("/ScanBox/down?filepath=/hdd/ts/sdk/dsdk/xlet/285475081/home/Scan_Box/신애/&imageName=");
	const string basePath("C:\\fileTest");

	HttpRequest hr;
	hr.open(hostAddress, port);
	string result = hr.requestText(reqObject, "POST");
	list<string> fileList = ParsingPoundSeperatedString(result);

	while(!fileList.empty()) {
		string currentFileName = fileList.front();
		fileList.pop_front();
		if(hr.requestSaveToFile(baseString + currentFileName, basePath + "\\"+ currentFileName) != true) return false;
	}
	return true;
}

void testIndexDownload()
{
	if(indexDownload()) cout << "success\n";
	else cout << "fail\n";
}

int main()
{
	testIndexDownload();
}