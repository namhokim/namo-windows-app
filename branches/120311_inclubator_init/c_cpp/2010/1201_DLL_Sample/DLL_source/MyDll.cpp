// EXPORTS 섹션에 맹글린된 링크 추가
#pragma comment(linker, "/export:Add=?Add@@YAHHH@Z")

// extern "C"
__declspec(dllexport)
int __cdecl Add(int a, int b)
{
	return (a + b);
}

__declspec(dllexport)
int __cdecl Sub(int a, int b)
{
	return (a - b);
}