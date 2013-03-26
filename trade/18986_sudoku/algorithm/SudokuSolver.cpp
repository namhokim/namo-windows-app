#include "StdAfx.h"
#include "SudokuSolver.h"
#include <string>		// for 파일입출력시 문자열버퍼
#include <fstream>		// for 파일입출력
#include <vector>		// for std::vector
#include <set>			// for std::set
#include <stack>		// for std::stack
#include <algorithm>	// for set_difference
#include <iterator>		// for std::inserter (vs2010 error, if not exist)
#include <cmath>		// for sqrt

class SudokuElem;	// 타입정의(typedef)를 위한 전방선언
struct SudoKuRecoder;// 타입정의(typedef)를 위한 전방선언
typedef std::string::size_type str_size;
typedef std::string::iterator str_iter;
typedef std::string::const_iterator str_const_iter;
typedef std::set<char> set_char;
typedef std::stack<SudoKuRecoder> vec_recoder;
typedef std::vector< SudokuElem > vector_SudokuElem;
typedef std::vector< vector_SudokuElem > vector2D_SudokuElem;

//////////////////////////////////////////////////////////////////////////

// 완전제곱수(1,4,9,16...)인지 판별하는 함수
bool IsPerfectSquare(int number)
{
	double d_sqrt = sqrt(double(number));
	int i_sqrt = static_cast<int>(d_sqrt);
	return (d_sqrt == i_sqrt);
}
// 정수형의 제곱갑 구하기(예.4->2, 9->3...)
inline int Sqrt(int number)
{
	return static_cast<int>(sqrt(double(number)));
}

const int NotSet = (-1);	// 설정이 안되었음을 나타내는 상수
const char BlankValue = 'B';// 텍스트 데이터에서 빈값을 나타내는 상수

//////////////////////////////////////////////////////////////////////////

// 스도쿠를 구성하는 원소
class SudokuElem
{
public:
	// 생성자
	SudokuElem(char value, const set_char candidates)
	{
		if(value==BlankValue) {
			m_candidates = candidates;
		} else {
			m_candidates.insert(value);
		}
	}
	// 숫자가 지정되었는지 판별
	bool isComplete() const {
		return (m_candidates.size() == 1);
	}
	// 첫번째 원소의 값을 구함
	char getFirstElement() {
		return *(m_candidates.begin());
	}
	// 값(집합) 획득
	set_char getData() {
		return m_candidates;
	}
	// 특정값으로 설정
	void setData(char value) {
		m_candidates.clear();
		m_candidates.insert(value);
	}
	// 특정값으로 설정
	void setData(set_char value_set) {
		//m_candidates.clear();	// CHECK
		m_candidates = value_set;
	}
	// 후보들을 제거
	void remove(char value) {
		m_candidates.erase(value);
	}
	// 후보값들을 제거(집합연산:차집합)
	void remove(const set_char& used_set) {
		set_char result;
		std::set_difference(m_candidates.begin(), m_candidates.end(),
			used_set.begin(), used_set.end(), std::inserter(result, result.end()));
		m_candidates.clear();
		m_candidates.insert(result.begin(), result.end());
	}
private:
	set_char m_candidates;	// 값 저장 맴버변수
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

// 스도쿠 데이터(SudokuElem를 포함)
class Sudoku
{
public:
	// 생성자: 스도쿠 크기를 인자로 받음
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
	// 값을 추가
	void push_data(char data) {
		m_data[m_curr].push_back(SudokuElem(data, m_candidates));
		if(m_data[m_curr].size()==m_size) {
			m_curr++;
		}
	}
	// 스도쿠 데이터가 필요한 개수를 반환(0이면 모든 숫자가 채워짐)
	int remainToSolve() {
		int count = 0;
		for (int i=0; i<m_size; ++i) {
			for (int j=0; j<m_size; ++j) {
				if(m_data.at(i).at(j).isComplete()==false) count++;
			}
		}
		return count;
	}
	// 스도크 크기를 구함(생성자에서 설정)
	int size() const {
		return m_size;
	}
	// 스도크의 셀 단위 크기를 구함(생성자에서 설정)
	int cellUnit() const {
		return m_cellUnit;
	}


	//////////////////////////////////////////////////////////////////////////
	// 완료된 집합을 구하는 메소드들

