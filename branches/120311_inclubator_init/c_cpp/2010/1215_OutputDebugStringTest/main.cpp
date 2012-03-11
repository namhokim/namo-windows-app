#define	WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "AdvOutputDebugString.h"
// See. http://msdn.microsoft.com/en-us/library/aa363362(VS.85).aspx

int main()
{
	OutputDebugString(TEXT("OutputDebugString()를 이용한 디버그 메시지"));
	OutputDebugStringA("OutputDebugStringA()를 이용한 디버그 메시지");
	std::wstring wmsg(L"OutputDebugStringW()를 이용한 디버그 메시지");
	OutputDebugStringW(wmsg.c_str());

	Namo::OutputDebugStringA("Namo::OutputDebugStringA 가변 데이터용 : %d\n", 22);
	Namo::OutputDebugStringW(L"Namo::OutputDebugStringW가변 데이터용 : %d\n", 23);
	Namo::OutputDebugString(TEXT("Namo::OutputDebugStringW가변 데이터용 : %d\n"), 20);

	return 0;
}
