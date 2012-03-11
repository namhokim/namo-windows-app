#pragma once

/* 

 �� ���̺귯����
 ���񽺸� ��ġ �� ������ �ϴ� ���� �Լ��� ������

 ������ : ���񽺸�(extern LPTSTR szSvcName in <ServiceMain.h>)

 �ۼ��� : Namho Kim
 �ۼ��� : 2011.4.29 15:05

 */

//
// Purpose: 
//   Installs a service in the SCM database
//
// Parameters:
//   None
// 
// Return value:
//   EXIT_SUCCESS(0) or EXIT_FAILURE(1) in <stdlib.h>
//
int __stdcall DoServiceInstall(void);


//
// Purpose: 
//   Uninstalls a service from the SCM database
//
// Parameters:
//   None
// 
// Return value:
//   EXIT_SUCCESS(0) or EXIT_FAILURE(1) in <stdlib.h>
//
int __stdcall DoServiceUninstall(void);