	// 완료된 행(row)의 집합을 구함
	set_char getRowSetSolved(int row) {
		set_char ret_set;
		for(int i=0; i<m_size; i++) {
			if(m_data.at(row).at(i).isComplete()) {
				ret_set.insert( m_data.at(row).at(i).getFirstElement() );
			}
		}
		return ret_set;
	}

	// 완료된 열(column)의 집합을 구함
	set_char getColSetSolved(int col) {
		set_char ret_set;
		for(int i=0; i<m_size; i++) {
			if(m_data.at(i).at(col).isComplete()) {
				ret_set.insert( m_data.at(i).at(col).getFirstElement() );
			}
		}
		return ret_set;
	}

	// 완료된 셀(cell)의 집합을 구함
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
	// 열의 값 제거

	// 특정행(인자)에서 설정되지 않은 칸의 중복된 값을 제거
	void removeRowAlreadyUsed(int row) {
		set_char exist = this->getRowSetSolved(row);
		for(int i=0; i<m_size; i++) {
			if(m_data.at(row).at(i).isComplete()==false) {
				m_data.at(row).at(i).remove(exist);
			}
		}
	}

	// 특정열(인자)에서 설정되지 않은 칸의 중복된 값을 제거
	void removeColAlreadyUsed(int col) {
		set_char exist = this->getColSetSolved(col);
		for(int i=0; i<m_size; i++) {
			if(m_data.at(i).at(col).isComplete()==false) {
				m_data.at(i).at(col).remove(exist);
			}
		}
	}

	// 특정셀(인자:셀의 좌측상단)에서 설정되지 않은 칸의 중복된 값을 제거
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

	// 데이터 제거
	void removeData() {
		m_curr = 0;
		for(int row=0; row<m_size; row++) {
			m_data.at(row).clear();
		}
		while(!m_record.empty()) m_record.pop();
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

	// 값 설정(레코드에 기록됨)
	void setData(int x, int y, char value) {
		m_record.push(SudoKuRecoder(x,y,m_data.at(x).at(y).getData()));
		m_data.at(x).at(y).setData(value);
	}

	// 올바른 범위의 값인지 판단
	bool isValidValue(char value) {
		bool bRes = ( ('1'<=value) && (value<=MAX_VALUE) );
		return bRes;
	}

	// 무르기 할 데이터가 있는지 판단
	bool hasUndoData() const {
		return (m_record.empty()==false);	// 비어있지 않다면 있는것
	}
	// 무르기
	void undoData() {
		SudoKuRecoder undoData = m_record.top();
		m_data.at(undoData.x).at(undoData.y).setData(undoData.value);
		m_record.pop();
	}

private:
	int m_size;		// 스도쿠의 크기
	int m_cellUnit;	// 스도쿠의 셀의 크기
	int m_curr;		// 스도쿠에 입력된 현재 값(push_back과 연관)
	vec_recoder m_record;	// 게임을 기록
	
	const char MAX_VALUE;		// 스도쿠에서 최대값(예. 4->'4')
	vector2D_SudokuElem m_data;	// 데이터
	set_char m_candidates;		// 기본 후보 집합(예. 4->{'1','2','3','4'}

	// 특정 위치의 셀의 영역을 구한다
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
// SudokuLoader 클래스의 구현

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

	// 그렇지 않다면 값 입력이 가능
	return true;
}

bool SudokuPlayer::play(int x, int y, char value)
{
	bool bRes = determinePlay(x,y,value);
	if (bRes) {
		m_data->setData(x, y, value);	// 값을 설정
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
	while( (remainCnt = m_data->remainToSolve()) ) {// 해결할 개수가 0이면 종료
		if (remainCntBefore == remainCnt) break;	// 문제해결이 되지 않으면

		// 가로방향 후보들 제외
		for (int row=0; row<size; ++row) m_data->removeRowAlreadyUsed(row);

		// 세로방향 후보들 제외
		for (int col=0; col<size; ++col) m_data->removeColAlreadyUsed(col);

		// 셀 안 후보들 제외
		for (int i=0; i<cellUnit; ++i) {
			for (int j=0; j<cellUnit; ++j) {
				m_data->removeCellAlreadyUsed(i*cellUnit, j*cellUnit);
			}
		}

		// 이전 남아있는 개수를 갱신
		remainCntBefore = remainCnt;
	}

	return (remainCnt==0);	// 모든 문제가 풀려있는지 여부
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