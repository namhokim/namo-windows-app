// algorithm.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "algorithm.h"
#include "SudokuSolver.h"
#include <conio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void solveTest();
void stepTest();
void autoPlayTest();

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
		//stepTest();
		autoPlayTest();
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
			cout << disp.getData(x, y, ' ');
		}
		cout << endl;
	}
}

void solveTest()
{
	SudokuLoader loader(SIZE_MAZE);
	if (loader.load(szFilename)) {
		Sudoku *sudoku = loader.data();
		cout << "Problem is..." << endl;;
		displayMaze(sudoku);
		SudokuPlayer player(sudoku);
		if(player.solveAll()) {
			cout << endl << "Solution is..." << endl;
			displayMaze(sudoku);
		} else {
			cout << "Cannot solve it." << endl;
		}
	} else {
		cout << "cannot open the file." << endl;
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

void autoPlayTest()
{
	SudokuLoader loader(SIZE_MAZE);
	if (loader.load(szFilename)) {
		Sudoku *sudoku = loader.data();
		cout << "Problem is..." << endl;
		displayMaze(sudoku);

		SudokuPlayer player(sudoku);
		cout << "Press enter..." << endl;
		cout << "(If want to quit, press any key except enter key.)" << endl;
		while (player.autoPlay()) {
			int ch = _getch();
			if (ch != 13) break;
			cout << endl;
			displayMaze(sudoku);
		}
	} else {
		cout << "cannot open the file." << endl;
	}
}