#include <iostream> 

#include <time.h>

using namespace std ;

// 전역변수 : 로또의 뽑는 수(변화에 따라 0~N개의 선택이 가능)
const int MaxLottoNumber = 6;

// 전방 선언(함수)
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

// 부분적으로 같은 것의 여부 반환
bool PatialSame(const int* lotto)
{
	for(int i=0; i<MaxLottoNumber; ++i) {
		// 자기 자신하고 비교를 하지 않는다.(+1을 하는 이유)
		for(int j=i+1; j<MaxLottoNumber; ++j) {
			// 같은 값이 있다면 참을 반환(푸르닝, pruning; 가지치기)
			if(lotto[i]==lotto[j]) return true;
		}
	}
	return false;
}
