#include <string>
#include <iostream>

using namespace std;

namespace Mail
{
template <typename _Elem>
basic_string<_Elem, char_traits<_Elem>, allocator<_Elem> >
MakeDoubleChar(basic_string<_Elem, char_traits<_Elem>, allocator<_Elem> > str, _Elem tokens)
{
	typedef basic_string<_Elem, char_traits<_Elem>, allocator<_Elem> >	strType;

	strType::size_type pivot = str.find_first_of(tokens);
	if(pivot == strType::npos) return str;	// 바꿀 작은 따옴표 없음

	strType rStr;
	rStr.reserve(str.size());	// 메모리 확보(for 성능)
	
	do {
		rStr.append(str.substr(0, pivot) + tokens + tokens);
		str.assign(str.substr(pivot+1));
		pivot = str.find_first_of(tokens);
	} while (pivot != strType::npos);
	if(!rStr.empty()) rStr.append(str);

	return rStr;
}
}

int main()
{
	//  로케일 : 한국어
	::setlocale(LC_ALL, "Korean");
	wstring str(L"09.04.04_IGM과 함께하는 협상스쿨] '협상의 고수' 되려면 상대방 '마음의 문' 부터 열어라.pdf");
	wcout << str << endl;
	wcout << Mail::MakeDoubleChar<wchar_t>(str, '\'') << endl;
	
	return 0;
}
