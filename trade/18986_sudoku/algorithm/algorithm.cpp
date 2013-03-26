// algorithm.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "algorithm.h"
#include "SudokuSolver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void solveTest();
void stepTest();

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
		//solveTest();
		stepTest();
	}

	return nRetCode;
}


#define SIZE_MAZE	4
LPCTSTR szFilename = _T("sudoku1.txt");

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
	if (loader.load(szFilename)) {
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

void stepTest()
{
	SudokuLoader loader(SIZE_MAZE);
	if (loader.load(szFilename)) {
		Sudoku *sudoku = loader.data();
		cout << "Problem is..." << endl;
		displayMaze(sudoku);

		SudokuPlayer player(sudoku);
		cout << "Position started with 0." << endl;
		cout << "If want to quit, press Ctrl-C." << endl;
		while (true) {
			int x, y;
			char value;
			cout << "x: ";	cin >> x;
			cout << "y: ";	cin >> y;
			cout << "value: ";	cin >> value;
			if(player.play(x,y,value)) {
				cout << "OK." << endl;
				displayMaze(sudoku);
			} else {
				cout << "NG." << endl;
			}
		}
	} else {
		cout << "cannot open the file." << endl;
	}
}