#include "WebArchiver.h"
#include <iostream>
using namespace std;

void testCodes()
{
	WebArchiver wa;
	//wa.testReadString();
	wa.testReadHttpPartially();
	//wa.testSaveToFile();
	//wa.testToAbsoluteURI();
	//wa.testIsAbsoluteURI();
	//wa.testURLStructure();
}

void testWebArchiver()
{
	WebArchiver wa;

//	wa.setStartPage("http://www.google.co.kr/index.html");
//	wa.setStartPage("http://www.google.co.kr/search?complete=1&hl=ko&q=gif&lr=");
//	wa.setStartPage("http://i.kdaq.empas.com/imgs/qsi.tsp/5886657/0/1/A/Å°¹«¶ó.bmp");
	wa.setStartPage("http://namoda.springnote.com/pages/563877/attachments/300678");

	wa.download();

}
int main()
{
	//testCodes();
	testWebArchiver();	
}
