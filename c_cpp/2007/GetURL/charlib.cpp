#include "charlib.h"

//	BOOL charicmp(2)
//
//	���� : �ΰ��� char���� �Է¹޾� ��ҹ��� ���� ���� ���Ѵ�.
//	��� : ���ĺ��� ��� �빮�ڷ� �ٲپ ��.
//	���ڵ� :
//	i	c1 - ���� ���� 1
//	i	c2 - ���� ���� 2
//	��ȯ�� :
//		TRUE, 1 : ����
//		FALSE, 2 : �ٸ�
BOOL charicmp(char c1, char c2)
{
	// ��� �빮�ڷ� ���� ��

	// c1�� �ҹ����� ���
	if(c1>='a' && c1<='z') {
		c1 -= ('a' - 'A');
	}

	// c2�� �ҹ����� ���
	if(c2>='a' && c2<='z') {
		c2 -= ('a' - 'A');
	}

	if(c1 == c2) return TRUE;
	else return FALSE;
}