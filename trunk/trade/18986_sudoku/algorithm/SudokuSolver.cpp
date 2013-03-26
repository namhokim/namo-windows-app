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

// ������ �����ϴ� ��
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

// ������ ������(SudokuElem�� ����)
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

	// �ش� ���� �Ϸ�Ǿ����� �����͸� �����´�.
	// �̿Ϸ�ÿ��� �⺻ ���� ��ȯ
	char getDataIfComplete(int x, int y, char default_value) {
		if(m_data.at(x).at(y).isComplete()) {
			return m_data.at(x).at(y).getFirstElement();
		} else {
			return default_value;
		}
	}

	// �ùٸ� ������ ������ �Ǵ�
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
	clearData();	// �� �ε��� ������ Ŭ����
	std::ifstream is(file, std::ios::in);
	if (is)
	{
		std::string line;
		int i=0;
		while(getline(is, line))
		{
			if (i>=m_size) return false;	// ���� ���� ���� ���

			if(!parseLine(line.c_str())) return false;
			i++;
		}
		is.close();	// ��������� �ݾ����� �ʾƵ� �ڵ����� ����
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
		if (i>=m_size) return false;	// ���� ���� ���� ���

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
	while( (remainCnt = m_data->remainToSolve()) ) {	// �ذ��� ������ 0�̸� ����
		if (remainCntBefore == remainCnt) break;	// �����ذ��� ���� ������

		// ���ι��� �ĺ��� ����
		for (int row=0; row<size; ++row) {
			m_data->removeRowAlreadyUsed(row);
		}
		// ���ι��� �ĺ��� ����
		for (int col=0; col<size; ++col) {
			m_data->removeColAlreadyUsed(col);
		}
		// TODO:
		// �� �� �ĺ��� ����
		// for ()
		remainCntBefore = remainCnt;
	}

	return (remainCnt==0);	// ��� ������ Ǯ���ִ��� ����
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

	// value�� ��ȿ�� ������ ��(1~MAX_MAZE)�ΰ�?
	if(m_data->isValidValue(value)==false) return false;

	// �̹� ä���� ���� ���� �������� �ϳ�?
	if(m_data->getDataIfComplete(x, y, NG) != NG) return false;

	// ���ο��� �ߺ��� ���� �ֳ�?
	set_char row_set = m_data->getRowSetSolved(x);
	if(row_set.find(value)!=row_set.end()) return false;

	// ���ο��� �ߺ��� ���� �ֳ�?
	set_char col_set = m_data->getColSetSolved(y);
	if(col_set.find(value)!=col_set.end()) return false;

	// ���ȿ� �ߺ��� ���� �ֳ�?
	set_char cell_set = m_data->getCellSetSolved(x, y);
	if(cell_set.find(value)!=cell_set.end()) return false;

	// �����ϴ�
	return true;
}

bool SudokuPlayer::play(int x, int y, char value)
{
	bool bRes = determinePlay(x,y,value);
	if (bRes) {
		// TODO: �� ����
	}
	return bRes;
}
