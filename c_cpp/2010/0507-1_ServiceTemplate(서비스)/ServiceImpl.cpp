#include "ServiceImpl.h"

#define	USE_WINDOWS_API
#ifdef	USE_WINDOWS_API
#include <Windows.h>
#endif
///////////////////////////////////////////////////////////////////////////////
//
// Purpose: 
//   수행할 서비스 구현
//
// Parameters:
//   없음
// 
// Return value:
//   없음
//
void DoService()
{
	while(1) {
		WaitForSingleObject(ghSvcPauseEvent, INFINITE);	// 일시정지
		if(!WaitForSingleObject(ghSvcStopEvent, 0)) {	// 정지 시그널시
			break;										// 루프 탈출
		}
	}
//
//#ifdef	USE_WINDOWS_API
//	WaitForSingleObject(ghSvcStopEvent, INFINITE);	// 스레드 양보를 하는 착한 루프
//#else
//	while(!isStopSignaled()) {	// CPU 100% 사용률을 보여주는 스핀 루프
//		;
//	}
//#endif
}



/*
Copyright (c) 2010  Namho Kim

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */
