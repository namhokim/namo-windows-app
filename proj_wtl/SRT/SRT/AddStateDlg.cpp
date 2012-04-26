#include "StdAfx.h"
#include "AddStateDlg.h"

#define RADIO_CURRENT_TIME		0
#define RADIO_SPECIFIC_TIME		1

CAddStateDlg::CAddStateDlg(CSRTView *view)
:m_view_ref(view)
{
}

LRESULT CAddStateDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	// 다이얼로그 아이템(컨트롤)을 지역 변수에 기록
	m_radioCurr	= GetDlgItem(IDC_RADIO_CURR_TIME);
	m_radioSpec	= GetDlgItem(IDC_RADIO_SPEC_TIME);
	m_editJob	= GetDlgItem(IDC_EDIT_JOB);
	m_datePicker= GetDlgItem(IDC_DATETIMEPICKER);
	m_datePicker.SetFont(AtlGetDefaultGuiFont());

	// 라디오 버튼 그룹 지정 및 '현재시간'을 기본적으로 선택
	// 주의 : IDC_RADIO_CURR_TIME는 WS_GROUP 속성이 설정되어 있다(그룹의 선두)
	CheckRadioButton(IDC_RADIO_CURR_TIME, IDC_RADIO_SPEC_TIME, IDC_RADIO_CURR_TIME);
	m_editJob.SetFocus();				// '작업' 입력란에 포커싱

	return FALSE;						// 포커싱을 위해 FALSE를 반환해야 함
}

LRESULT CAddStateDlg::OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CAddStateDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// 유효성 검증
	DoDataExchange(TRUE, IDC_EDIT_JOB);
	if(m_job.GetLength()<=0) {
		// TODO : 리소스에서 메시지 가져오도록
		// TODO : 위치 조정 못하나?
		MessageBox(TEXT("작업명을 입력하세요"), TEXT("알림"), MB_OK);
		m_editJob.SetFocus();
		return 0;
	}
	DoDataExchange(TRUE, IDC_EDIT_GROUP);
	if(m_group.GetLength()<=0)	m_group = TEXT("-");	// 그룹이 빠져있을 때는 dash로

	// 날짜 정보
	SYSTEMTIME st;
	DoDataExchange(TRUE, IDC_RADIO_CURR_TIME);
	switch(m_radio) {
		default:
		case RADIO_CURRENT_TIME:	// 현재시간
			::GetLocalTime(&st);			
			break;
		case RADIO_SPECIFIC_TIME:	// 특정시간 : 피커 데이트			
			m_datePicker.GetSystemTime(&st);// 세팅되어 있는 시간
			break;
	}	

	// 리스트뷰에 입력 & 파일저장
	bool isDateChanged = m_view_ref->InsertOneRecord(m_job, m_group, &st);

	EndDialog(wID);
	return 0;
}
LRESULT CAddStateDlg::OnDtnDatetimechangeDatetimepicker(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	//LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	
	// 데이트 피커가 변경되면, 라디오 버튼 선택 변경
	if(m_radioSpec.GetCheck() == BST_UNCHECKED) {
		CheckRadioButton(IDC_RADIO_CURR_TIME, IDC_RADIO_SPEC_TIME, IDC_RADIO_SPEC_TIME);
	}

	return 0;
}
