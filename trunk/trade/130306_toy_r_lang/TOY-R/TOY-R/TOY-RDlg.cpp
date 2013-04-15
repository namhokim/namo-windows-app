
// TOY-RDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TOY-R.h"
#include "TOY-RDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTOYRDlg 대화 상자




CTOYRDlg::CTOYRDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTOYRDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTOYRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PROG, m_program);
	DDX_Control(pDX, IDC_EDIT_CONVERT, m_convert);
	DDX_Control(pDX, IDC_EDIT_RESULT, m_result);
}

//////////////////////////////////////////////////////////////////////////
// private

void CTOYRDlg::ClearEditControl()
{
	m_program.SetWindowText(_T(""));
	m_convert.SetWindowText(_T(""));
	m_result.SetWindowText(_T(""));
}

void CTOYRDlg::LoadFromFile()
{
	TCHAR szFilter[] = _T("텍스트 문서(*.txt)|*.txt| All Files(*.*)|*.*||");
	CFileDialog open(TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY,
		szFilter);
	if(open.DoModal() == IDOK)
	{
		CStdioFile fp;
		CString m_Buffer;
		CString m_TempBuffer;

		if(!fp.Open(open.m_ofn.lpstrFile, CFile::modeRead)) {
			MessageBox(_T("파일 열기에 실패"));
			return;
		}

		while (!feof (fp.m_pStream))
		{
			fp.ReadString( m_TempBuffer );
			m_Buffer += m_TempBuffer;
			m_Buffer += "\r\n";
		}

		fp.Close();

		m_program.SetWindowText(m_Buffer);
	}
}

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTOYRDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CTOYRDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_PROG, &CTOYRDlg::OnBnClickedButtonLoadProg)
	ON_BN_CLICKED(IDC_BUTTON_PREFIX, &CTOYRDlg::OnBnClickedButtonPrefix)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, &CTOYRDlg::OnBnClickedButtonConvert)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTOYRDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_IC, &CTOYRDlg::OnBnClickedButtonLoadIc)
	ON_BN_CLICKED(IDC_BUTTON_CALC, &CTOYRDlg::OnBnClickedButtonCalc)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CTOYRDlg::OnBnClickedButtonQuit)
END_MESSAGE_MAP()


// CTOYRDlg 메시지 처리기

BOOL CTOYRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTOYRDlg::OnPaint()
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
HCURSOR CTOYRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTOYRDlg::OnBnClickedButtonClear()
{
	ClearEditControl();
}

void CTOYRDlg::OnBnClickedButtonLoadProg()
{
	LoadFromFile();
}

void CTOYRDlg::OnBnClickedButtonPrefix()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTOYRDlg::OnBnClickedButtonConvert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTOYRDlg::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTOYRDlg::OnBnClickedButtonLoadIc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTOYRDlg::OnBnClickedButtonCalc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CTOYRDlg::OnBnClickedButtonQuit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
