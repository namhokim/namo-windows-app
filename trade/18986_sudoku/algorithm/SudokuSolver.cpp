#include "StdAfx.h"
#include "SudokuSolver.h"
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>	// for set_difference
#include <iterator>		// for std::inserter (vs2010 error, if not exist)

typedef std::string::size_type str_size;
typedef std::string::iterator str_iter;
typedef std::string::const_iterator str_const_iter;
typedef std::set<char> set_char;

//////////////////////////////////////////////////////////////////////////

// 스도쿠를 구성하는 셀
class SudokuElem
{
public:
	SudokuElem(char value, const set_char candidates)
	{
		if(value=='B') {
			m_candidates = candidates;
		} else {
			m_candidates.insert(value);
		}
	}

	bool isComplete() const {
		return (m_candidates.size() == 1);
	}
	void remove(char value) {
		m_candidates.erase(value);
	}
	char getFirstElement() {
		return *(m_candidates.begin());
	}
	void remove(const set_char& used_set) {
		set_char result;
		std::set_difference(m_candidates.begin(), m_candidates.end(),
			used_set.begin(), used_set.end(), std::inserter(result, result.end()));
		m_candidates.clear();
		m_candidates.insert(result.begin(), result.end());
	}
private:
	set_char m_candidates;
};

typedef std::vector< SudokuElem > vector_SudokuElem;
typedef std::vector< vector_SudokuElem > vector2D_SudokuElem;

// 스도쿠 데이터(SudokuElem를 포함)
class Sudoku
{
public:
	Sudoku(int size) : m_size(size), m_curr(0) {
		for (int i=0; i<size; ++i) {
			m_data.push_back(vector_SudokuElem ());
			m_candidates.insert('1'+i);	// '1','2','3'....
		}
	}
	void push_data(char data) {
		m_data[m_curr].push_back(SudokuElem(data, m_candidates));
		if(m_data[m_curr].size()==m_size) {
			m_curr++;
		}
	}

	bool needToSolve() {
		return false;
	}
	int remainToSolve() {
		int count = 0;
		for (int i=0; i<m_size; ++i) {
			for (int j=0; j<m_size; ++j) {
				if(m_data.at(i).at(j).isComplete()==false) count++;
			}
		}
		return count;
	}
	int size() const {
		return m_size;
	}

	set_char getRowSetSolved(int row) {
		set_char ret_set;
		for(int i=0; i<m_size; i++) {
			if(m_data.at(row).at(i).isComplete()) {
				ret_set.insert( m_data.at(row).at(i).getFirstElement() );
			}
		}
		return ret_set;
	}
	void removeRowAlreadyUsed(int row, const set_char& used_set) {
		for(int i=0; i<m_size; i++) {
			if(m_data.at(row).at(i).isComplete()==false) {
				m_data.at(row).at(i).remove(used_set);
			}
		}
	}

	set_char getColSetSolved(int col) {
		set_char ret_set;
		for(int i=0; i<m_size; i++) {
			if(m_data.at(i).at(col).isComplete()) {
				ret_set.insert( m_data.at(i).at(col).getFirstElement() );
			}
		}
		return ret_set;
	}
	void removeColAlreadyUsed(int col, const set_char& used_set) {
		for(int i=0; i<m_size; i++) {
			if(m_data.at(i).at(col).isComplete()==false) {
				m_data.at(i).at(col).remove(used_set);
			}
		}
	}
	void removeData() {
		m_curr = 0;
		for(int row=0; row<m_size; row++) {
			m_data.at(row).clear();
		}
	}

	char getDataIfComplete(int x, int y, char default_value) {
		if(m_data.at(x).at(y).isComplete()) {
			return m_data.at(x).at(y).getFirstElement();
		} else {
			return default_value;
		}
	}
private:
	int m_size, m_curr;
	vector2D_SudokuElem m_data;
	set_char m_candidates;
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
	clearData();	// 재 로딩시 데이터 클리어
	std::ifstream is(file, std::ios::in);
	if (is)
	{
		std::string line;
		int i=0;
		while(getline(is, line))
		{
			if (i>=m_size) return false;	// 파일 포맷 오류 대비

			if(!parseLine(line.c_str())) return false;
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

void SudokuLoader::clearData()
{
	m_data->removeData();
}

bool SudokuLoader::parseLine(const std::string& line)
{
	int i=0;
	for (str_const_iter pos=line.begin(); pos!=line.end(); ++pos)
	{
		if (i>=m_size) return false;	// 파일 포맷 오류 대비

		m_data->push_data(*pos);

		i++;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////

SudokuSolver::SudokuSolver(Sudoku* sudoku)
:m_data(sudoku)
{
}

bool SudokuSolver::solve()
{
	int remainCnt, remainCntBefore, size;
	size = m_data->size();

	remainCntBefore = 0;
	while( (remainCnt = m_data->remainToSolve()) ) {	// 해결할 개수가 0이면 종료
		if (remainCntBefore == remainCnt) break;	// 문제해결이 되지 않으면

		// 가로방향 후보들 제외
		for (int r=0; r<size; ++r) {
			set_char exist = m_data->getRowSetSolved(r);
			m_data->removeRowAlreadyUsed(r, exist);
		}
		// 세로방향 후보들 제외
		for (int r=0; r<size; ++r) {
			set_char exist = m_data->getColSetSolved(r);
			m_data->removeColAlreadyUsed(r, exist);
		}
		// TODO:
		// 셀 안 후보들 제외
		// for ()
		remainCntBefore = remainCnt;
	}

	return (remainCnt==0);	// 모든 문제가 풀려있는지 여부
}

//////////////////////////////////////////////////////////////////////////

SudokuDisplayer::SudokuDisplayer(Sudoku* data)
{
	m_data = data;
}

char SudokuDisplayer::getData(int x, int y, char default_value)
{
	return m_data->getDataIfComplete(x, y, default_value);
}
