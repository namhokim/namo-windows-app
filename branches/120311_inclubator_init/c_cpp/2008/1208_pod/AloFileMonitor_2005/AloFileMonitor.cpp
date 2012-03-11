/**
 * @file	AloFileMonitor.cpp
 * @brief   CAloFileMonitor class 
 * @author	alones
 * @date initial version: 2007.08.22 
 * @brief http://alones.kr/wiki/wiki.php/c_cpp_windows_tip_alofilemonitor
 */

#include "AloFileMonitor.h"


/** @brief set file monitor, file path and call-back funtion */
bool CAloFileMonitor::SetFileMonitor(_STRING filePath, PFnFileChanged pFn)
{
	if( filePath.size() == 0 || pFn == NULL)
		return false;

	size_t nPos = filePath.rfind(_T("\\"));
	if( nPos == -1 )
	{
		nPos = filePath.rfind(_T("/"));
		if( nPos == -1)
			return false;
	}

	// set dir path
	m_dirPath = filePath.substr(0, nPos+1);

	// set file path
	m_filePath = filePath;

	// get last-write time of the file	
	GetFileAttributesEx( m_filePath.c_str(), GetFileExInfoStandard, (void*)&m_fileAttData);	
	

	// set function pointer
	m_pFnFileChanged = pFn;
	
	return true;
}

HANDLE dwChangeHandles[2]; 	

DWORD WINAPI thFnMonitor(LPVOID lpParam);
DWORD WINAPI thFnMonitor(LPVOID lpParam)
{
	CAloFileMonitor* pMonitor = (CAloFileMonitor*)lpParam;
	if( pMonitor == NULL)
		return 0;

	DWORD dwWaitStatus; 	
	_WIN32_FILE_ATTRIBUTE_DATA fileAttData;
	
	// Watch the C:\WINDOWS directory for file creation and 
	// deletion. 
	
	dwChangeHandles[0] = FindFirstChangeNotification( 
		pMonitor->m_dirPath.c_str(),           // directory to watch 
		FALSE,                         // do not watch the subtree 
		FILE_NOTIFY_CHANGE_LAST_WRITE); // watch file name changes 
	
	if (dwChangeHandles[0] == INVALID_HANDLE_VALUE)
	{
		DWORD dError = GetLastError();
		ExitProcess(GetLastError()); 
	}	

	dwChangeHandles[1] = CreateSemaphore( 
		NULL,   // no security attributes
		0,   // initial count
		1,   // maximum count
		NULL);  // unnamed semaphore

	if( dwChangeHandles[1] == NULL)
		return 0;
	
	// Change notification is set. Now wait on both notification 
	// handles and refresh accordingly. 	
	while (TRUE) 
	{ 
		
		// Wait for notification.
		
		dwWaitStatus = WaitForMultipleObjects(2, dwChangeHandles, FALSE, INFINITE); 
		
		switch (dwWaitStatus) 
		{ 
        case WAIT_OBJECT_0: 
			
			// check the file is changed
			// Retrieve the file times for the file.
			GetFileAttributesEx( pMonitor->m_filePath.c_str(), GetFileExInfoStandard, (void*)&fileAttData);				

			if( CompareFileTime(&fileAttData.ftLastWriteTime, &pMonitor->m_fileAttData.ftLastWriteTime) != 0 )
			{
				(*pMonitor->m_pFnFileChanged)();
				pMonitor->m_fileAttData = fileAttData;
			}

			// A file was created or deleted in C:\WINDOWS. 
			// Refresh this directory and restart the 
			// change notification. RefreshDirectory is an 
			// application-defined function. 			
				if ( FindNextChangeNotification( dwChangeHandles[0]) == FALSE ) 
					return 0;
				break;
		case WAIT_OBJECT_0 + 1: 
			return 0;
			break;

        default: 
            return 0;
		} 
	} 


	return 0;
}

/** @brief start monitor */
bool CAloFileMonitor::Start()
{
	if(CAloFileMonitor::m_filePath.size()==0 ||
		CAloFileMonitor::m_dirPath.size()==0 ||
		CAloFileMonitor::m_pFnFileChanged==NULL) return false;
	DWORD dwThreadId;
	HANDLE hThread;
	
	hThread = CreateThread( 
        NULL,           // no security attributes 
        0,              // use default stack size  
        thFnMonitor,    // thread function 
        this,			// argument to thread function 
        0,              // use default creation flags 
        &dwThreadId);   // returns the thread identifier 

	return true;
}

/** @brief stop monitor */
bool CAloFileMonitor::Stop()
{

	if (!ReleaseSemaphore( 
        dwChangeHandles[1],  // handle to semaphore
        1,           // increase count by one
        NULL) )      // not interested in previous count
	{
		return false;
	}

	::Sleep(100);

	return true;
}