// ArrayBITMAP.h: interface for the CArrayBITMAP class.
//
// Class for storing any standart bitmaps in array
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARRAYBITMAP_H__0AB1F07C_1AD7_4848_AE0B_B7985BDC6423__INCLUDED_)
#define AFX_ARRAYBITMAP_H__0AB1F07C_1AD7_4848_AE0B_B7985BDC6423__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef ID_BMP_ARRAY_TIMEOUT
#define ID_BMP_ARRAY_TIMEOUT 10000L //10 sec timeout
#endif

class CArrayBITMAP  
{
public:
	int Add(LPBITMAP pBITMAP);					//add new bitmap
	int Add(CBitmap * pCBitmap);				//add new bitmap from object CBitmap
	int GetSize();								//get bitmaps count
	BOOL Replace(int iIndex, LPBITMAP pBITMAP); //replace bitmap by index
	BOOL Replace(int iIndex, CBitmap * pCBitmap); //replace with CBitmap
	BOOL Remove(int iIndex);					//remove bitmap by index
	void RemoveAll();							//remove all bitmas from array
	LPBITMAP GetAt(int iIndex);					//get ponter to bitmap
	CArrayBITMAP();								//constractor
	virtual ~CArrayBITMAP();					//destructor

private:
	DWORD GetBmpLenght(LPBITMAP pBITMAP);	//get bitmap buffer lenght
	BYTE * m_pBytes;						//pointer to bitmaps data
	BITMAP * m_pHeaders;	    //pointer to bitmaps headers
    DWORD * p_dwAddress;		//pointer to bitmap data adresses
	int	m_iCount;				//bitmaps count
	DWORD m_dwSize;				//buffer size
	HANDLE hMutex;				//synchronization object - threads usage support
};

#endif // !defined(AFX_ARRAYBITMAP_H__0AB1F07C_1AD7_4848_AE0B_B7985BDC6423__INCLUDED_)
