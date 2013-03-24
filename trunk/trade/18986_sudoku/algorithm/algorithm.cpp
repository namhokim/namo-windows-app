// algorithm.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "algorithm.h"
#include "SudokuSolver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void algorithm();

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
		algorithm();
	}

	return nRetCode;
}


#define SIZE_MAZE	4

void algorithm()
{
	SudokuLoader loader(SIZE_MAZE);
	if (loader.load(_T("sudoku1.txt"))) {
		Sudoku *sudoku = loader.data();
		SudokuSolver solver(sudoku);
		if(solver.solve()) {
			printf("Solved it.");
		} else {
			printf("Cannot solve it.");
		}
	}
}
