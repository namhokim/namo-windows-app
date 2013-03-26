#include "StdAfx.h"
#include "SudokuSolver.h"
#include <string>		// for ��������½� ���ڿ�����
#include <fstream>		// for ���������
#include <vector>		// for std::vector
#include <set>			// for std::set
#include <stack>		// for std::stack
#include <algorithm>	// for set_difference
#include <iterator>		// for std::inserter (vs2010 error, if not exist)
#include <cmath>		// for sqrt

class SudokuElem;	// Ÿ������(typedef)�� ���� ���漱��
struct SudoKuRecoder;// Ÿ������(typedef)�� ���� ���漱��
typedef std::string::size_type str_size;
typedef std::string::iterator str_iter;
typedef std::string::const_iterator str_const_iter;
typedef std::set<char> set_char;
typedef std::stack<SudoKuRecoder> vec_recoder;
typedef std::vector< SudokuElem > vector_SudokuElem;
typedef std::vector< vector_SudokuElem > vector2D_SudokuElem;

//////////////////////////////////////////////////////////////////////////

// ����������(1,4,9,16...)���� �Ǻ��ϴ� �Լ�
bool IsPerfectSquare(int number)
{
	double d_sqrt = sqrt(double(number));
	int i_sqrt = static_cast<int>(d_sqrt);
	return (d_sqrt == i_sqrt);
}
// �������� ������ ���ϱ�(��.4->2, 9->3...)
inline int Sqrt(int number)
{
	return static_cast<int>(sqrt(double(number)));
}

const int NotSet = (-1);	// ������ �ȵǾ����� ��Ÿ���� ���
const char BlankValue = 'B';// �ؽ�Ʈ �����Ϳ��� ���� ��Ÿ���� ���

//////////////////////////////////////////////////////////////////////////

// ������ �����ϴ� ����
class SudokuElem
{
public:
	// ������
	SudokuElem(char value, const set_char candidates)
	{
		if(value==BlankValue) {
			m_candidates = candidates;
		} else {
			m_candidates.insert(value);
		}
	}
	// ���ڰ� �����Ǿ����� �Ǻ�
	bool isComplete() const {
		return (m_candidates.size() == 1);
	}
	// ù��° ������ ���� ����
	char getFirstElement() {
		return *(m_candidates.begin());
	}
	// ��(����) ȹ��
	set_char getData() {
		return m_candidates;
	}
	// Ư�������� ����
	void setData(char value) {
		m_candidates.clear();
		m_candidates.insert(value);
	}
	// Ư�������� ����
	void setData(set_char value_set) {
		//m_candidates.clear();	// CHECK
		m_candidates = value_set;
	}
	// �ĺ����� ����
	void remove(char value) {
		m_candidates.erase(value);
	}
	// �ĺ������� ����(���տ���:������)
	void remove(const set_char& used_set) {
		set_char result;
		std::set_difference(m_candidates.begin(), m_candidates.end(),
			used_set.begin(), used_set.end(), std::inserter(result, result.end()));
		m_candidates.clear();
		m_candidates.insert(result.begin(), result.end());
	}
private:
	set_char m_candidates;	// �� ���� �ɹ�����
};

struct SudoKuRecoder
{
	SudoKuRecoder() {}
	SudoKuRecoder(int x, int y, set_char value) {
		this->x = x;
		this->y = y;
		this->value = value;
	}
	int x, y;
	set_char value;
};

// ������ ������(SudokuElem�� ����)
class Sudoku
{
public:
	// ������: ������ ũ�⸦ ���ڷ� ����
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
	// ���� �߰�
	void push_data(char data) {
		m_data[m_curr].push_back(SudokuElem(data, m_candidates));
		if(m_data[m_curr].size()==m_size) {
			m_curr++;
		}
	}
	// ������ �����Ͱ� �ʿ��� ������ ��ȯ(0�̸� ��� ���ڰ� ä����)
	int remainToSolve() {
		int count = 0;
		for (int i=0; i<m_size; ++i) {
			for (int j=0; j<m_size; ++j) {
				if(m_data.at(i).at(j).isComplete()==false) count++;
			}
		}
		return count;
	}
	// ����ũ ũ�⸦ ����(�����ڿ��� ����)
	int size() const {
		return m_size;
	}
	// ����ũ�� �� ���� ũ�⸦ ����(�����ڿ��� ����)
	int cellUnit() const {
		return m_cellUnit;
	}


	//////////////////////////////////////////////////////////////////////////
	// �Ϸ�� ������ ���ϴ� �޼ҵ��

	// �Ϸ�� ��(row)�� ������ ����
	set_char getRowSetSolved(int row) {
		set_char ret_set;
		for(int i=0; i<m_size; i++) {
			if(m_data.at(row).at(i).isComplete()) {
				ret_set.insert( m_data.at(row).at(i).getFirstElement() );
			}
		}
		return ret_set;
	}

