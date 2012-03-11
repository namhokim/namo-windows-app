#include <stdio.h>

#include <Windows.h>
#include <tchar.h>

#include <Imagehlp.h>
#pragma comment(lib, "Imagehlp.lib")

//#include <Winver.h>
//#pragma comment(lib, "Version.lib")

DWORD GetExeDirectory(LPTSTR lpFilepath);
DWORD GetExeDirectoryA(LPSTR lpFilepath);
BOOL RecursiveResDir(LPBYTE pImgView, DWORD dwResImg, DWORD dwVirtualAddr,
					 DWORD dwOffset, INT nDepth, WORD wItemCnt, BOOL bVer,
					 LPDWORD dwFileVersionMS, LPDWORD dwFileVersionLS);

int main()
{
	/*TCHAR file[MAX_PATH];
	GetExeDirectory(file);
	lstrcat(file, _T("\\BasicApp.exe"));

	DWORD dwHeaderSum, dwCheckSum, dwRet;
	dwRet = MapFileAndCheckSum(file, &dwHeaderSum, &dwCheckSum);*/

	BOOL bValidDOS = FALSE;
	BOOL bValidPE = FALSE;
	WORD wMachine = IMAGE_FILE_MACHINE_UNKNOWN;
	WORD wOptMagic = 0;
	//WORD wMajorImageVersion = 0;
    //WORD wMinorImageVersion = 0;
	BOOL bExistRSRC = FALSE;
	//WORD wNumberOfIdEntries = 0;

	BOOL bExistVersion = FALSE;
	DWORD dwFileVersionMS = 0;
	DWORD dwFileVersionLS = 0;

	CHAR file2[MAX_PATH];
	LOADED_IMAGE li = {0};
	BOOL bRes;

	//GetExeDirectoryA(file2);
	//lstrcatA(file2, "\\BasicApp.exe");
	lstrcpyA(file2, "C:\\temp\\PE_Samples\\Shell.exe");

	bRes = MapAndLoad(file2, NULL, &li, FALSE, TRUE);
	if(bRes) {
		// IMAGE_DOS_HEADER
		PIMAGE_DOS_HEADER dos_header = (PIMAGE_DOS_HEADER)li.MappedAddress;
		//bValidDOS = li.fDOSImage;
		bValidDOS = (IMAGE_DOS_SIGNATURE == dos_header->e_magic);

		// IMAGE_NT_HEADERS
		bValidPE = (IMAGE_NT_SIGNATURE == li.FileHeader->Signature);

		// IMAGE_FILE_HEADER
		IMAGE_FILE_HEADER &file_header = li.FileHeader->FileHeader;
		wMachine = file_header.Machine;

		// IMAGE_OPTIONAL_HEADER
		IMAGE_OPTIONAL_HEADER &opt_header = li.FileHeader->OptionalHeader;
		wOptMagic = opt_header.Magic;
		//wMajorImageVersion = opt_header.MajorImageVersion;
		//wMinorImageVersion = opt_header.MinorImageVersion;

		// IMAGE_DATA_DIRECTORY array
		PIMAGE_DATA_DIRECTORY data_dir = opt_header.DataDirectory;

		// 리소스 섹션 헤더 획득
		PIMAGE_SECTION_HEADER pISH = NULL;
		for (WORD i=0; i<file_header.NumberOfSections; ++i) {
			PIMAGE_SECTION_HEADER pCurr = &li.Sections[i];

			if( RtlEqualMemory(pCurr->Name, ".rsrc\x0\x0\x0", IMAGE_SIZEOF_SHORT_NAME) ) {
				pISH = pCurr;
				bExistRSRC = TRUE;
				break;
			}
			
		}

		if(bExistRSRC) {
			// 리소스 디렉터리 획득
			PIMAGE_RESOURCE_DIRECTORY p_res_dir = (PIMAGE_RESOURCE_DIRECTORY)
				(li.MappedAddress + pISH->PointerToRawData);
			//wNumberOfIdEntries = p_res_dir->NumberOfIdEntries;

			// 리소스 디렉터리 엔트리 획득
			WORD wTotal = p_res_dir->NumberOfIdEntries + p_res_dir->NumberOfNamedEntries;
			PIMAGE_RESOURCE_DIRECTORY_ENTRY pEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)
				(p_res_dir + sizeof(IMAGE_RESOURCE_DIRECTORY));


			bExistVersion = RecursiveResDir(
				(LPBYTE)li.MappedAddress, pISH->PointerToRawData, pISH->VirtualAddress,
				(pISH->PointerToRawData + sizeof(IMAGE_RESOURCE_DIRECTORY)), 1,
				wTotal, FALSE, &dwFileVersionMS, &dwFileVersionLS);
		}

		// Deallocate all resources
		UnMapAndLoad(&li);
	}

	// OUTPUT
	printf("Is Valid DOS : %d\n", bValidDOS);
	if(bValidDOS) {
		printf(" Is Valid PE : %d\n", bValidPE);
		if(bValidPE) {
			printf(" Machine : 0x%04x\n", wMachine);

			// IMAGE_NT_OPTIONAL_HDR32_MAGIC (0x10b = 267), IMAGE_NT_OPTIONAL_HDR64_MAGIC (0x20b = 523)
			printf(" Optional Header Magic : 0x%x\n", wOptMagic);
			printf(" Is Exist Resource Section : %d\n", bExistRSRC);
			printf("  Is Exist Version Info : %d\n", bExistVersion);
			if(bExistVersion) {
				printf("   File version : %d.%d.%d.%d\n",
					HIWORD(dwFileVersionMS),LOWORD(dwFileVersionMS),
					HIWORD(dwFileVersionLS),LOWORD(dwFileVersionLS)
					);
			}
		}
	}

	return 0;
}

