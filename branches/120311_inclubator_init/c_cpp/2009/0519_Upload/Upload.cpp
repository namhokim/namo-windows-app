// Upload.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "RyeolHttpClient.h"

void TestUpload()
{
	using namespace Ryeol ;

	CHttpClient         objHttpReq ;
	CHttpResponse *     pobjHttpRes = NULL ;

	try {
		// Initialize the User Agent
		objHttpReq.SetInternet (_T ("My User Agent v1.0")) ;

		// Add user's custom HTTP headers
		objHttpReq.AddHeader (_T ("Ryeol-Magic"), _T ("My Magic Header")) ;
		objHttpReq.AddHeader (_T ("User-Magic"), _T ("User's Magic Header")) ;

		// Add user's parameters
		objHttpReq.AddParam (_T ("sid"), _T ("00-1D-72-90-C3-4B")) ;
		objHttpReq.AddParam (_T ("dev"),
			_T ("00-00-74-6f-06-cB;00-00-74-ca-67-3B;00-00-74-d9-89-5B;00-00-74-c1-a7-fB"));

		// Specifies a file to upload
		objHttpReq.AddParam (_T ("mdb"), 
			_T ("wisdomLT.mdb"), 
			CHttpClient::ParamFile) ;
		objHttpReq.AddParam (_T ("ini"), 
			_T ("config.ini"), 
			CHttpClient::ParamFile) ;

		// Start a new request
		objHttpReq.BeginUpload (_T ("http://10.15.28.166:8080")
			_T ("/wisdomlt/reqbak.jsp")) ;

		// Specifies the number of bytes to send when the Proceed method is called.
		const DWORD     cbProceed = 1024 ;  // 1K

		do {

			;//...     // Place codes to report progress information to user.
		} while ( !(pobjHttpRes = objHttpReq.Proceed (cbProceed)) ) ;
		//...     // Place codes to handle the returned CHttpResponse object.
	} catch (httpclientexception & e) {
		printf("%d\n", e.Win32LastError());
		//...     // Place exception handling codes here.
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestUpload();
	return 0;
}

