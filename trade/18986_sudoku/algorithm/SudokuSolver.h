#pragma once

// ������ ������ ��� �ִ� Ŭ����(���漱��, ���� ������ .cpp�� ��)
class Sudoku;

// �����͸� ���Ͽ��� �ҷ����� �δ�
class SudokuLoader
{
public:
	SudokuLoader(int size);	// ������ - ����: �������� ũ��
	~SudokuLoader(void);	// �Ҹ��� - ������ ������ å��
	bool load(LPCTSTR file);// �ҷ�����
	Sudoku* data();			// ������ ���� ȹ��

private:
	int m_size;				// ũ�� �ɹ�����
	Sudoku* m_data;			// ������ ������

	void clearData();							// ������ ����
	bool parseLine(const std::string& line);	// �Ľ�
};

// ������ ǥ�ø� ���� ���� �������� Ŭ����
class SudokuDisplayer
{
public:
	SudokuDisplayer(Sudoku*);
	char getData(int x, int y, char default_value);	// ���� ������
private:
	Sudoku* m_data;
};

// ������ ����ϴ� Ŭ����
class SudokuPlayer
{
public:
	SudokuPlayer(Sudoku*);		// ������
	~SudokuPlayer();			// �Ҹ���
	bool determinePlay(int x, int y, char value);	// ��ȿ���� ���� ���� false ����
	bool play(int x, int y, char value);			// ���������� determinePlay ȣ��
	bool autoPlay();			// �ڵ� �÷��� - 1�ܰ� ���� ����
	bool solveAll();			// �ذ��� ������ �������Ǯ��
	bool hasUndoData();			// �����Ⱑ ������ �����Ͱ� �ִ��� ����
	bool undo();				// ������

private:
	Sudoku* m_data;					// ������
};
