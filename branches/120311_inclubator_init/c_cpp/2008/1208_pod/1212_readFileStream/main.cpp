#include <iostream>
#include <tchar.h>
#include "PJLReader.h"
#include <locale>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	PJLReader reader;
	reader.open(_T("C:\\temp\\test.prn"));
	
	if(reader.isOpened()) {
		reader.fillPJLinfo();
		stringKV dictionary = reader.getKVMap();

		_COUT << _T("TIME : ") << dictionary[_T("TIME")] << endl;
		_COUT << _T("DATE : ") << dictionary[_T("DATE")] << endl;
		_COUT << _T("FRONTCOVERPRINT : ") << dictionary[_T("FRONTCOVERPRINT")] << endl;
		_COUT << _T("BACKCOVERPRINT : ") << dictionary[_T("BACKCOVERPRINT")] << endl;
		_COUT << _T("SLIPSHEETPRINT : ") << dictionary[_T("SLIPSHEETPRINT")] << endl;
		_COUT << _T("CHAPTERPRINT : ") << dictionary[_T("CHAPTERPRINT")] << endl;
		_COUT << _T("DATAMODE : ") << dictionary[_T("DATAMODE")] << endl;
		_COUT << _T("ORIENTATION : ") << dictionary[_T("ORIENTATION")] << endl;
		_COUT << _T("DUPLEX : ") << dictionary[_T("DUPLEX")] << endl;
		_COUT << _T("RESOLUTION : ") << dictionary[_T("RESOLUTION")] << endl;
		_COUT << _T("EDGETOEDGE : ") << dictionary[_T("EDGETOEDGE")] << endl;
		_COUT << _T("STAPLE : ") << dictionary[_T("STAPLE")] << endl;
		_COUT << _T("PUNCH : ") << dictionary[_T("PUNCH")] << endl;
		_COUT << _T("PAPER : ") << dictionary[_T("PAPER")] << endl;
		_COUT << _T("COPIES : ") << dictionary[_T("COPIES")] << endl;
		_COUT << _T("QTY : ") << dictionary[_T("QTY")] << endl;
		_COUT << _T("AUTOTRAYCHANGE : ") << dictionary[_T("AUTOTRAYCHANGE")] << endl;
		_COUT << _T("LANGUAGE : ") << dictionary[_T("LANGUAGE")] << endl;
	}

	return 0;
}
