// algorithm.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "algorithm.h"
#include "SudokuSolver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void solveTest();

// 유일한 응용 프로그램 개체입니다.

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 오류 코드를 필요에 따라 수정합니다.
		_tprintf(_T("심각한 오류: MFC를 초기화하지 못했습니다.\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 응용 프로그램의 동작은 여기에서 코딩합니다.
		solveTest();
	}

	return nRetCode;
}


#define SIZE_MAZE	4

void displayMaze(Sudoku * sudoku)
{
	SudokuDisplayer disp(sudoku);
	for (int x=0; x<SIZE_MAZE; x++)
	{
		for (int y=0; y<SIZE_MAZE; y++)
		{
			char ch = disp.getData(x, y, ' ');
			printf("%c ", ch);
		}
		printf("\n");
	}
}

void solveTest()
{
	SudokuLoader loader(SIZE_MAZE);
	if (loader.load(_T("sudoku1.txt"))) {
		Sudoku *sudoku = loader.data();
		printf("Problem is...\n");
		displayMaze(sudoku);
		SudokuSolver solver(sudoku);
		if(solver.solve()) {
			printf("\nSolution is...\n");
			displayMaze(sudoku);
		} else {
			printf("Cannot solve it.");
		}
	} else {
		printf("cannot open the file.");
	}
}

