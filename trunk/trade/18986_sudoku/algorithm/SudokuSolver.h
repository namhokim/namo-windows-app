#pragma once

class Sudoku;

class SudokuLoader
{
public:
	SudokuLoader(int);
	~SudokuLoader(void);
	bool load(LPCTSTR file);
	Sudoku* data();

private:
	int m_size;
	Sudoku* m_data;

	void clearData();
	bool parseLine(const std::string& line);
};

class SudokuSolver
{
public:
	SudokuSolver(Sudoku*);
	bool solve();

private:
	Sudoku* m_data;
};

class SudokuDisplayer
{
public:
	SudokuDisplayer(Sudoku*);
	char getData(int x, int y, char default_value);
private:
	Sudoku* m_data;
};

class SudokuPlayer
{
public:
	SudokuPlayer(Sudoku*);
	bool determinePlay(int x, int y, char value);	// 유효하지 않은 진행 false 리턴
	bool play(int x, int y, char value);			// 내부적으로 determinePlay 호출

private:
	Sudoku* m_data;
};
