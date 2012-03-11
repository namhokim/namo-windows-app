#pragma once

/* 

 �� ���̺귯����
 ���񽺸� ���� �� ������ �ϴ� ���� �Լ��� ������

 ������ : ���񽺸�(extern LPTSTR szSvcName in <ServiceMain.h>)

 �ۼ��� : Namho Kim
 �ۼ��� : 2011.4.29 15:05

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
