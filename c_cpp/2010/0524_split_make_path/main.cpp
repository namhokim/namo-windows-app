#include <stdlib.h>
#include <string>
#include <Windows.h>

std::wstring getFileNameW(const wchar_t *path);

int main() {
	MessageBox(NULL,
		getFileNameW(L"C:\\Documents and Settings\\namhokim\\바탕 화면\\OID정리_091113").c_str(),
		L"알림", MB_OK);
	return 0;
}

std::wstring getFileNameW(const wchar_t *path)
{
	wchar_t filename[_MAX_FNAME] = {0,};
	wchar_t extension[_MAX_EXT] = {0,};
	errno_t r = _wsplitpath_s(path, NULL, 0, NULL, 0, filename, _MAX_FNAME, extension, _MAX_EXT);
	if(0==r) {
		wchar_t file[_MAX_PATH] = {0, };
		_wmakepath_s(file, _MAX_PATH, NULL, NULL, filename, extension);

		return std::wstring(file);
	}
	return L"";
}