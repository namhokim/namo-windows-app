#include <string>
#include <iostream>
#include <fstream>
#include <set>

#include "CSV.h"

using namespace std;

void analysis1();
void analysis2();

// TODO : 성능 비교 - analysis1 vs analysis2
int main() {
	//analysis1();
	analysis2();	

	return 0;
}


void analysis1()
{
	unsigned long lc=0;
	string line;

	while (getline(cin, line)) {
		if(line.empty() || line[0]=='#') {	// 읽은 값이 없거나, 주석문이면
			continue;						// cut off
		}

		vector<string> rec;
		csvline_populate(rec, line, ' ');
		string ip(rec[2]);

		ofstream file(ip.append(".txt").c_str(), ios_base::out | ios_base::app | ios_base::ate);
		file << line << endl;
		file.close();

		lc++;
	}
}

void analysis2()
{
	string line, ip_before;
	ofstream file;

	while (getline(cin, line)) {
		if(line.empty() || line[0]=='#') {	// 읽은 값이 없거나, 주석문이면
			continue;						// cut off
		}

		// line 분리
		vector<string> rec;
		csvline_populate(rec, line, ' ');
		string ip(rec[2]);

		// 파일 IO
		ip.append(".txt");
		if(ip_before.empty() || ip != ip_before) {
			if(file.is_open()) file.close();

			file.open(ip.c_str(),
				ios_base::out | ios_base::app | ios_base::ate);
		}
		//ofstream file(ip.append(".txt").c_str(), ios_base::out | ios_base::app | ios_base::ate);
		file << line << endl;
		ip_before.assign(ip);
	}
}
