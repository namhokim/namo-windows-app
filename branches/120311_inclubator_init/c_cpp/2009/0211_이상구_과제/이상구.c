#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#pragma warning (disable:4996)

// define some macro values ///////////////////////////////////////////////////////////////////////////
#define	MAX_NAME_LENGTH		20	// ��� �̸� ����
#define	MAX_INT_LENGTH		20	// ��� �̸� ����
#define	MAX_STUDENT_NUMBER	50	// �Է� ���� �ִ� �л� ����
#define NUMBER_OF_SUBJECT	3	// ���� ����(_student ����ü�� ������ ������ ���� �� ���� ��ȭ)
#define MIN_NUMBER		-1		// ������ �� �� ���� ���� ��
#define	TRUE 1
#define FALSE 0
#define BOOL int
#define OUTPUT_FILE_NAME	"�̻�.txt"

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

// �Լ� ����(��) ////////////////////////////////////////////////////////////////////////////////////////
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

// �Լ� ����(��) //////////////////////////////////////////////////////////////////////////////////////////
void UserInput()
{
	int currentIndex, currentMaxScore, indexMaxScore[MAX_STUDENT_NUMBER], numberOfMaxScoreIndex;
	Student student[MAX_STUDENT_NUMBER];

	currentIndex = 0;
	currentMaxScore = MIN_NUMBER;
	numberOfMaxScoreIndex = 0;

	while(1) {
		InputMsg();
		InputString("�̸�", student[currentIndex].name);
		if(strlen(student[currentIndex].name) == 0) break;	// ���� �Է��� �� ����
		student[currentIndex].scoreKor = InputNumber("����");
		student[currentIndex].scoreEng = InputNumber("����");
		student[currentIndex].scoreMath= InputNumber("����");
		CalcurateScore(&student[currentIndex]);
		SetMaxScore(currentIndex, &currentMaxScore, student[currentIndex].scoreTotal, indexMaxScore, &numberOfMaxScoreIndex);
		++currentIndex;
	}

	// ���� ���
	OutputToFile(student, currentIndex);
	// ȭ�� ���
	OutputToScreen(currentIndex, currentMaxScore, indexMaxScore, numberOfMaxScoreIndex, student);
}

// �Է� Message
void InputMsg()	{	fprintf(stdout, "# �����͸� �Է��ϼ���.\n");	}

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
			rewind(stdin);	// ���۸� ����
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
	if(0 == currIdx) {	// �� ó�� ��
		(*currMaxScore) = currScore;
		MaxScoreIndex[0] = 0;
		(*numberOfMaxScoreIndex) = 1;
	}
	else if(currScore > (*currMaxScore)) {	// �ִ밪 �ٲ�
		(*currMaxScore) = currScore;
		MaxScoreIndex[0] = currIdx;
		(*numberOfMaxScoreIndex) = 1;
	}
	else if(currScore == (*currMaxScore)) {	// �� �ٸ� �ִ밪�� ����(��.���� 1��)
		(*currMaxScore) = currScore;
		MaxScoreIndex[(*numberOfMaxScoreIndex)] = currIdx;
		++(*numberOfMaxScoreIndex);
	}
	// ���� ���� ���� �ּҰ����� ���� ��쿡�� ó�� �� �ʿ� ����
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

	fprintf(stdout, "# �Էµ� �л����� ��� %d���Դϴ�\n", numberOfStudents);
	fprintf(stdout, "# ���հ������� ���� ���� �л��� %d���� ���� ", topScore);
	for(i=0 ; i < numberOfMaxScoreIndex; ++i) {
		if(i>0) fprintf(stdout, ", ");
		tmpInt = indexMaxScore[i];
		fprintf(stdout, "%s", studentData[tmpInt].name);
	}
	fprintf(stdout, " �Դϴ�.\n");

}

// int(4 Byte)�� �ִ밪�� 10�ڸ�(2147483647)�� ����
void OutputToFile(pStudent studentData, const int numberOfStudents)
{
	int i, j;
	size_t k, maxNameLength, currLength;
	FILE *output_file;

	output_file = fopen(OUTPUT_FILE_NAME,"wt");
	if(output_file == NULL) return;

	// ����� �̸��� ���� ���ϱ�
	maxNameLength = 0;
	for(i=0; i<numberOfStudents; ++i) {
		currLength = strlen(studentData[i].name);
		if(maxNameLength < currLength) maxNameLength = currLength;
	}
	++maxNameLength;

	fprintf(output_file,"����");
	for(k=strlen("����"); k<=maxNameLength; ++k) fprintf(output_file, "%c", ' ');
	fprintf(output_file,"        ����         ����         ����         ����         ���\n");
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
