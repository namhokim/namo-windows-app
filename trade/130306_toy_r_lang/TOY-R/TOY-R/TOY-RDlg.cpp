
// TOY-RDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "TOY-R.h"
#include "TOY-RDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTOYRDlg ��ȭ ����




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
	TCHAR szFilter[] = _T("�ؽ�Ʈ ����(*.txt)|*.txt| All Files(*.*)|*.*||");
	CFileDialog open(TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY,
		szFilter);
	if(open.DoModal() == IDOK)
	{
		CStdioFile fp;
		CString m_Buffer;
		CString m_TempBuffer;

		if(!fp.Open(open.m_ofn.lpstrFile, CFile::modeRead)) {
			MessageBox(_T("���� ���⿡ ����"));
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


// CTOYRDlg �޽��� ó����

BOOL CTOYRDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CTOYRDlg::OnPaint()
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CTOYRDlg::OnBnClickedButtonConvert()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CTOYRDlg::OnBnClickedButtonSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CTOYRDlg::OnBnClickedButtonLoadIc()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CTOYRDlg::OnBnClickedButtonCalc()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}

void CTOYRDlg::OnBnClickedButtonQuit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}
