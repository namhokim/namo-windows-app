#include "ServiceControlManager.h"
#include <tchar.h>

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	ServiceControlManager scm;
	if(scm.open(TEXT("ntrtscan")) != true) cout << "Office Scan Service�� �����ϴ�.\n";
	else {
		if(scm.isServiceRunning()) {
			cout << "Office Scan Service�� �簡�� �մϴ�.\n";
			if(scm.stopService()) {
				cout << "������";
				while(!scm.isServiceStopped()) {
					Sleep(500);
					cout << ".";
				}
				if(scm.startService()) {
					cout << "\n������";
					while(!scm.isServiceRunning()) {
						Sleep(500);
						cout << ".";
					}
				}
				else cout << "���۽���\n";
			}
			else cout << "�簡������\n";
		}
		else cout << "���񽺰� �������� �� �������ּ���~\n";
	}
	cout << endl << endl;
	cout << "Build version 090106\n";
	cout << "Commemorative application\n";
	cout << "Made in Namo �� 2009\n";

	cout << "\n����Ϸ��� ���� Ű�� �����ʽÿ� . . .";
	char c;
	c= getchar();	// ȭ�鿡 ����
	return 0;
}
