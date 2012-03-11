// ArrayBITMAP.cpp: implementation of the CArrayBITMAP class.
// Class for storing any standart bitmaps in array
// autor:			Alex Belousov 
// creation data:	26.09.2002
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArrayBITMAP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CArrayBITMAP::CArrayBITMAP()
{
	m_pBytes = NULL;	//pointer to bitmaps data
	m_pHeaders = NULL;	//pointer to bitmaps headers
	p_dwAddress = NULL; //adresses
	m_dwSize = 0;		//buffers size
	m_iCount = 0;		//elements count

	// Thread support
	// Create a mutex with no initial owner.
	hMutex = CreateMutex(NULL, FALSE, "MutexToProtectBitmapArray"); 

}
CArrayBITMAP::~CArrayBITMAP()
{
	if(m_pBytes != NULL)
		delete m_pBytes;
	if(m_pHeaders != NULL)
		delete m_pHeaders;
	if(p_dwAddress != NULL)
		delete p_dwAddress;
	if(hMutex != NULL)
		CloseHandle(hMutex);
}
//Function: add new bitmap
//Parameter: pointer to CBitmap object
//Return: index of new element( -1 if error )
int CArrayBITMAP::Add(CBitmap * pCBitmap)
{
    BITMAP bmp; 
		::GetObject((HBITMAP)pCBitmap->operator HBITMAP(), sizeof(BITMAP), (LPSTR)&bmp);
	DWORD dwDim = GetBmpLenght(&bmp);//bitmap dimention
		if(dwDim == 0)
			return -1;//bitmap structure error
	BYTE * pBuf = (BYTE *) malloc(sizeof(BYTE) * dwDim);
		if(pBuf == NULL)
			return -1;//memory error
		::GetBitmapBits(pCBitmap->operator HBITMAP(), dwDim, pBuf);
		//add buffer pointer
		bmp.bmBits = pBuf;
		//add bitmap to array
	int iRet = Add(&bmp);
		//clear
		delete pBuf;

	return iRet;
}
//Function: add new bitmap
//Parameter: pointer to BITMAP Structure
//Return: index of new element( -1 if error )
int CArrayBITMAP::Add(LPBITMAP pBITMAP)
{
	if(pBITMAP->bmBits == NULL)
		return -1;//error of bitmap data buffer
	
	DWORD dwDim = GetBmpLenght(pBITMAP);
		if(dwDim == 0)
			return -1;//error size of bitmap

	DWORD dwWaitResult; 
		// Request ownership of mutex.
	    dwWaitResult = WaitForSingleObject( hMutex, 10000L);// 10 second time-out
 		if(dwWaitResult != WAIT_OBJECT_0) 
            return -1; 
	__try 
	{ 
		//Allocate BITMAP data buffer
		if(m_pBytes == NULL)
			m_pBytes = (BYTE *)malloc(sizeof(BYTE) * dwDim);
		else
			m_pBytes = (BYTE *)realloc(m_pBytes, sizeof(BYTE) * m_dwSize + sizeof(BYTE) * dwDim);
		
		if(m_pBytes == NULL)
		{
			ReleaseMutex(hMutex);
			return -1;	//error: is not enough available memory
		}
		//resize BITMAP headers array
		if(m_pHeaders == NULL)
			m_pHeaders = (BITMAP *)malloc(sizeof(BITMAP));
		else
			m_pHeaders = (BITMAP *)realloc(m_pHeaders, sizeof(BITMAP) * (m_iCount + 1));

		if(m_pHeaders == NULL)
		{
			ReleaseMutex(hMutex);
			return -1;	//error: is not enough available memory
		}

		//resize addresses array
		if(p_dwAddress == NULL)
			p_dwAddress = (DWORD *)malloc(sizeof(DWORD));
		else
			p_dwAddress = (DWORD *)realloc(p_dwAddress, sizeof(DWORD) * (m_iCount + 1));

		if(p_dwAddress == NULL)
		{
			ReleaseMutex(hMutex);
			return -1;	//error: is not enough available memory
		}

		//copy new BITMAP data
		memcpy((BYTE *)m_pBytes+m_dwSize, (BYTE *)pBITMAP->bmBits, sizeof(BYTE) * dwDim);
		memcpy((BITMAP *)&m_pHeaders[m_iCount], (BITMAP *)pBITMAP, sizeof(BITMAP));

		m_pHeaders[m_iCount].bmBits = (BYTE *)&m_pBytes[m_dwSize];
		p_dwAddress[m_iCount] = m_dwSize;

		//add bitmaps count
		m_iCount++;

		//update buffer lenght
		m_dwSize += dwDim;
	}
	__finally 
	{ 
		// Release ownership of the mutex object.
        if (! ReleaseMutex(hMutex)) 
			return -1;
                    
	} 

	return m_iCount-1; //current index
}
//Function: get bitmap from array by index
//Parameter: BITMAP index
//Return: pointer to BITMAP
LPBITMAP CArrayBITMAP::GetAt(int iIndex)
{
	if(iIndex >= m_iCount)
		return NULL;

	if(WaitForSingleObject( hMutex, ID_BMP_ARRAY_TIMEOUT)!= WAIT_OBJECT_0) // 10 second time-out
       return NULL; 
	__try 
	{ 
		m_pHeaders[iIndex].bmBits = (BYTE *)(m_pBytes+p_dwAddress[iIndex]);
	}
	__finally 
	{ 
		// Release ownership of the mutex object.
        if (! ReleaseMutex(hMutex)) 
			return NULL;
                    
	} 
	return &m_pHeaders[iIndex];
}
//Function: replace bitmap in array
//Parameters: BITMAP index, and pointer to new BITMAP
//Return: TRUE or FALSE
BOOL CArrayBITMAP::Replace(int iIndex, CBitmap * pCBitmap)
{
    BITMAP bmp; 
		::GetObject((HBITMAP)pCBitmap->operator HBITMAP(), sizeof(BITMAP), (LPSTR)&bmp);
	DWORD dwDim = GetBmpLenght(&bmp);//bitmap dimention
		if(dwDim == 0)
			return -1;//bitmap structure error
	BYTE * pBuf = (BYTE *) malloc(sizeof(BYTE) * dwDim);
		if(pBuf == NULL)
			return -1;//memory error
		::GetBitmapBits(pCBitmap->operator HBITMAP(), dwDim, pBuf);
		//add buffer pointer
		bmp.bmBits = pBuf;
		//add bitmap to array
	BOOL bRet = Replace(iIndex, &bmp);
		//clear
		delete pBuf;
	return bRet;
}
//Function: replace bitmap in array
//Parameters: BITMAP index, and pointer to new BITMAP
//Return: TRUE or FALSE
BOOL CArrayBITMAP::Replace(int iIndex, LPBITMAP pBITMAP)
{
	//check index
	if(iIndex >= m_iCount || iIndex < 0)
		return FALSE;
	if(GetAt(iIndex) == NULL)
		return FALSE;

	// Request ownership of mutex.
	if(WaitForSingleObject( hMutex, ID_BMP_ARRAY_TIMEOUT)!= WAIT_OBJECT_0) // 10 second time-out
        return FALSE; 
	__try 
	{ 
	//length of new bitmap
	DWORD dwNewBitmapLength = GetBmpLenght(pBITMAP);
		if(dwNewBitmapLength == 0)
			return FALSE;//error of bitmap size

	//Shift in data buffer
	long ll_ShiftAddress = 0;
		
		//1.First Place Image and Middle Place Image
		if(iIndex < m_iCount - 1)
		{
				ll_ShiftAddress = dwNewBitmapLength - (p_dwAddress[iIndex + 1] - p_dwAddress[iIndex]);
				//copy last part of buffer to temp
			BYTE * m_BufLastPart = (BYTE *)malloc(sizeof(BYTE) * (m_dwSize - p_dwAddress[iIndex + 1]));
					
				if(m_BufLastPart == NULL)
				{
					ReleaseMutex(hMutex);
					return FALSE; //error: is not enough available memory
				}

				memcpy((BYTE *)m_BufLastPart,(BYTE *) m_pBytes+p_dwAddress[iIndex+1],
										sizeof(BYTE)*(m_dwSize - p_dwAddress[iIndex + 1]));
				//change buffer length
				m_pBytes = (BYTE *)realloc((BYTE *)m_pBytes, sizeof(BYTE)*(p_dwAddress[iIndex] + dwNewBitmapLength +
									(m_dwSize - p_dwAddress[iIndex + 1]) ));
				if(m_pBytes == NULL)
				{
					delete m_BufLastPart;
					ReleaseMutex(hMutex);
					return FALSE; //error: is not enough available memory
				}
				//copy new bitmap data	
				memcpy((BYTE *)m_pBytes + p_dwAddress[iIndex], (BYTE *)pBITMAP->bmBits, 
														sizeof(BYTE)*dwNewBitmapLength);
				//copy last part from temp
				memcpy((BYTE *)m_pBytes + p_dwAddress[iIndex] + dwNewBitmapLength, 
					(BYTE *)m_BufLastPart, sizeof(BYTE)*(m_dwSize - p_dwAddress[iIndex + 1]));

				delete m_BufLastPart;
			
		}
		else
		//2.Last Place Image
		{	
			//calculate shift of buffer length
			ll_ShiftAddress = dwNewBitmapLength - (m_dwSize - p_dwAddress[iIndex]);
			//change buffer length
			m_pBytes = (BYTE *)realloc((BYTE *)m_pBytes, sizeof(BYTE) * 
												(p_dwAddress[iIndex] + dwNewBitmapLength));
			if(m_pBytes == NULL)
			{
				ReleaseMutex(hMutex);
				return FALSE; //error: is not enough available memory
			}

			//copy new bitmap data
			memcpy((BYTE *)m_pBytes + p_dwAddress[iIndex], (BYTE *)pBITMAP->bmBits, 
														sizeof(BYTE)*dwNewBitmapLength);

		}
		//copy header data
		memcpy((BITMAP *)&m_pHeaders[iIndex], (BITMAP *)pBITMAP, sizeof(BITMAP));
		
		//replace addresses and pointers
		for(int i=iIndex+1; i<m_iCount; i++)
		{
			p_dwAddress[i] += ll_ShiftAddress;
			m_pHeaders[i].bmBits = (void *)&m_pBytes[p_dwAddress[i]];
		}
		
		//update buffer length variable
		m_dwSize += ll_ShiftAddress;
	}
	__finally 
	{ 
		// Release ownership of the mutex object.
        if (! ReleaseMutex(hMutex)) 
			return FALSE;
                    
	} 

	return TRUE;
}
//Function: remove bitmap from array
//Parameters: BITMAP index
//Return: TRUE or FALSE
BOOL CArrayBITMAP::Remove(int iIndex)
{
	//check index
	if(iIndex >= m_iCount || iIndex < 0)
		return FALSE;
	if(GetAt(iIndex) == NULL)
		return FALSE;
	// Request ownership of mutex.
	if(WaitForSingleObject( hMutex, ID_BMP_ARRAY_TIMEOUT)!= WAIT_OBJECT_0) // 10 second time-out
        return FALSE; 
	__try 
	{ 
	//length of removing bitmap
	DWORD dwBitmapLength = GetBmpLenght(GetAt(iIndex));
		if(dwBitmapLength == 0)
		{
			ReleaseMutex(hMutex);
			return FALSE;//error of bitmap size
		}	
				//update buffer length variable
		if(m_dwSize < dwBitmapLength)
		{
			ReleaseMutex(hMutex);
			return FALSE;//error of bitmap size
		}
		if(iIndex < m_iCount - 1)
		{
			BYTE * m_BufLastPart = (BYTE *)malloc(sizeof(BYTE) * (m_dwSize - p_dwAddress[iIndex + 1]));
					
				if(m_BufLastPart == NULL)
				{
					ReleaseMutex(hMutex);
					return FALSE; //error: is not enough available memory
				}

				memcpy((BYTE *)m_BufLastPart,(BYTE *) m_pBytes + p_dwAddress[iIndex+1],
										sizeof(BYTE)*(m_dwSize - p_dwAddress[iIndex+1]));

				//change buffer length
				memcpy((BYTE *) m_pBytes+p_dwAddress[iIndex], (BYTE *)m_BufLastPart,
											sizeof(BYTE)*(m_dwSize - p_dwAddress[iIndex + 1]));

				m_pBytes = (BYTE *)realloc((BYTE *)m_pBytes, sizeof(BYTE) * (m_dwSize-dwBitmapLength));
				delete m_BufLastPart;
		}
		else //last image
			m_pBytes = (BYTE *)realloc((BYTE *)m_pBytes, 
						sizeof(BYTE) * (m_dwSize-dwBitmapLength));

		if(m_pBytes == NULL && m_iCount > 1)
		{
			ReleaseMutex(hMutex);
			return FALSE; //error: is not enough available memory
		}	

		m_dwSize -= dwBitmapLength;
		

		//replace addresses and pointers
		for(int i=iIndex+1; i<m_iCount; i++)
		{
			if(i<m_iCount-1)
			{
				p_dwAddress[i] = p_dwAddress[i+1] - dwBitmapLength;
				m_pHeaders[i].bmBits = (BYTE *)(m_pBytes + p_dwAddress[i]);
			}
			m_pHeaders[i-1] = m_pHeaders[i];
		}
		m_iCount --; //decrease count
		if(m_iCount > 0)
			p_dwAddress = (DWORD *)realloc(p_dwAddress, sizeof(DWORD) * m_iCount);
		if(p_dwAddress == NULL && m_iCount > 0)
		{
			ReleaseMutex(hMutex);
			return FALSE; //error: is not enough available memory
		}	
		if(m_iCount > 0)
			m_pHeaders = (BITMAP *)realloc(m_pHeaders, sizeof(BITMAP) * m_iCount);
		if(m_pHeaders == NULL && m_iCount > 0)
		{
			ReleaseMutex(hMutex);
			return FALSE; //error: is not enough available memory
		}	

		if(m_iCount == 0)
		{
			//Clear array
			if(m_pBytes != NULL)
				delete m_pBytes;
			if(m_pHeaders != NULL)
				delete m_pHeaders;
			if(p_dwAddress != NULL)
				delete p_dwAddress;
			m_pBytes = NULL;
			m_pHeaders = NULL;
			p_dwAddress = NULL;
			m_dwSize = 0;
		}
	}
	__finally 
	{ 
		// Release ownership of the mutex object.
        if (! ReleaseMutex(hMutex)) 
			return FALSE;
                    
	} 	
	return TRUE;
}
//Function: remove all bitmaps from array
//No parameters
//No return
void CArrayBITMAP::RemoveAll()
{
	if(WaitForSingleObject( hMutex, ID_BMP_ARRAY_TIMEOUT)!= WAIT_OBJECT_0) // 10 second time-out
        return ; 
	__try 
	{ 
		//Clear array
		if(m_pBytes != NULL)
			delete m_pBytes;
		if(m_pHeaders != NULL)
			delete m_pHeaders;
		if(p_dwAddress != NULL)
			delete p_dwAddress;
		m_pBytes = NULL;
		m_pHeaders = NULL;
		p_dwAddress = NULL;
		m_dwSize = 0;
		m_iCount = 0;
	}
	__finally 
	{ 
		// Release ownership of the mutex object.
		ReleaseMutex(hMutex); 
	} 	
}
//Function: get BITMAPs count
//Return: count
int CArrayBITMAP::GetSize()
{
	if(WaitForSingleObject( hMutex, ID_BMP_ARRAY_TIMEOUT)!= WAIT_OBJECT_0) // 10 second time-out
        return -1;//error 
	__try 
	{
		
	}
	__finally 
	{ 
		// Release ownership of the mutex object.
		ReleaseMutex(hMutex); 
	} 	
	return m_iCount;
}
//Private Function: get BITMAP Lenghth
//Parameter: pointer to BITMAP structure
//Return: Bitmap Lenghth
DWORD CArrayBITMAP::GetBmpLenght(LPBITMAP pBITMAP)
{
	if(pBITMAP->bmWidth <= 0 || pBITMAP->bmHeight <= 0)
		return 0;//to left bitmap

	return ((pBITMAP->bmWidth * pBITMAP->bmPlanes * pBITMAP->bmBitsPixel + 31) & ~31) /8 * pBITMAP->bmHeight;
}
