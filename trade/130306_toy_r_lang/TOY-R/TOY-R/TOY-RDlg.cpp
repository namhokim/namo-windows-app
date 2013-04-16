
// TOY-RDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "TOY-R.h"
#include "TOY-RDlg.h"
#include "../../portfix_to_prefix/ToyTokenizer.h"
#include <sstream>
#include <vector>
#include <string>
#include <iterator>

using namespace std;

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
	m_convert_content.Empty();
	m_result.SetWindowText(_T(""));
	m_imc.clear();
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

void CTOYRDlg::ConvertToPrefix()
{
	// ���α׷� �о�ͼ�
	CString prog;
	m_program.GetWindowText(prog);
	if (prog.GetLength()==0)
	{
		MessageBox(_T("������ �����ϴ�."));
		m_program.SetFocus();
		return;
	}

	// ��ȯ�Ͽ�
	string prefix;
	if(postfix_to_prefix((CStringA)prog, prefix))
	{
		// �⺻ ���ڿ��� �ٿ���
		m_convert_content.Append((CString)prefix.c_str());
		m_convert_content.Append(_T("\r\n"));

		// ȭ�鿡 �ݿ��Ѵ�
		m_convert.SetWindowText(m_convert_content);
	} else {
		MessageBox(_T("����"));
	}
}

void CTOYRDlg::CreateIMCode()
{
	// ���α׷� �о�ͼ�
	CString prog;
	m_program.GetWindowText(prog);
	if (prog.GetLength()==0)
	{
		MessageBox(_T("������ �����ϴ�."));
		m_program.SetFocus();
		return;
	}

	// �߰��ڵ带 �����Ѵ� (out_imc)
	if(make_im_code((CStringA)prog, m_imc)) {
		stringstream ss;
		copy(m_imc.begin(), m_imc.end(), ostream_iterator<string>(ss, "\r\n"));

		// ȭ�� ���
		m_convert_content.Append((CString)ss.str().c_str());
		m_convert_content.Append(_T("\r\n"));

		// ȭ�鿡 �ݿ��Ѵ�
		m_convert.SetWindowText(m_convert_content);
	} else {
		MessageBox(_T("�߰��ڵ� ���� ����"));
	}
}

void CTOYRDlg::SaveTheIMCode()
{
	if (m_imc.empty())
	{
		MessageBox(_T("������ �߰��ڵ尡 �����ϴ�."));
		return;
	}

	// ���Ϸ� ���� ���̾�α�
	TCHAR szFilter[] = _T("�ؽ�Ʈ ����(*.txt)|*.txt| All Files(*.*)|*.*||");
	CFileDialog open(FALSE, _T("txt"), _T("save.txt"), OFN_OVERWRITEPROMPT,
		szFilter);
	if(open.DoModal() == IDOK)
	{
		// ���ڿ��� ��ȯ
		stringstream ss;
		copy(m_imc.begin(), m_imc.end(), ostream_iterator<string>(ss, "\r\n"));
		CString data = (CString)ss.str().c_str();

		// ���Ͽ� ���
		CStdioFile fp;
		CString m_Buffer;
		CString m_TempBuffer;

		if(!fp.Open(open.m_ofn.lpstrFile, CFile::modeCreate | CFile::modeWrite)) {
			MessageBox(_T("���� ���⿡ ����"));
			return;
		} else {
			fp.WriteString(data);
		}

		fp.Close();
	}
}

void CTOYRDlg::LoadTheIMCode()
{
	TCHAR szFilter[] = _T("�ؽ�Ʈ ����(*.txt)|*.txt| All Files(*.*)|*.*||");
	CFileDialog open(TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY,
		szFilter);
	if(open.DoModal() == IDOK)
	{
		CStdioFile fp;
		CString line;

		if(!fp.Open(open.m_ofn.lpstrFile, CFile::modeRead)) {
			MessageBox(_T("���� ���⿡ ����"));
			return;
		}

		m_imc.clear();
		while (!feof (fp.m_pStream))
		{
			if(fp.ReadString( line ))
			{
				string l( (CStringA)line );

				// remove line-feed
				string::size_type pos = 0; // Must initialize
				while ( ( pos = l.find ("\r",pos) ) != string::npos )
				{
					l.erase ( pos, 1 );
				}
				m_imc.push_back(l);
			}
		}

		fp.Close();

		// ���ڿ��� ��ȯ
		stringstream ss;
		copy(m_imc.begin(), m_imc.end(), ostream_iterator<string>(ss, "\r\n"));
		CString data = (CString)ss.str().c_str();

		// ȭ�鿡 �߰�
		m_convert_content.Append(data);
		m_convert.SetWindowText(m_convert_content);
	}
}

void CTOYRDlg::Evaluation()
{
	if (m_imc.empty())
	{
		MessageBox(_T("�߰� �ڵ尡 �����ϴ�."));
		return;
	}

	int ev;
	if(evaluation(m_imc, ev)) {
		CString r;
		r.Format(_T("%d"), ev);
		m_result.SetWindowText(r);
	} else {
		m_result.SetWindowText(_T("Undefined"));
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
	ConvertToPrefix();
}

void CTOYRDlg::OnBnClickedButtonConvert()
{
	CreateIMCode();
}

void CTOYRDlg::OnBnClickedButtonSave()
{
	SaveTheIMCode();
}

void CTOYRDlg::OnBnClickedButtonLoadIc()
{
	LoadTheIMCode();
}

void CTOYRDlg::OnBnClickedButtonCalc()
{
	Evaluation();
}

void CTOYRDlg::OnBnClickedButtonQuit()
{
	EndDialog(0);
}
