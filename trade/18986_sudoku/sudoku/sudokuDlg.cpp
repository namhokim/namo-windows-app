
// sudokuDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "sudoku.h"
#include "sudokuDlg.h"
#include <Strsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char EmptyChar = ' ';


// CsudokuDlg 대화 상자

CsudokuDlg::CsudokuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsudokuDlg::IDD, pParent), m_hasUndo(false)
	, m_loader(MAZE_SIZE), m_data(NULL), m_x(NOT_SELECTED), m_y(NOT_SELECTED)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsudokuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON11, btn[0][0]);
	DDX_Control(pDX, IDC_BUTTON12, btn[0][1]);
	DDX_Control(pDX, IDC_BUTTON13, btn[0][2]);
	DDX_Control(pDX, IDC_BUTTON14, btn[0][3]);
	DDX_Control(pDX, IDC_BUTTON21, btn[1][0]);
	DDX_Control(pDX, IDC_BUTTON22, btn[1][1]);
	DDX_Control(pDX, IDC_BUTTON23, btn[1][2]);
	DDX_Control(pDX, IDC_BUTTON24, btn[1][3]);
	DDX_Control(pDX, IDC_BUTTON31, btn[2][0]);
	DDX_Control(pDX, IDC_BUTTON32, btn[2][1]);
	DDX_Control(pDX, IDC_BUTTON33, btn[2][2]);
	DDX_Control(pDX, IDC_BUTTON34, btn[2][3]);
	DDX_Control(pDX, IDC_BUTTON41, btn[3][0]);
	DDX_Control(pDX, IDC_BUTTON42, btn[3][1]);
	DDX_Control(pDX, IDC_BUTTON43, btn[3][2]);
	DDX_Control(pDX, IDC_BUTTON44, btn[3][3]);
	DDX_Control(pDX, IDC_BUTTON_UNDO, m_ButtonUndo);
	DDX_Control(pDX, IDC_BUTTON1, m_Select1);
	DDX_Control(pDX, IDC_BUTTON2, m_Select2);
	DDX_Control(pDX, IDC_BUTTON3, m_Select3);
	DDX_Control(pDX, IDC_BUTTON4, m_Select4);
	DDX_Control(pDX, IDC_STATIC_POSITION, m_postion);
	DDX_Control(pDX, IDC_BUTTON_MODE_AUTO, m_modeAuto);
	DDX_Control(pDX, IDC_BUTTON_MODE_PLAYER, m_modePlayer);
}

BEGIN_MESSAGE_MAP(CsudokuDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CsudokuDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CsudokuDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CsudokuDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON1, &CsudokuDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CsudokuDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CsudokuDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CsudokuDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON11, &CsudokuDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CsudokuDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CsudokuDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CsudokuDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON21, &CsudokuDlg::OnBnClickedButton21)
	ON_BN_CLICKED(IDC_BUTTON22, &CsudokuDlg::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON23, &CsudokuDlg::OnBnClickedButton23)
	ON_BN_CLICKED(IDC_BUTTON24, &CsudokuDlg::OnBnClickedButton24)
	ON_BN_CLICKED(IDC_BUTTON31, &CsudokuDlg::OnBnClickedButton31)
	ON_BN_CLICKED(IDC_BUTTON32, &CsudokuDlg::OnBnClickedButton32)
	ON_BN_CLICKED(IDC_BUTTON33, &CsudokuDlg::OnBnClickedButton33)
	ON_BN_CLICKED(IDC_BUTTON34, &CsudokuDlg::OnBnClickedButton34)
	ON_BN_CLICKED(IDC_BUTTON41, &CsudokuDlg::OnBnClickedButton41)
	ON_BN_CLICKED(IDC_BUTTON42, &CsudokuDlg::OnBnClickedButton42)
	ON_BN_CLICKED(IDC_BUTTON43, &CsudokuDlg::OnBnClickedButton43)
	ON_BN_CLICKED(IDC_BUTTON44, &CsudokuDlg::OnBnClickedButton44)
	ON_BN_CLICKED(IDC_BUTTON_MODE_AUTO, &CsudokuDlg::OnBnClickedButtonModeAuto)
	ON_BN_CLICKED(IDC_BUTTON_MODE_PLAYER, &CsudokuDlg::OnBnClickedButtonModePlayer)
	ON_BN_CLICKED(IDC_BUTTON_UNDO, &CsudokuDlg::OnBnClickedButtonUndo)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
