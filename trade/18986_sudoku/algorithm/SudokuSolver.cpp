#include "StdAfx.h"
#include "SudokuSolver.h"
#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>	// for set_difference
#include <iterator>		// for std::inserter (vs2010 error, if not exist)
#include <iostream>

typedef std::string::size_type str_size;
typedef std::string::iterator str_iter;
typedef std::string::const_iterator str_const_iter;
typedef std::set<char> set_char;

//////////////////////////////////////////////////////////////////////////
#include <cmath>		// for sqrt
bool IsPerfectSquare(int number)
{
	double d_sqrt = sqrt(double(number));
	int i_sqrt = static_cast<int>(d_sqrt);
	return (d_sqrt == i_sqrt);
}
inline int Sqrt(int number)
{
	return static_cast<int>(sqrt(double(number)));
}
const int NotSet = (-1);

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
	Sudoku(int size) : m_size(size), m_curr(0), MAX_VALUE('0'+size)
	{
		for (int i=0; i<size; ++i) {
			m_data.push_back(vector_SudokuElem ());
			m_candidates.insert('1'+i);	// '1','2','3'....
		}
		if (IsPerfectSquare(size)) {
			m_cellUnit = Sqrt(size);
		} else {
			m_cellUnit = NotSet;
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

	//////////////////////////////////////////////////////////////////////////
	set_char getRowSetSolved(int row) {
		set_char ret_set;
		for(int i=0; i<m_size; i++) {
			if(m_data.at(row).at(i).isComplete()) {
				ret_set.insert( m_data.at(row).at(i).getFirstElement() );
			}
		}
		return ret_set;
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

	set_char getCellSetSolved(int x, int y) {
		set_char ret_set;
		int x1, x2, y1, y2;
		if(getCellRange(x,y, x1, x2, y1, y2)) {
			for (int i=x1; i<x2; i++) {
				for (int j=y1; j<y2; j++) {
					if(m_data.at(i).at(j).isComplete()) {
						ret_set.insert( m_data.at(i).at(j).getFirstElement() );
					}
				}
			}
		}
		return ret_set;
	}

	//////////////////////////////////////////////////////////////////////////
	void removeRowAlreadyUsed(int row) {
		set_char exist = this->getRowSetSolved(row);
		for(int i=0; i<m_size; i++) {
			if(m_data.at(row).at(i).isComplete()==false) {
				m_data.at(row).at(i).remove(exist);
			}
		}
	}
	void removeColAlreadyUsed(int col) {
		set_char exist = this->getColSetSolved(col);
		for(int i=0; i<m_size; i++) {
			if(m_data.at(i).at(col).isComplete()==false) {
				m_data.at(i).at(col).remove(exist);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void removeData() {
		m_curr = 0;
		for(int row=0; row<m_size; row++) {
			m_data.at(row).clear();
		}
	}

	// 해당 셀이 완료되었으면 데이터를 가져온다.
	// 미완료시에는 기본 값을 반환
	char getDataIfComplete(int x, int y, char default_value) {
		if(m_data.at(x).at(y).isComplete()) {
			return m_data.at(x).at(y).getFirstElement();
		} else {
			return default_value;
		}
	}

	// 올바른 범위의 값인지 판단
	bool isValidValue(char value) {
		bool bRes = ( ('1'<=value) && (value<=MAX_VALUE) );
		return bRes;
	}
private:
	int m_size, m_curr, m_cellUnit;
	const char MAX_VALUE;
	vector2D_SudokuElem m_data;
	set_char m_candidates;

	bool getCellRange(int x, int y, int& x1, int& x2, int& y1, int& y2)
	{
		if(m_cellUnit==NotSet) return false;

		x1 = (x/m_cellUnit)*m_cellUnit;
		x2 = x1 + m_cellUnit;
		y1 = (y/m_cellUnit)*m_cellUnit;
		y2 = y1 + m_cellUnit;
		return true;
	}
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
		for (int row=0; row<size; ++row) {
			m_data->removeRowAlreadyUsed(row);
		}
		// 세로방향 후보들 제외
		for (int col=0; col<size; ++col) {
			m_data->removeColAlreadyUsed(col);
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

//////////////////////////////////////////////////////////////////////////

SudokuPlayer::SudokuPlayer(Sudoku* data)
{
	m_data = data;
}

bool SudokuPlayer::determinePlay(int x, int y, char value)
{
	const char NG = 'N';

	// value가 유효한 범위의 값(1~MAX_MAZE)인가?
	if(m_data->isValidValue(value)==false) return false;

	// 이미 채워진 셀에 값을 넣으려고 하나?
	if(m_data->getDataIfComplete(x, y, NG) != NG) return false;

	// 가로열에 중복된 값이 있나?
	set_char row_set = m_data->getRowSetSolved(x);
	if(row_set.find(value)!=row_set.end()) return false;

	// 세로열에 중복된 값이 있나?
	set_char col_set = m_data->getColSetSolved(y);
	if(col_set.find(value)!=col_set.end()) return false;

	// 셀안에 중복된 값이 있나?
	set_char cell_set = m_data->getCellSetSolved(x, y);
	if(cell_set.find(value)!=cell_set.end()) return false;

	// 가능하다
	return true;
}

bool SudokuPlayer::play(int x, int y, char value)
{
	bool bRes = determinePlay(x,y,value);
	if (bRes) {
		// TODO: 값 변경
	}
	return bRes;
}
