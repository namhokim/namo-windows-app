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