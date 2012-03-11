/**
 * @file	AloFileMonitor.h
 * @brief   CAloFileMonitor class 
 * @author	alones
 * @date initial version: 2007.08.22 
 * @modified version: 2008.11.11
 * @brief http://alones.kr/wiki/wiki.php/c_cpp_windows_tip_alofilemonitor
 */

#ifndef __ALO_FILE_MONITOR_H__
#define __ALO_FILE_MONITOR_H__

#include <windows.h>
#include <string>
#include "StringUtil.h"	// for _T

// function pointer for call-back when changed
typedef void (*PFnFileChanged)();

/**
 * @brief monitor file changed
 */
class CAloFileMonitor
{
	// attributes
public:
	_STRING		m_filePath;			/**< file to monitor */	
	_WIN32_FILE_ATTRIBUTE_DATA	m_fileAttData;
	_STRING		m_dirPath;			/**< directory contain the file to monitor */
	PFnFileChanged	m_pFnFileChanged;	/**< call-back when file changed */

	// operations
public:
	CAloFileMonitor()
		:m_filePath(_T("")),m_dirPath(_T("")),m_pFnFileChanged(NULL)
	{
	}
	/** @brief set file monitor, file path and call-back funtion */
	bool SetFileMonitor(_STRING filePath, PFnFileChanged pFn);

	/** @brief start monitor */
	bool Start();

	/** @brief stop monitor */
	bool Stop();
};

#endif //__ALO_FILE_MONITOR_H__