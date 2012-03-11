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
	if(pivot == strType::npos) return str;	// �ٲ� ���� ����ǥ ����

	strType rStr;
	rStr.reserve(str.size());	// �޸� Ȯ��(for ����)
	
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
	//  ������ : �ѱ���
	::setlocale(LC_ALL, "Korean");
	wstring str(L"09.04.04_IGM�� �Բ��ϴ� ������] '������ ���' �Ƿ��� ���� '������ ��' ���� �����.pdf");
	wcout << str << endl;
	wcout << Mail::MakeDoubleChar<wchar_t>(str, '\'') << endl;
	
	return 0;
}