// private:
void CsudokuDlg::ClearButtonValues()
{
	for(int i=0; i<MAZE_SIZE; i++)
	{
		for(int j=0; j<MAZE_SIZE; j++)
		{
			SetButtonValue(btn[i][j], EmptyChar);
		}
	}
}

void CsudokuDlg::LoadFromFile(LPCTSTR file)
{
	InitPlayMode();
	SelectPosition(NOT_SELECTED, NOT_SELECTED);
	m_isPlayerMode = false;

	if (m_loader.load(file)) {
		m_data = m_loader.data();
		DisplayToUI(m_data);	// UI에 데이터 표시
	} else {
		MessageBox(_T("데이터를 로드할 수 없습니다."), _T("에러"), MB_OK | MB_ICONERROR);
	}
}

void CsudokuDlg::DisplayToUI(Sudoku *data)
{
	SudokuDisplayer disp(data);
	for(int i=0; i<MAZE_SIZE; i++) {
		for(int j=0; j<MAZE_SIZE; j++) {
			char ch = disp.getData(i, j, EmptyChar);
			SetButtonValue(btn[i][j], ch);
		}
	}
	SudokuPlayer p(data);
	if(p.hasUndoData()) m_ButtonUndo.EnableWindow(TRUE);
	else m_ButtonUndo.EnableWindow(FALSE);
}

void CsudokuDlg::SetButtonValue(CButton&button, char value)
{
	if (value==EmptyChar && m_isPlayerMode) {
		button.EnableWindow(TRUE);
		button.SetWindowText(_T(""));
	} else {
		button.EnableWindow(FALSE);
		TCHAR text[2] = {value, 0};
		button.SetWindowText(text);
	}
}

void CsudokuDlg::SaveToFile(LPCTSTR file)
{
	std::ofstream os (file, std::ios::out);
	if (os)
	{
		for(int i=0; i<MAZE_SIZE; i++)
		{
			for(int j=0; j<MAZE_SIZE; j++)
			{
				CString value;
				btn[i][j].GetWindowText(value);
				if(value.GetLength()==0) os << 'B';
				else os << (LPCSTR)value.GetString();
			}
			os << std::endl;
		}
	}
}

void CsudokuDlg::InitPlayMode()
{
	m_modeAuto.EnableWindow(TRUE);
	m_modePlayer.EnableWindow(TRUE);
}

void CsudokuDlg::SelectPosition(int x, int y)
{
	if(x==NOT_SELECTED || y==NOT_SELECTED) {
		EnableSelectButton(FALSE);
		m_postion.SetWindowText(_T("(x, y)"));
	} else {
		m_x = x;
		m_y = y;

		EnableSelectButton(TRUE);

		TCHAR msg[10] = {0};
		StringCchPrintf(msg, 10, _T("(%d, %d)"), x, y);
		m_postion.SetWindowText(msg);
	}
}

void CsudokuDlg::EnableSelectButton(BOOL bEnable)
{
	m_Select1.EnableWindow(bEnable);
	m_Select2.EnableWindow(bEnable);
	m_Select3.EnableWindow(bEnable);
	m_Select4.EnableWindow(bEnable);
}

//////////////////////////////////////////////////////////////////////////

// CsudokuDlg 메시지 처리기

BOOL CsudokuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	InitPlayMode();
	SelectPosition(NOT_SELECTED, NOT_SELECTED);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}


// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CsudokuDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CsudokuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//////////////////////////////////////////////////////////////////////////
// 추가 구현 함수들

