#pragma once

// 스도쿠 정보를 담고 있는 클래스(전방선언, 실제 구현은 .cpp에 함)
class Sudoku;

// 데이터를 파일에서 불러오는 로더
class SudokuLoader
{
public:
	SudokuLoader(int size);	// 생성자 - 인자: 스도쿠의 크기
	~SudokuLoader(void);	// 소멸자 - 데이터 해지의 책임
	bool load(LPCTSTR file);// 불러오기
	Sudoku* data();			// 데이터 참조 획득

private:
	int m_size;				// 크기 맴버변수
	Sudoku* m_data;			// 데이터 포인터

	void clearData();							// 데이터 삭제
	bool parseLine(const std::string& line);	// 파싱
};

// 데이터 표시를 위해 값을 가져오는 클래스
class SudokuDisplayer
{
public:
	SudokuDisplayer(Sudoku*);
	char getData(int x, int y, char default_value);	// 값을 가져옴
private:
	Sudoku* m_data;
};

// 게임을 담당하는 클래스
class SudokuPlayer
{
public:
	SudokuPlayer(Sudoku*);		// 생성자
	~SudokuPlayer();			// 소멸자
	bool determinePlay(int x, int y, char value);	// 유효하지 않은 진행 false 리턴
	bool play(int x, int y, char value);			// 내부적으로 determinePlay 호출
	bool autoPlay();			// 자동 플레이 - 1단계 게임 질행
	bool solveAll();			// 해결이 가능한 문제모두풀기
	bool hasUndoData();			// 무르기가 가능한 데이터가 있는지 여부
	bool undo();				// 무르기

private:
	Sudoku* m_data;					// 데이터
};
