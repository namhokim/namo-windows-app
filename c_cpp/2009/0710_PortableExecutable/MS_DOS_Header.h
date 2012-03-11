#include <Windows.h>
#include <string>

#ifndef _MS_DOS_HEADER_H_
#define _MS_DOS_HEADER_H_
class MS_DOS_Header
{
public:
	MS_DOS_Header(void);
	~MS_DOS_Header(void);
	
	bool Open(LPCTSTR filename);
	bool OpenA(LPCSTR filename);
	bool OpenW(LPCWSTR filename);
	void Close();
	bool IsOpened();
	
	bool IsDosHeader();
	
	// members
	std::string m_LastError;
private:
	HANDLE m_hFile;
	HANDLE m_hFileMapping;
	PIMAGE_DOS_HEADER m_DosHeader;
	bool OpenPostProcess();
};
#endif
