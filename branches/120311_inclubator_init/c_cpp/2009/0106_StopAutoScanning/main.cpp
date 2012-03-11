#include "ServiceControlManager.h"
#include <tchar.h>

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	ServiceControlManager scm;
	if(scm.open(TEXT("ntrtscan")) != true) cout << "Office Scan Service가 없습니다.\n";
	else {
		if(scm.isServiceRunning()) {
			cout << "Office Scan Service를 재가동 합니다.\n";
			if(scm.stopService()) {
				cout << "중지중";
				while(!scm.isServiceStopped()) {
					Sleep(500);
					cout << ".";
				}
				if(scm.startService()) {
					cout << "\n시작중";
					while(!scm.isServiceRunning()) {
						Sleep(500);
						cout << ".";
					}
				}
				else cout << "시작실패\n";
			}
			else cout << "재가동실패\n";
		}
		else cout << "서비스가 가동중일 때 실행해주세요~\n";
	}
	cout << endl << endl;
	cout << "Build version 090106\n";
	cout << "Commemorative application\n";
	cout << "Made in Namo ⓒ 2009\n";

	cout << "\n계속하려면 엔터 키나 누르십시오 . . .";
	char c;
	c= getchar();	// 화면에 보임
	return 0;
}
