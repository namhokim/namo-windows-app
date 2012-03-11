#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning (disable:4996)

// define some macro values ///////////////////////////////////////////////////////////////////////////
#define	MAX_NAME_LENGTH		20	// 사람 이름 길이
#define	MAX_INT_LENGTH		20	// 사람 이름 길이
#define	MAX_STUDENT_NUMBER	50	// 입력 받을 최대 학생 개수
#define NUMBER_OF_SUBJECT	3	// 과목 숫자(_student 구조체의 과목의 개수가 변할 때 같이 변화)
#define MIN_NUMBER		-1		// 점수가 될 수 없는 작은 수
#define	TRUE 1
#define FALSE 0
#define BOOL int
#define OUTPUT_FILE_NAME	"이상구.txt"

// define structures ///////////////////////////////////////////////////////////////////////////////////
typedef struct _student {
	char name[MAX_NAME_LENGTH+1];
	int scoreKor;
	int scoreEng;
	int scoreMath;
	int scoreTotal;
	float scoreAvg;
} Student, * pStudent;

typedef struct _com_result {
	int total;
	float average;
	int oepn;
} ComResult, * pComResult;

// 함수 정의(들) ////////////////////////////////////////////////////////////////////////////////////////
void UserInput();
void InputMsg();
void InputString(const char *displayTitle, char *name);
int  InputNumber(const char *displayTitle);
void CalcurateScore(pStudent student);
void SetMaxScore(const int currIdx, int *currMaxScore, const int currScore, int *MaxScoreIndex, int *numberOfMaxScoreIndex);
BOOL isNumber(const char * numberStr, const size_t number);
void OutputToScreen(const int numberOfStudents,
					const int topScore, const int *indexMaxScore, const int numberOfMaxScoreIndex,
					pStudent studentData);
void OutputToFile(pStudent studentData, const int numberOfStudents);

// start up function /////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	UserInput();
	
	return 0;
}

// 함수 선언(들) //////////////////////////////////////////////////////////////////////////////////////////
void UserInput()
{
	int currentIndex, currentMaxScore, indexMaxScore[MAX_STUDENT_NUMBER], numberOfMaxScoreIndex;
	Student student[MAX_STUDENT_NUMBER];

	currentIndex = 0;
	currentMaxScore = MIN_NUMBER;
	numberOfMaxScoreIndex = 0;

	while(1) {
		InputMsg();
		InputString("이름", student[currentIndex].name);
		if(strlen(student[currentIndex].name) == 0) break;	// 빈문자 입력일 때 종료
		student[currentIndex].scoreKor = InputNumber("국어");
		student[currentIndex].scoreEng = InputNumber("영어");
		student[currentIndex].scoreMath= InputNumber("수학");
		CalcurateScore(&student[currentIndex]);
		SetMaxScore(currentIndex, &currentMaxScore, student[currentIndex].scoreTotal, indexMaxScore, &numberOfMaxScoreIndex);
		++currentIndex;
	}

	// 파일 출력
	OutputToFile(student, currentIndex);
	// 화면 출력
	OutputToScreen(currentIndex, currentMaxScore, indexMaxScore, numberOfMaxScoreIndex, student);
}

// 입력 Message
void InputMsg()	{	fprintf(stdout, "# 데이터를 입력하세요.\n");	}

// displayTitle : in, name : out
void InputString(const char *displayTitle, char *name)
{
	name[0] = '\0';
	fprintf(stdout, "%s : ", displayTitle);
	fscanf(stdin, "%[^\n]s", name);
}

// displayTitle : in
int InputNumber(const char *displayTitle)
{
	char strBuffer[MAX_INT_LENGTH];
	int inputNumber;

	while(1) {
		fprintf(stdout, "%s : ", displayTitle);
		if(fscanf(stdin, "%s", strBuffer) != 1) continue;
		if(isNumber(strBuffer, strlen(strBuffer)) == TRUE) {
			inputNumber = atoi(strBuffer);
			rewind(stdin);	// 버퍼를 비우기
			return inputNumber;
		}
	}
}

// student : in & out
void CalcurateScore(pStudent student)
{
	student->scoreTotal = student->scoreKor + student->scoreEng + student->scoreMath;
	student->scoreAvg = student->scoreTotal / (float)NUMBER_OF_SUBJECT;
}