	// �Ϸ�� ��(column)�� ������ ����
	set_char getColSetSolved(int col) {
		set_char ret_set;
		for(int i=0; i<m_size; i++) {
			if(m_data.at(i).at(col).isComplete()) {
				ret_set.insert( m_data.at(i).at(col).getFirstElement() );
			}
		}
		return ret_set;
	}

	// �Ϸ�� ��(cell)�� ������ ����
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
	// ���� �� ����

	// Ư����(����)���� �������� ���� ĭ�� �ߺ��� ���� ����
	void removeRowAlreadyUsed(int row) {
		set_char exist = this->getRowSetSolved(row);
		for(int i=0; i<m_size; i++) {
			if(m_data.at(row).at(i).isComplete()==false) {
				m_data.at(row).at(i).remove(exist);
			}
		}
	}

	// Ư����(����)���� �������� ���� ĭ�� �ߺ��� ���� ����
	void removeColAlreadyUsed(int col) {
		set_char exist = this->getColSetSolved(col);
		for(int i=0; i<m_size; i++) {
			if(m_data.at(i).at(col).isComplete()==false) {
				m_data.at(i).at(col).remove(exist);
			}
		}
	}

	// Ư����(����:���� �������)���� �������� ���� ĭ�� �ߺ��� ���� ����
	void removeCellAlreadyUsed(int x, int y) {
		set_char exist = this->getCellSetSolved(x, y);
		int x1, x2, y1, y2;
		x1 = x;
		x2 = x + m_cellUnit;
		y1 = y;
		y2 = y + m_cellUnit;
		for(int i=x1; i<x2; i++) {
			for(int j=y1; j<y2; j++) {
				if(m_data.at(i).at(j).isComplete()==false) {
					m_data.at(i).at(j).remove(exist);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	// ������ ����
	void removeData() {
		m_curr = 0;
		for(int row=0; row<m_size; row++) {
			m_data.at(row).clear();
		}
		while(!m_record.empty()) m_record.pop();
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

	// �� ����(���ڵ忡 ��ϵ�)
	void setData(int x, int y, char value) {
		m_record.push(SudoKuRecoder(x,y,m_data.at(x).at(y).getData()));
		m_data.at(x).at(y).setData(value);
	}

	// �ùٸ� ������ ������ �Ǵ�
	bool isValidValue(char value) {
		bool bRes = ( ('1'<=value) && (value<=MAX_VALUE) );
		return bRes;
	}

	// ������ �� �����Ͱ� �ִ��� �Ǵ�
	bool hasUndoData() const {
		return (m_record.empty()==false);	// ������� �ʴٸ� �ִ°�
	}
	// ������
	void undoData() {
		SudoKuRecoder undoData = m_record.top();
		m_data.at(undoData.x).at(undoData.y).setData(undoData.value);
		m_record.pop();
	}

private:
	int m_size;		// �������� ũ��
	int m_cellUnit;	// �������� ���� ũ��
	int m_curr;		// ������ �Էµ� ���� ��(push_back�� ����)
	vec_recoder m_record;	// ������ ���
	
	const char MAX_VALUE;		// �������� �ִ밪(��. 4->'4')
	vector2D_SudokuElem m_data;	// ������
	set_char m_candidates;		// �⺻ �ĺ� ����(��. 4->{'1','2','3','4'}

	// Ư�� ��ġ�� ���� ������ ���Ѵ�
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
// SudokuLoader Ŭ������ ����

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

SudokuPlayer::~SudokuPlayer()
{

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

	// �׷��� �ʴٸ� �� �Է��� ����
	return true;
}

bool SudokuPlayer::play(int x, int y, char value)
{
	bool bRes = determinePlay(x,y,value);
	if (bRes) {
		m_data->setData(x, y, value);	// ���� ����
	}
	return bRes;
}

bool SudokuPlayer::autoPlay()
{
	return true;
}

bool SudokuPlayer::solveAll()
{
	int remainCnt, remainCntBefore, size, cellUnit;
	size = m_data->size();
	cellUnit = m_data->cellUnit();

	remainCntBefore = 0;
	while( (remainCnt = m_data->remainToSolve()) ) {// �ذ��� ������ 0�̸� ����
		if (remainCntBefore == remainCnt) break;	// �����ذ��� ���� ������

		// ���ι��� �ĺ��� ����
		for (int row=0; row<size; ++row) m_data->removeRowAlreadyUsed(row);

		// ���ι��� �ĺ��� ����
		for (int col=0; col<size; ++col) m_data->removeColAlreadyUsed(col);

		// �� �� �ĺ��� ����
		for (int i=0; i<cellUnit; ++i) {
			for (int j=0; j<cellUnit; ++j) {
				m_data->removeCellAlreadyUsed(i*cellUnit, j*cellUnit);
			}
		}

		// ���� �����ִ� ������ ����
		remainCntBefore = remainCnt;
	}

	return (remainCnt==0);	// ��� ������ Ǯ���ִ��� ����
}

bool SudokuPlayer::hasUndoData()
{
	return m_data->hasUndoData();
}

bool SudokuPlayer::undo()
{
	bool bRes = hasUndoData();
	if(bRes) {
		m_data->undoData();
	}
	return bRes;
}