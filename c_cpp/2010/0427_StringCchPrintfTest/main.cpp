#include <tchar.h>
#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Strsafe.h>

// 전방선언
HRESULT ConvertTest(unsigned char *pszaText, PWSTR pszwText, SIZE_T cchSize);

int main() {
	unsigned char pszaText[] = "한글 테스트";
	WCHAR pszwText[1024] = {0,};

	HRESULT hr = ConvertTest(pszaText, pszwText, sizeof(pszwText));

	// The resultant string at pszDest is "The answer is 1 + 2 = 3."

	return 0;
}

HRESULT ConvertTest(unsigned char *pszaText, PWSTR pszwText, SIZE_T cchSize)
{
	return ::StringCchPrintfW(pszwText, cchSize, L"%S", pszaText);
}