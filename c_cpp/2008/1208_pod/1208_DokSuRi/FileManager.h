#pragma once
#include "stringUtil.h"

class FileManager
{
public:
	FileManager(void);
	~FileManager(void);
	_STRING getFilenameByTime(_STRING ext=_T(""), _STRING prefixFilename=_T(""), _STRING postfixFilename=_T(""));
	bool saveToFile(_STRING filename, _STRING content);
	bool removeFile(_STRING filename);
	bool copyFile(_STRING origin, _STRING dest);
private:
	_STRING getTimeString();
};