// 다이얼로그 기반에 엑셀러레이터(단축키)를 처리하기 위한 함수
BOOL CsudokuDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_hAccelTable!=NULL)
	{
		if(TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////////////////////////////
// 버튼에 대한 핸들러 함수들

void CsudokuDlg::OnBnClickedButtonNew()
{
	TCHAR szFilter[] = _T("텍스트 문서(*.txt)|*.txt| All Files(*.*)|*.*||");
	CFileDialog open(TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY,
		szFilter);
	if(open.DoModal() == IDOK)
	{
		ClearButtonValues();
		LoadFromFile(open.m_ofn.lpstrFile);
	}
}



void CsudokuDlg::OnBnClickedButtonSave()
{
	TCHAR szFilter[] = _T("텍스트 문서(*.txt)|*.txt| All Files(*.*)|*.*||");
	CFileDialog open(FALSE, _T("txt"), _T("save.txt"), OFN_OVERWRITEPROMPT,
		szFilter);
	if(open.DoModal() == IDOK)
	{
		SaveToFile(open.m_ofn.lpstrFile);
	}
}

void CsudokuDlg::OnBnClickedButtonQuit()
{
	EndDialog(0);
}


void CsudokuDlg::OnBnClickedButton(char value)
{
	if(m_x!=NOT_SELECTED || m_y!=NOT_SELECTED) {
		int x = m_x-1;
		int y = m_y-1;
		SudokuPlayer p(m_data);
		if(p.play(x, y, value)) {
			DisplayToUI(m_data);
			SelectPosition(NOT_SELECTED,NOT_SELECTED);
		} else {
			MessageBox(_T("올바르지 않은 입력입니다."));
		}
	}
}

void CsudokuDlg::OnBnClickedButton1()
{
	OnBnClickedButton('1');
}

void CsudokuDlg::OnBnClickedButton2()
{
	OnBnClickedButton('2');
}

void CsudokuDlg::OnBnClickedButton3()
{
	OnBnClickedButton('3');
}

void CsudokuDlg::OnBnClickedButton4()
{
	OnBnClickedButton('4');
}


void CsudokuDlg::OnBnClickedButton11()
{
	SelectPosition(1,1);
}

void CsudokuDlg::OnBnClickedButton12()
{
	SelectPosition(1,2);
}

void CsudokuDlg::OnBnClickedButton13()
{
	SelectPosition(1,3);
}

void CsudokuDlg::OnBnClickedButton14()
{
	SelectPosition(1,4);
}

void CsudokuDlg::OnBnClickedButton21()
{
	SelectPosition(2,1);
}

void CsudokuDlg::OnBnClickedButton22()
{
	SelectPosition(2,2);
}

void CsudokuDlg::OnBnClickedButton23()
{
	SelectPosition(2,3);
}

void CsudokuDlg::OnBnClickedButton24()
{
	SelectPosition(2,4);
}

void CsudokuDlg::OnBnClickedButton31()
{
	SelectPosition(3,1);
}

void CsudokuDlg::OnBnClickedButton32()
{
	SelectPosition(3,2);
}

void CsudokuDlg::OnBnClickedButton33()
{
	SelectPosition(3,3);
}

void CsudokuDlg::OnBnClickedButton34()
{
	SelectPosition(3,4);
}

void CsudokuDlg::OnBnClickedButton41()
{
	SelectPosition(4,1);
}

void CsudokuDlg::OnBnClickedButton42()
{
	SelectPosition(4,2);
}

void CsudokuDlg::OnBnClickedButton43()
{
	SelectPosition(4,3);
}

void CsudokuDlg::OnBnClickedButton44()
{
	SelectPosition(4,4);
}

void CsudokuDlg::OnBnClickedButtonModeAuto()
{
	if (m_data==NULL) return;

	m_modePlayer.EnableWindow(FALSE);

	SudokuPlayer player(m_data);
	player.solveAll();
	DisplayToUI(m_data);	// UI에 데이터 표시
}

void CsudokuDlg::OnBnClickedButtonModePlayer()
{
	if (m_data==NULL) return;

	m_modeAuto.EnableWindow(FALSE);
	m_isPlayerMode = true;
	DisplayToUI(m_data);	// UI에 데이터 표시
}

void CsudokuDlg::OnBnClickedButtonUndo()
{
	SudokuPlayer p(m_data);
	p.undo();
	DisplayToUI(m_data);	// UI에 데이터 표시
}

