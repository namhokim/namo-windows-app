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


1. ImangeHelpLib �����丵
 Output �������� typdedef struct ��
 main �Լ����� ������ �Լ��� �и�
	1. ����
	2. ���
 �Ǵ� �Լ� �ۼ�
	1. IsValidFile
 ���ڿ� ���� ����� �Լ�
	1. DEFINE ũ��
	2. DWORD x2 -> CHAR or WCHAR
2. CreateFile & CreateFileMapping ���� ������Ʈ ����