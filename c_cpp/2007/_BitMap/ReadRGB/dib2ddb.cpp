#include <windows.h>

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx,by;
	BITMAP bit;

	MemDC=CreateCompatibleDC(hdc);
	OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit,sizeof(BITMAP),&bit);
	bx=bit.bmWidth;
	by=bit.bmHeight;

	BitBlt(hdc,0,0,bx,by,MemDC,0,0,SRCCOPY);

	SelectObject(MemDC,OldBitmap);
	DeleteDC(MemDC);
}

HBITMAP MakeDDBFromDIB(HDC hdc,char *Path)
{
	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER *fh;
	HBITMAP hBit;
	PVOID ih;
//	BITMAP bit;

	hFile=CreateFile(Path,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile==INVALID_HANDLE_VALUE) {
		return NULL;
	}

	FileSize=GetFileSize(hFile,NULL);

	fh=(BITMAPFILEHEADER *)malloc(FileSize);
	ReadFile(hFile,fh,FileSize,&dwRead,NULL);
	CloseHandle(hFile);

	ih=((PBYTE)fh+sizeof(BITMAPFILEHEADER));
	// DDB로 변환한다.
	hBit=CreateDIBitmap(hdc,(BITMAPINFOHEADER *)ih,CBM_INIT,
		(PBYTE)fh+fh->bfOffBits,(BITMAPINFO *)ih,DIB_RGB_COLORS);

	// DDB로 변환한 후 래스터 데이터는 따로 설정한다.
	//hBit=CreateDIBitmap(hdc,(BITMAPINFOHEADER *)ih,0,NULL,NULL,0);
	//GetObject(hBit,sizeof(BITMAP),&bit);
	//SetDIBits(NULL,hBit,0,bit.bmHeight,(PBYTE)fh+fh->bfOffBits,(BITMAPINFO *)ih,DIB_RGB_COLORS);
	free(fh);		// DIB buffer de-allocation
	return hBit;
}
