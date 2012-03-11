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
	printf("양의 정수를 입력하면 소수인지 알려줄게. : ");
	scanf("%d", &number);
	printf("\n");
	firstTime = time(NULL);

	if(CheckByDiv(number)) printf("이 수는 소수입니다. \n");
	else printf("이 수는 소수가 아닙니다. \n");

	secondTime = time(NULL);
	printf("CheckByDiv() 함수에 의해 걸린 시간은 %ld초입니다. \n\n", secondTime - firstTime);
	firstTime = time(NULL);

	if(CheckByRoot(number)) printf("이 수는 소수입니다. \n");
	else printf("이 수는 소수가 아닙니다. \n");

	secondTime = time(NULL);
	printf("CheckByRoot() 함수에 의해 걸린 시간은 %ld초입니다. \n", secondTime - firstTime);
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