#include "charlib.h"

//	BOOL charicmp(2)
//
//	설명 : 두개의 char형을 입력받아 대소문자 관계 없이 비교한다.
//	방법 : 알파벳일 경우 대문자로 바꾸어서 비교.
//	인자들 :
//	i	c1 - 비교할 문자 1
//	i	c2 - 비교할 문자 2
//	반환값 :
//		TRUE, 1 : 같음
//		FALSE, 2 : 다름
BOOL charicmp(char c1, char c2)
{
	// 모두 대문자로 만들어서 비교

	// c1이 소문자일 경우
	if(c1>='a' && c1<='z') {
		c1 -= ('a' - 'A');
	}

	// c2이 소문자일 경우
	if(c2>='a' && c2<='z') {
		c2 -= ('a' - 'A');
	}

	if(c1 == c2) return TRUE;
	else return FALSE;
}