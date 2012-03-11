#pragma once

#define	MAX_WAIT_TIME_FOR_TEST_PROCESS	30000	// 30 secs

namespace process {
	int Execute(CString& exeFile, int arg1);
} /* namespace process */