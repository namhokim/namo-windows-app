// EXPORTS ���ǿ� �ͱ۸��� ��ũ �߰�
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