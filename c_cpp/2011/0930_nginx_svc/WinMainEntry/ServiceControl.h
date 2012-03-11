#pragma once

/* 

 이 라이브러리는
 서비스를 시작 및 정지를 하는 헬퍼 함수를 지원함

 의존성 : 서비스명(extern LPTSTR szSvcName in <ServiceMain.h>)

 작성자 : Namho Kim
 작성일 : 2011.4.29 15:05

 */


//
// Purpose: 
//   Starts the service if possible.
//
// Parameters:
//   None
// 
// Return value:
//   EXIT_SUCCESS(0) or EXIT_FAILURE(1) in <stdlib.h>
//
int __stdcall DoStartSvc(void);


//
// Purpose: 
//   Stops the service.
//
// Parameters:
//   None
// 
// Return value:
//   EXIT_SUCCESS(0) or EXIT_FAILURE(1) in <stdlib.h>
//
int  __stdcall DoStopSvc(void);
