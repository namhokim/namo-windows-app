// algorithm.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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