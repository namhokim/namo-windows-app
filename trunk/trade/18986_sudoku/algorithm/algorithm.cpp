// algorithm.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "algorithm.h"
#include "SudokuSolver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void solveTest();

// ������ ���� ���α׷� ��ü�Դϴ�.

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC�� �ʱ�ȭ�մϴ�. �ʱ�ȭ���� ���� ��� ������ �μ��մϴ�.
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���� �ڵ带 �ʿ信 ���� �����մϴ�.
		_tprintf(_T("�ɰ��� ����: MFC�� �ʱ�ȭ���� ���߽��ϴ�.\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: ���� ���α׷��� ������ ���⿡�� �ڵ��մϴ�.
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

