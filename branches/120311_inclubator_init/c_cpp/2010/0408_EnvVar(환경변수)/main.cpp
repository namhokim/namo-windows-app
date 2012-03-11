#include <Windows.h>
#include <tchar.h>

void PrintEvironmentVariable(PCTSTR pszVariableName);

int main()
{
	PrintEvironmentVariable(TEXT("GOROOT"));
	::SetEnvironmentVariable(TEXT("GOROOT"), TEXT("c:\\go\\"));
	PrintEvironmentVariable(TEXT("GOROOT"));

	return 0;
}

#include <vector>
void PrintEvironmentVariable(PCTSTR pszVariableName)
{
	PTSTR pszValue = NULL;

	// 환경변수 값을 저장하는 데 필요한 버퍼 크기를 가져온다.
	DWORD dwResult = ::GetEnvironmentVariable(pszVariableName, pszValue, 0);
	if (dwResult != 0) {
		// 환경변수 값을 저장하기 위한 버퍼 할당
		DWORD size = dwResult;	// * sizeof(TCHAR);
		std::vector<TCHAR> buf(dwResult);
		pszValue = (PTSTR)&buf[0];
		::GetEnvironmentVariable(pszVariableName, pszValue, size);
		_tprintf(TEXT("'%s'=%s\n"), pszVariableName, pszValue);
	} else {
		_tprintf(TEXT("'%s'=<unknown value>\n"), pszVariableName);
	}
}