#include <Windows.h>
#include <string>
#include <map>
#include <fstream>
#include <iostream>

using namespace std;

wchar_t* GetDBFilename(wchar_t* filename);
void DivideMacAndDesc(const string&line, string& mac, string&desc);
void GetMacFromArpTable(const string&line, string& mac);

int main()
{
	// map
	map<string, string> mac_map;

	// DB in memory
	wchar_t fn[MAX_PATH] = {0};
	GetDBFilename(fn);

	std::ifstream file(fn);
	if(file.fail()) {
		cerr << "에러: 파일을 찾을 수 없습니다." << fn << std::endl;
		return 1;
	}

	string buf;
	while( getline(file, buf) ) {
		string mac, desc;
		DivideMacAndDesc(buf, mac, desc);
		mac_map.insert(pair<string, string>(mac, desc));
	}

	while( getline(cin, buf) ) {
		string mac;
		GetMacFromArpTable(buf, mac);

		map<string, string>::iterator pos = mac_map.find(mac);
		if(mac_map.end()!=pos) {
			cout << buf.substr(0, 46) << pos->second << endl;
		} else {
			cout << buf << endl;
		}
	}

	return 0;
}

wchar_t* GetDBFilename(wchar_t* filename)
{
	DWORD dwLength = GetModuleFileNameW(NULL, filename, MAX_PATH);
	if(0!=dwLength) {
		LPWCH string = filename + dwLength;

		while(--string != filename && *string != L'\\')    ;

		if (*string == L'\\') {
			*string = L'\0';            // NULL 치환
			wcscat_s(filename, MAX_PATH, L"\\mac_db.txt");
		}
	}

	return filename;
}

void DivideMacAndDesc(const string&line, string& mac, string&desc)
{
	mac = line.substr(0, 17);
	desc = line.substr(18);
	//cout << mac << endl;
	//cout << desc << endl;
}

void GetMacFromArpTable(const string&line, string& mac)
{
	try {
		mac = line.substr(24, 17);
	} catch( std::out_of_range& ex ) {
		mac = ex.what();
	}
	//cout << mac << endl;
}