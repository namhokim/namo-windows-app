#pragma once
class SingleInstance
{
private:
	HANDLE  m_hMutex;

public:
	SingleInstance(LPCTSTR lpszMutexName = _T("SingleMutex"))
	{
		m_hMutex = CreateMutex(NULL, TRUE, lpszMutexName);
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(m_hMutex);
			m_hMutex = NULL;
		}
	}

	~SingleInstance()
	{
		if(m_hMutex)
		{
			CloseHandle(m_hMutex);
			m_hMutex = NULL;
		}
	}

	BOOL IsExist() {return m_hMutex==NULL;}
};

