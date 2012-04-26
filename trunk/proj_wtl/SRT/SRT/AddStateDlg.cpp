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

	// ���̾�α� ������(��Ʈ��)�� ���� ������ ���
	m_radioCurr	= GetDlgItem(IDC_RADIO_CURR_TIME);
	m_radioSpec	= GetDlgItem(IDC_RADIO_SPEC_TIME);
	m_editJob	= GetDlgItem(IDC_EDIT_JOB);
	m_datePicker= GetDlgItem(IDC_DATETIMEPICKER);
	m_datePicker.SetFont(AtlGetDefaultGuiFont());

	// ���� ��ư �׷� ���� �� '����ð�'�� �⺻������ ����
	// ���� : IDC_RADIO_CURR_TIME�� WS_GROUP �Ӽ��� �����Ǿ� �ִ�(�׷��� ����)
	CheckRadioButton(IDC_RADIO_CURR_TIME, IDC_RADIO_SPEC_TIME, IDC_RADIO_CURR_TIME);
	m_editJob.SetFocus();				// '�۾�' �Է¶��� ��Ŀ��

	return FALSE;						// ��Ŀ���� ���� FALSE�� ��ȯ�ؾ� ��
}

LRESULT CAddStateDlg::OnCancelCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CAddStateDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// ��ȿ�� ����
	DoDataExchange(TRUE, IDC_EDIT_JOB);
	if(m_job.GetLength()<=0) {
		// TODO : ���ҽ����� �޽��� ����������
		// TODO : ��ġ ���� ���ϳ�?
		MessageBox(TEXT("�۾����� �Է��ϼ���"), TEXT("�˸�"), MB_OK);
		m_editJob.SetFocus();
		return 0;
	}
	DoDataExchange(TRUE, IDC_EDIT_GROUP);
	if(m_group.GetLength()<=0)	m_group = TEXT("-");	// �׷��� �������� ���� dash��

	// ��¥ ����
	SYSTEMTIME st;
	DoDataExchange(TRUE, IDC_RADIO_CURR_TIME);
	switch(m_radio) {
		default:
		case RADIO_CURRENT_TIME:	// ����ð�
			::GetLocalTime(&st);			
			break;
		case RADIO_SPECIFIC_TIME:	// Ư���ð� : ��Ŀ ����Ʈ			
			m_datePicker.GetSystemTime(&st);// ���õǾ� �ִ� �ð�
			break;
	}	

	// ����Ʈ�信 �Է� & ��������
	bool isDateChanged = m_view_ref->InsertOneRecord(m_job, m_group, &st);

	EndDialog(wID);
	return 0;
}
LRESULT CAddStateDlg::OnDtnDatetimechangeDatetimepicker(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	//LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	
	// ����Ʈ ��Ŀ�� ����Ǹ�, ���� ��ư ���� ����
	if(m_radioSpec.GetCheck() == BST_UNCHECKED) {
		CheckRadioButton(IDC_RADIO_CURR_TIME, IDC_RADIO_SPEC_TIME, IDC_RADIO_SPEC_TIME);
	}

	return 0;
}
