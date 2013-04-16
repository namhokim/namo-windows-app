
// TOY-RDlg.cpp : 구현 파일
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
	m_convert_content.Empty();
	m_result.SetWindowText(_T(""));
	m_imc.clear();
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

void CTOYRDlg::ConvertToPrefix()
{
	// 프로그램 읽어와서
	CString prog;
	m_program.GetWindowText(prog);
	if (prog.GetLength()==0)
	{
		MessageBox(_T("수식이 없습니다."));
		m_program.SetFocus();
		return;
	}

	// 변환하여
	string prefix;
	if(postfix_to_prefix((CStringA)prog, prefix))
	{
		// 기본 문자열에 붙여서
		m_convert_content.Append((CString)prefix.c_str());
		m_convert_content.Append(_T("\r\n"));

		// 화면에 반영한다
		m_convert.SetWindowText(m_convert_content);
	} else {
		MessageBox(_T("실패"));
	}
}

void CTOYRDlg::CreateIMCode()
{
	// 프로그램 읽어와서
	CString prog;
	m_program.GetWindowText(prog);
	if (prog.GetLength()==0)
	{
		MessageBox(_T("수식이 없습니다."));
		m_program.SetFocus();
		return;
	}

	// 중간코드를 생성한다 (out_imc)
	if(make_im_code((CStringA)prog, m_imc)) {
		stringstream ss;
		copy(m_imc.begin(), m_imc.end(), ostream_iterator<string>(ss, "\r\n"));

		// 화면 출력
		m_convert_content.Append((CString)ss.str().c_str());
		m_convert_content.Append(_T("\r\n"));

		// 화면에 반영한다
		m_convert.SetWindowText(m_convert_content);
	} else {
		MessageBox(_T("중간코드 생성 실패"));
	}
}

void CTOYRDlg::SaveTheIMCode()
{
	if (m_imc.empty())
	{
		MessageBox(_T("저장할 중간코드가 없습니다."));
		return;
	}

	// 파일로 저장 다이얼로그
	TCHAR szFilter[] = _T("텍스트 문서(*.txt)|*.txt| All Files(*.*)|*.*||");
	CFileDialog open(FALSE, _T("txt"), _T("save.txt"), OFN_OVERWRITEPROMPT,
		szFilter);
	if(open.DoModal() == IDOK)
	{
		// 문자열로 변환
		stringstream ss;
		copy(m_imc.begin(), m_imc.end(), ostream_iterator<string>(ss, "\r\n"));
		CString data = (CString)ss.str().c_str();

		// 파일에 기록
		CStdioFile fp;
		CString m_Buffer;
		CString m_TempBuffer;

		if(!fp.Open(open.m_ofn.lpstrFile, CFile::modeCreate | CFile::modeWrite)) {
			MessageBox(_T("파일 열기에 실패"));
			return;
		} else {
			fp.WriteString(data);
		}

		fp.Close();
	}
}

void CTOYRDlg::LoadTheIMCode()
{
	TCHAR szFilter[] = _T("텍스트 문서(*.txt)|*.txt| All Files(*.*)|*.*||");
	CFileDialog open(TRUE, _T("txt"), _T("*.txt"), OFN_HIDEREADONLY,
		szFilter);
	if(open.DoModal() == IDOK)
	{
		CStdioFile fp;
		CString line;

		if(!fp.Open(open.m_ofn.lpstrFile, CFile::modeRead)) {
			MessageBox(_T("파일 열기에 실패"));
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

		// 문자열로 변환
		stringstream ss;
		copy(m_imc.begin(), m_imc.end(), ostream_iterator<string>(ss, "\r\n"));
		CString data = (CString)ss.str().c_str();

		// 화면에 추가
		m_convert_content.Append(data);
		m_convert.SetWindowText(m_convert_content);
	}
}

void CTOYRDlg::Evaluation()
{
	if (m_imc.empty())
	{
		MessageBox(_T("중간 코드가 없습니다."));
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
