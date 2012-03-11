#include <iostream> 

#include <time.h>

using namespace std ;

// �������� : �ζ��� �̴� ��(��ȭ�� ���� 0~N���� ������ ����)
const int MaxLottoNumber = 6;

// ���� ����(�Լ�)
bool PatialSame(const int* lotto);

int main(){
	int lotto[MaxLottoNumber];
	srand( (unsigned)time(NULL) );

	do {
		for(int i=0; i<MaxLottoNumber; ++i) {
			lotto[i] = rand() % 45 + 1;
		}
	}while(PatialSame(lotto));

	for(int i=0; i<MaxLottoNumber; ++i) {
		cout << lotto[i] << " "<< endl;
	}

	return 0;
}

// �κ������� ���� ���� ���� ��ȯ
bool PatialSame(const int* lotto)
{
	for(int i=0; i<MaxLottoNumber; ++i) {
		// �ڱ� �ڽ��ϰ� �񱳸� ���� �ʴ´�.(+1�� �ϴ� ����)
		for(int j=i+1; j<MaxLottoNumber; ++j) {
			// ���� ���� �ִٸ� ���� ��ȯ(Ǫ����, pruning; ����ġ��)
			if(lotto[i]==lotto[j]) return true;
		}
	}
	return false;
}
