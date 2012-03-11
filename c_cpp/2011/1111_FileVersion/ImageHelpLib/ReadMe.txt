[Shell.exe (32 bit)]
Is Valid DOS : 1
 Is Valid PE : 1
 Machine : 0x01c2 (IMAGE_FILE_MACHINE_THUMB)
 Optional Header Magic : 0x10b (IMAGE_NT_OPTIONAL_HDR32_MAGIC)
 Is Exist Resource Section : 1
  Is Exist Version Info : 1
   File version : 1.0.1.6


[BasicApp.exe (32 bit)]
Is Valid DOS : 1
 Is Valid PE : 1
 Machine : 0x014c (IMAGE_FILE_MACHINE_I386)
 Optional Header Magic : 0x10b (IMAGE_NT_OPTIONAL_HDR32_MAGIC)
 Is Exist Resource Section : 1
  Is Exist Version Info : 0


[nginx_svc.exe (64 bit)]
Is Valid DOS : 1
 Is Valid PE : 1
 Machine : 0x8664 (IMAGE_FILE_MACHINE_AMD64)
 Optional Header Magic : 0x20b (IMAGE_NT_OPTIONAL_HDR64_MAGIC)
 Is Exist Resource Section : 1
  Is Exist Version Info : 0


1. ImangeHelpLib 리팩토링
 Output 변수들을 typdedef struct 로
 main 함수에서 별도의 함수로 분리
	1. 수행
	2. 출력
 판단 함수 작성
	1. IsValidFile
 문자열 버전 만드는 함수
	1. DEFINE 크기
	2. DWORD x2 -> CHAR or WCHAR
2. CreateFile & CreateFileMapping 버전 프로젝트 생성