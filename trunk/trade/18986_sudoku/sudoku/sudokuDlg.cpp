
// sudokuDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "sudoku.h"
#include "sudokuDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsudokuDlg 대화 상자




CsudokuDlg::CsudokuDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsudokuDlg::IDD, pParent)
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
}

BEGIN_MESSAGE_MAP(CsudokuDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CsudokuDlg::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CsudokuDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


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

BOOL CsudokuDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_hAccelTable!=NULL)
	{
		if(TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CsudokuDlg::LoadFromFile(LPCTSTR file)
{
	std::ifstream is (file, std::ios::in);
	if (is)
	{
		std::string line;
		int i=0;
		while(getline(is, line))
		{
			if (i>=MAZE_SIZE) break;	// 파일 포맷 오류 대비
			CString v1,v2,v3,v4;
			ParseLine(line.c_str(),v1,v2,v3,v4);
			SetButtonValue(btn[i][0], v1);
			SetButtonValue(btn[i][1], v2);
			SetButtonValue(btn[i][2], v3);
			SetButtonValue(btn[i][3], v4);
			i++;
		}
		is.close();
	}
}

void CsudokuDlg::ParseLine(LPCSTR line,CString& v1,CString& v2,CString& v3,CString& v4)
{
	if (line[0]!='B') v1=line[0];
	if (line[1]!='B') v2=line[1];
	if (line[2]!='B') v3=line[2];
	if (line[3]!='B') v4=line[3];
}

void CsudokuDlg::SetButtonValue(CButton&button, const CString& value)
{
	if (value.GetLength()!=0)
	{
		button.EnableWindow(FALSE);
		button.SetWindowText(value);
	}
	else
	{
		button.EnableWindow(TRUE);
		button.SetWindowText(_T(""));
	}
}

void CsudokuDlg::ClearButtonValues()
{
	CString emptyStr;
	for(int i=0; i<MAZE_SIZE; i++)
	{
		for(int j=0; j<MAZE_SIZE; j++)
		{
			SetButtonValue(btn[i][j], emptyStr);
		}
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