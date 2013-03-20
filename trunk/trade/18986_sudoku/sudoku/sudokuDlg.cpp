
// sudokuDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "sudoku.h"
#include "sudokuDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CsudokuDlg ��ȭ ����




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
			if (i>=MAZE_SIZE) break;	// ���� ���� ���� ���
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
	TCHAR szFilter[] = _T("�ؽ�Ʈ ����(*.txt)|*.txt| All Files(*.*)|*.*||");
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