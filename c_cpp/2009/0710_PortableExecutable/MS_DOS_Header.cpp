#include "MS_DOS_Header.h"

MS_DOS_Header::MS_DOS_Header(void)
:m_hFile(INVALID_HANDLE_VALUE), m_hFileMapping(0), m_DosHeader(0)
{
}

MS_DOS_Header::~MS_DOS_Header(void)
{
	Close();
}

inline void CloseHandleAndInit(HANDLE hObject, HANDLE initValue)
{
	::CloseHandle(hObject);
	hObject = initValue;
}

// File Open
bool MS_DOS_Header::Open(LPCTSTR filename)
{
#ifdef  UNICODE
	return OpenW(filename);
#else
	return OpenA(filename);
#endif
}

bool MS_DOS_Header::OpenA(LPCSTR filename)
{
	Close();
	
	m_hFile = ::CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		
	return OpenPostProcess();
}

bool MS_DOS_Header::OpenW(LPCWSTR filename)
{
	Close();
	
	m_hFile = ::CreateFileW(filename, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		
	return OpenPostProcess();
}

bool MS_DOS_Header::OpenPostProcess()
{
	if ( m_hFile == INVALID_HANDLE_VALUE )
	{
		m_LastError = "Couldn't open file with CreateFile()";
		return false;
	}
	
	m_hFileMapping = ::CreateFileMapping(m_hFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if ( m_hFileMapping == 0 )
	{
		::CloseHandleAndInit(m_hFile, INVALID_HANDLE_VALUE);
		m_LastError = "Couldn't open file mapping with CreateFileMapping()";
		return false;
	}
	
	LPVOID lpFileBase;
	lpFileBase = ::MapViewOfFile(m_hFileMapping, FILE_MAP_READ, 0, 0, 0);
	if ( lpFileBase == 0 )
	{
		::CloseHandleAndInit(m_hFileMapping, 0);
		::CloseHandleAndInit(m_hFile, INVALID_HANDLE_VALUE);
		m_LastError = "Couldn't map view of file with MapViewOfFile()";
		return false;
	}
	m_DosHeader = (PIMAGE_DOS_HEADER)lpFileBase;
	
	return true;
}


// File Close
void MS_DOS_Header::Close()
{
	if ( 0 != (LPVOID)m_DosHeader ) {
		::UnmapViewOfFile((LPVOID)m_DosHeader);
		m_DosHeader = 0;
	}
	if ( 0 != m_hFileMapping )				::CloseHandleAndInit(m_hFileMapping, 0);
	if ( INVALID_HANDLE_VALUE != m_hFile )	::CloseHandleAndInit(m_hFile, INVALID_HANDLE_VALUE);
}

bool MS_DOS_Header::IsOpened()
{
	return ( INVALID_HANDLE_VALUE!=m_hFile
	&& 0 != m_hFileMapping
	&& 0 != (LPVOID)m_DosHeader);
}


bool MS_DOS_Header::IsDosHeader()
{
	return ( IsOpened() ) && ( IMAGE_DOS_SIGNATURE == (m_DosHeader->e_magic) );
}
