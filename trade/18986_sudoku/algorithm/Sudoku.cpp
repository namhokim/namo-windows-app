#include "StdAfx.h"
#include "Sudoku.h"
#include <string>
#include <fstream>
#include <vector>
#include <set>

typedef std::string::size_type str_size;
typedef std::string::iterator str_iter;
typedef std::string::const_iterator str_const_iter;

//////////////////////////////////////////////////////////////////////////

// 스도쿠를 구성하는 셀
class SudokuElem
{
public:
	SudokuElem(char value, const std::set<char> candidates)
	:m_candidates(candidates)
	{
		if(value!='B') {
			m_candidates.erase(value);
		}
	}

	bool isComplete() const {
		return (m_candidates.size() == 1);
	}
	void remove(char value) {
		m_candidates.erase(value);
	}
private:
	std::set<char> m_candidates;
};

// 스도쿠 데이터(SudokuElem를 포함)
class Sudoku
{
public:
	Sudoku(int size) : m_size(size), m_curr(0) {
		for (int i=0; i<size; ++i) {
			m_data.push_back(std::vector <SudokuElem> ());
			m_candidates.insert('1'+i);	// '1','2','3'....
		}
	}
	void push_data(char data) {
		m_data[m_curr].push_back(SudokuElem(data, m_candidates));
		if(m_data[m_curr].size()==m_size) {
			m_curr++;
		}
	}
private:
	int m_size, m_curr;
	std::vector< std::vector< SudokuElem > > m_data;
	std::set<char> m_candidates;
};
//////////////////////////////////////////////////////////////////////////

SudokuLoader::SudokuLoader(int size)
{
	m_size = size;
	m_data = new Sudoku(size);
}

SudokuLoader::~SudokuLoader(void)
{
	if(m_data!=NULL) delete m_data;
}

bool SudokuLoader::load(LPCTSTR file)
{
	std::ifstream is(file, std::ios::in);
	if (is)
	{
		std::string line;
		int i=0;
		while(getline(is, line))
		{
			if (i>=m_size) return false;	// 파일 포맷 오류 대비

			parseLine(line.c_str());
			i++;
		}
		is.close();	// 명시적으로 닫아주지 않아도 자동으로 닫힘
		return true;
	}
	return false;
}

Sudoku *SudokuLoader::data()
{
	return m_data;
}

void SudokuLoader::parseLine(const std::string& line)
{
	for (str_const_iter pos=line.begin(); pos!=line.end(); ++pos)
	{
		m_data->push_data(*pos);
	}
}

//////////////////////////////////////////////////////////////////////////

SudokuSolver::SudokuSolver(Sudoku* sudoku)
:m_data(sudoku)
{
}

void SudokuSolver::solve()
{
	
}