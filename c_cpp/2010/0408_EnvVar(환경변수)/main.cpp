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

	// ȯ�溯�� ���� �����ϴ� �� �ʿ��� ���� ũ�⸦ �����´�.
	DWORD dwResult = ::GetEnvironmentVariable(pszVariableName, pszValue, 0);
	if (dwResult != 0) {
		// ȯ�溯�� ���� �����ϱ� ���� ���� �Ҵ�
		DWORD size = dwResult;	// * sizeof(TCHAR);
		std::vector<TCHAR> buf(dwResult);
		pszValue = (PTSTR)&buf[0];
		::GetEnvironmentVariable(pszVariableName, pszValue, size);
		_tprintf(TEXT("'%s'=%s\n"), pszVariableName, pszValue);
	} else {
		_tprintf(TEXT("'%s'=<unknown value>\n"), pszVariableName);
	}
}