void SetMaxScore(const int currIdx, int *currMaxScore, const int currScore, int *MaxScoreIndex, int *numberOfMaxScoreIndex)
{
	if(0 == currIdx) {	// 맨 처음 값
		(*currMaxScore) = currScore;
		MaxScoreIndex[0] = 0;
		(*numberOfMaxScoreIndex) = 1;
	}
	else if(currScore > (*currMaxScore)) {	// 최대값 바뀜
		(*currMaxScore) = currScore;
		MaxScoreIndex[0] = currIdx;
		(*numberOfMaxScoreIndex) = 1;
	}
	else if(currScore == (*currMaxScore)) {	// 또 다른 최대값과 같음(예.공동 1위)
		(*currMaxScore) = currScore;
		MaxScoreIndex[(*numberOfMaxScoreIndex)] = currIdx;
		++(*numberOfMaxScoreIndex);
	}
	// 현재 값이 현재 최소값보다 작은 경우에는 처리 할 필요 없음
}

BOOL isNumber(const char * numberStr, const size_t number)
{
	size_t i;
	for(i=0; i < number; ++i) {
		if(numberStr[i] < '0' || numberStr[i] > '9') return FALSE;
	}
	return TRUE;
}

void OutputToScreen(const int numberOfStudents,
					const int topScore, const int *indexMaxScore, const int numberOfMaxScoreIndex,
					pStudent studentData)
{
	int i, tmpInt;

	fprintf(stdout, "# 입력된 학생수는 모두 %d명입니다\n", numberOfStudents);
	fprintf(stdout, "# 총합계점수가 가장 높은 학생은 %d점을 받은 ", topScore);
	for(i=0 ; i < numberOfMaxScoreIndex; ++i) {
		if(i>0) fprintf(stdout, ", ");
		tmpInt = indexMaxScore[i];
		fprintf(stdout, "%s", studentData[tmpInt].name);
	}
	fprintf(stdout, " 입니다.\n");

}

// int(4 Byte)의 최대값은 10자리(2147483647)로 가정
void OutputToFile(pStudent studentData, const int numberOfStudents)
{
	int i, j;
	size_t k, maxNameLength, currLength;
	FILE *output_file;

	output_file = fopen(OUTPUT_FILE_NAME,"wt");
	if(output_file == NULL) return;

	// 가장긴 이름의 길이 구하기
	maxNameLength = 0;
	for(i=0; i<numberOfStudents; ++i) {
		currLength = strlen(studentData[i].name);
		if(maxNameLength < currLength) maxNameLength = currLength;
	}
	++maxNameLength;

	fprintf(output_file,"성명");
	for(k=strlen("성명"); k<=maxNameLength; ++k) fprintf(output_file, "%c", ' ');
	fprintf(output_file,"        국어         영어         수학         총점         평균\n");
	fprintf(output_file,"=============================================================================\n");
	for(i=0; i<numberOfStudents; ++i) {
		fprintf(output_file,"%s", studentData[i].name);
		for(k=strlen(studentData[i].name); k<=maxNameLength; ++k) fprintf(output_file, "%c", ' ');
		
		for(j=(int)log10(studentData[i].scoreKor); j<=10; ++j) fprintf(output_file, "%c", ' ');
		fprintf(output_file,"%d ", studentData[i].scoreKor);
		for(j=(int)log10(studentData[i].scoreEng); j<=10; ++j) fprintf(output_file, "%c", ' ');
		fprintf(output_file,"%d ", studentData[i].scoreEng);
		for(j=(int)log10(studentData[i].scoreMath); j<=10; ++j) fprintf(output_file, "%c", ' ');
		fprintf(output_file,"%d ", studentData[i].scoreMath);
		for(j=(int)log10(studentData[i].scoreTotal); j<=10; ++j) fprintf(output_file, "%c", ' ');
		fprintf(output_file,"%d ", studentData[i].scoreTotal);
		for(j=(int)log10(studentData[i].scoreAvg)+2; j<=10; ++j) fprintf(output_file, "%c", ' ');
		fprintf(output_file,"%.1lf\n", studentData[i].scoreAvg);
	}
	fclose(output_file);
}