DWORD GetExeDirectory(LPTSTR lpFilepath)
{
    DWORD dwLength = GetModuleFileName(NULL, lpFilepath, MAX_PATH);
    if(0!=dwLength) {
        LPTCH string = lpFilepath + dwLength;
 
        while(--string != lpFilepath && *string != _T('\\'))    ;
 
        if (*string == _T('\\')) {
            *string = _T('\0');            // NULL 치환
 
            return (string-lpFilepath);    // 길이 반환
        }
    }
    return 0;
}

DWORD GetExeDirectoryA(LPSTR lpFilepath)
{
    DWORD dwLength = GetModuleFileNameA(NULL, lpFilepath, MAX_PATH);
    if(0!=dwLength) {
        LPCH string = lpFilepath + dwLength;
 
        while(--string != lpFilepath && *string != '\\')    ;
 
        if (*string == '\\') {
            *string = '\0';            // NULL 치환
 
            return (string-lpFilepath);    // 길이 반환
        }
    }
    return 0;
}

// http://mentohust.googlecode.com/svn-history/r94/trunk/src/mycheck.h
// http://sourceforge.net/mailarchive/forum.php?thread_name=E1MFPZ1-0002W7-D3%40bj8yhf1.ch3.sourceforge.com&forum_name=nsis-commits
// http://msdn.microsoft.com/en-us/library/ms647001(VS.85).aspx
typedef struct _VS_VERSIONINFO { 
	WORD	wLength;
	WORD	wValueLength;
	WORD	wType;
	WORD	szKey[16];
	WORD	Padding1[1];
	VS_FIXEDFILEINFO Value;
} VS_VERSIONINFO, *PVS_VERSIONINFO;
// http://msdn.microsoft.com/en-us/library/ms646997(VS.85).aspx

BOOL RecursiveResDir(LPBYTE pImgView, DWORD dwResImg, DWORD dwVirtualAddr,
					 DWORD dwOffset, INT nDepth, WORD wItemCnt, BOOL bVer,
					 LPDWORD dwFileVersionMS, LPDWORD dwFileVersionLS)
{
	LPBYTE pIter = (LPBYTE)(pImgView + dwOffset);

	// IMAGE_RESOURCE_DIRECTORY_ENTRY : dwOffset
	for(WORD wItem = 0; wItem < wItemCnt; ++wItem) {
		PIMAGE_RESOURCE_DIRECTORY_ENTRY pIRDE
			= (PIMAGE_RESOURCE_DIRECTORY_ENTRY)pIter;
		pIter += sizeof(IMAGE_RESOURCE_DIRECTORY_ENTRY);

		if(pIRDE->NameIsString) {
			PIMAGE_RESOURCE_DIR_STRING_U pIRDS
				= (PIMAGE_RESOURCE_DIR_STRING_U) (pImgView + dwResImg + pIRDE->NameOffset);

			LPSTR pszResName = new CHAR[pIRDS->Length + 1];
			int nConvLen = WideCharToMultiByte(CP_ACP, 0, pIRDS->NameString, pIRDS->Length,
				pszResName, pIRDS->Length + 1, NULL, NULL);
			pszResName[nConvLen] = 0x00;
			//printf("    ID: %d, NAME: %s\n", pIRDE->Id, pszResName);
			delete []pszResName;
		} else {
			if(RT_VERSION == MAKEINTRESOURCE(pIRDE->Id)) {
				bVer = TRUE;
			}
		}

		if(pIRDE->DataIsDirectory) {
			dwOffset = dwResImg + pIRDE->OffsetToDirectory;
			PIMAGE_RESOURCE_DIRECTORY pIRD
				= (PIMAGE_RESOURCE_DIRECTORY) (pImgView + dwOffset);
			WORD wItemCnt = pIRD->NumberOfIdEntries + pIRD->NumberOfNamedEntries;
			dwOffset += sizeof(IMAGE_RESOURCE_DIRECTORY);
			if(RecursiveResDir(pImgView, dwResImg, dwVirtualAddr, dwOffset, nDepth+1, wItemCnt, bVer,
				dwFileVersionMS, dwFileVersionLS)) {
				return TRUE;
			}
		} else {
			if(bVer) {
				dwOffset = dwResImg + pIRDE->OffsetToData;
				PIMAGE_RESOURCE_DATA_ENTRY pData =
					(PIMAGE_RESOURCE_DATA_ENTRY)(pImgView + dwOffset);

				PVS_VERSIONINFO pVerInfo = (PVS_VERSIONINFO)
					(pImgView + pData->OffsetToData - dwVirtualAddr + dwResImg);
				if(VS_FFI_SIGNATURE==pVerInfo->Value.dwSignature) {	// Magic Number
					VS_FIXEDFILEINFO*pVF = &pVerInfo->Value;
					/*printf("    File version : %d.%d.%d.%d\n",
						HIWORD(pVF->dwFileVersionMS),LOWORD(pVF->dwFileVersionMS),
						HIWORD(pVF->dwFileVersionLS),LOWORD(pVF->dwFileVersionLS)
						);*/
					*dwFileVersionMS = pVF->dwFileVersionMS;
					*dwFileVersionLS = pVF->dwFileVersionLS;
					return TRUE;
				}
			}
		}
	} /* for(WORD wItem = 0; wItem < wItemCnt; ++wItem) */

	return FALSE;
}