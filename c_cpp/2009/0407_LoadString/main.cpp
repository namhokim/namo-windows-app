#include "resource.h"
#include "WinStrUtil.h"


int main()
{
	std::tstring s = Util::LoadString(IDS_STRING1);
	wprintf(L"%s, %d\n", s.c_str(), s.size());
}
