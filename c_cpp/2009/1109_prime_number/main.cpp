#include <stdio.h>
#include <math.h>
#include <time.h>

int CheckByDiv(unsigned int);
int CheckByRoot(unsigned int);
int main(void)
{
	unsigned int number = 1;
	long firstTime = 0;
	long secondTime = 0;
	printf("���� ������ �Է��ϸ� �Ҽ����� �˷��ٰ�. : ");
	scanf("%d", &number);
	printf("\n");
	firstTime = time(NULL);

	if(CheckByDiv(number)) printf("�� ���� �Ҽ��Դϴ�. \n");
	else printf("�� ���� �Ҽ��� �ƴմϴ�. \n");

	secondTime = time(NULL);
	printf("CheckByDiv() �Լ��� ���� �ɸ� �ð��� %ld���Դϴ�. \n\n", secondTime - firstTime);
	firstTime = time(NULL);

	if(CheckByRoot(number)) printf("�� ���� �Ҽ��Դϴ�. \n");
	else printf("�� ���� �Ҽ��� �ƴմϴ�. \n");

	secondTime = time(NULL);
	printf("CheckByRoot() �Լ��� ���� �ɸ� �ð��� %ld���Դϴ�. \n", secondTime - firstTime);
	return 0;
}

int CheckByDiv(unsigned int num)
{
	register unsigned int div;
	int value = 0;
	for(div = 2;num % div;div++);
	if(div == num) value = 1;
	else value = 0;
	return value;
}

int CheckByRoot(unsigned int num)
{
	unsigned rootNum = (unsigned) sqrt((double)num);
	register unsigned div = 0;
	int value = 0;
	for(div = rootNum; num % div; div--);
	if(div == 1) value = 1;
	else value = 0;
	return value;
}