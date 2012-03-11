// Codo.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <Windows.h>
#define	LSB1			0x0007
#define	PRINT_STAUTS	0x00E3
#define	LSB9			0x01FF
#define	CLEAR_3			0x01C0
int status[] = {3, 3, 3, 4, 4, 4, 4, 3, 3, 3, 4, 1, 1, 1, 3, 4, 5, 3};
int curr_status = 0;

int GetSNMP()
{
	static const int size_of_status = sizeof(status)/sizeof(int) -1;

	Sleep(1000);

	if(curr_status > size_of_status) return 3;
	else return status[curr_status++];
}

bool IsDiffBefore(unsigned long queue, int current_status)
{
	if (~(queue&LSB1) & current_status) return true;
	else return false;
}

void PushToQueue(unsigned long &queue, int status)
{
	queue = queue << 3;
	queue = queue | status;
}

// index�� 0���� ���� [2,1,0]
int GetQueueIndex(unsigned long queue, int i)
{
	queue = queue >> (i*3);
	return (LSB1 & queue);
}

bool IsPrinted(unsigned long queue)
{
	// true Ȥ�� false�� ����
	return (PRINT_STAUTS==(LSB9 & queue));
	/*if(PRINT_STAUTS&~(queue))
		return true;
	else 
		return false;*/
}

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned long queue = 0;
	int s;

	while (1) {
		s = GetSNMP();
		// ť�� ù ��° [2~0]�� ��
		bool bDiff = IsDiffBefore(queue, s);
		if(bDiff) {
			PushToQueue(queue, s);
		}
		
		printf("current status :%d\t / queue : %d|%d|%d / Is Difference : %d\n",
			s, GetQueueIndex(queue, 2), GetQueueIndex(queue, 1), GetQueueIndex(queue, 0), bDiff);

		if(IsPrinted(queue)) {
			printf("PRINTING\n");
			queue = queue | CLEAR_3;
		}

	}

	return 0;
}

