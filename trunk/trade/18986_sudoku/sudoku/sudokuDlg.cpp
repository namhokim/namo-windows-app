
// sudokuDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "sudoku.h"
#include "sudokuDlg.h"
#include <Strsafe.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const char EmptyChar = ' ';


// CsudokuDlg ��ȭ ����

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
}

BEGIN_MESSAGE_MAP(CsudokuDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CsudokuDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CsudokuDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CsudokuDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_RADIO_MODE_AUTO, &CsudokuDlg::OnChangeRadioMode)
	ON_BN_CLICKED(IDC_RADIO_MODE_PLAYER, &CsudokuDlg::OnChangeRadioMode)
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

	if (m_loader.load(file)) {
		m_data = m_loader.data();

		// UI�� ������ ǥ��
		DisplayToUI(m_data);

		// �ذ�� ����
		SudokuSolver solver(m_data);
		solver.solve();
	} else {
		MessageBox(_T("�����͸� �ε��� �� �����ϴ�."), _T("����"), MB_OK | MB_ICONERROR);
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
}

void CsudokuDlg::SetButtonValue(CButton&button, char value)
{
	if (value==EmptyChar) {
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
	((CButton *)GetDlgItem(IDC_RADIO_MODE_AUTO))->SetCheck(BST_UNCHECKED);
	((CButton *)GetDlgItem(IDC_RADIO_MODE_PLAYER))->SetCheck(BST_CHECKED);
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

// CsudokuDlg �޽��� ó����

BOOL CsudokuDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	InitPlayMode();
	SelectPosition(NOT_SELECTED, NOT_SELECTED);

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}


// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CsudokuDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CsudokuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//////////////////////////////////////////////////////////////////////////
// �߰� ���� �Լ���

// ���̾�α� ��ݿ� ������������(����Ű)�� ó���ϱ� ���� �Լ�
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
// ��ư�� ���� �ڵ鷯 �Լ���

void CsudokuDlg::OnBnClickedButtonNew()
{
	TCHAR szFilter[] = _T("�ؽ�Ʈ ����(*.txt)|*.txt| All Files(*.*)|*.*||");
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
	TCHAR szFilter[] = _T("�ؽ�Ʈ ����(*.txt)|*.txt| All Files(*.*)|*.*||");
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

void CsudokuDlg::OnChangeRadioMode()
{
	switch(GetCheckedRadioButton(IDC_RADIO_MODE_AUTO, IDC_RADIO_MODE_PLAYER))
	{
	case IDC_RADIO_MODE_AUTO:
		m_ButtonUndo.EnableWindow(FALSE);
		DisplayToUI(m_data);	// for test (�ѹ��� �ذ�)
		break;
	case IDC_RADIO_MODE_PLAYER:
		if(m_hasUndo) m_ButtonUndo.EnableWindow(TRUE);
		break;
	default:
		// you have not specified what to do when you select radio X and Y, so specify it here
		break;
	}
}
void CsudokuDlg::OnBnClickedButton1()
{
	if(m_x!=NOT_SELECTED || m_y!=NOT_SELECTED) {
		SetButtonValue(btn[m_x-1][m_y-1], '1');
		SelectPosition(NOT_SELECTED,NOT_SELECTED);
	}
}

void CsudokuDlg::OnBnClickedButton2()
{
	if(m_x!=NOT_SELECTED || m_y!=NOT_SELECTED) {
		SetButtonValue(btn[m_x-1][m_y-1], '2');
		SelectPosition(NOT_SELECTED,NOT_SELECTED);
	}
}

void CsudokuDlg::OnBnClickedButton3()
{
	if(m_x!=NOT_SELECTED || m_y!=NOT_SELECTED) {
		SetButtonValue(btn[m_x-1][m_y-1], '3');
		SelectPosition(NOT_SELECTED,NOT_SELECTED);
	}
}

void CsudokuDlg::OnBnClickedButton4()
{
	if(m_x!=NOT_SELECTED || m_y!=NOT_SELECTED) {
		SetButtonValue(btn[m_x-1][m_y-1], '4');
		SelectPosition(NOT_SELECTED,NOT_SELECTED);
	}
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
