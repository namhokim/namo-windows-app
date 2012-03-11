/* $Id: DemoDlg.cpp,v 1.3 2005/08/16 17:15:09 pyabo Exp $
 *
 * Author: Pablo Yabo (pablo.yabo@nektra.com)
 *
 * Copyright (c) 2004-2006 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved.
 *
 * You may modify and/or integrate this code into your commercial software
 * keeping this notice.
 *
 **/

#if !defined(AFX_MESSAGETREEDLG_H__FA8C97EB_23EB_4A80_BA8E_B852F9A19CEB__INCLUDED_)
#define AFX_MESSAGETREEDLG_H__FA8C97EB_23EB_4A80_BA8E_B852F9A19CEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageTreeDlg.h : header file
//

#include "msoeapi.h"
#include "mimeole.h"

/////////////////////////////////////////////////////////////////////////////
// CMessageTreeDlg dialog

class CMessageTreeDlg : public CDialog
{
// Construction
public:
	CMessageTreeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMessageTreeDlg)
	enum { IDD = IDD_MESSAGE_TREE };
	CButton	m_btnRemAttach;
	CButton	m_btnGoPrev;
	CButton	m_btnGoParent;
	CButton	m_btnGoNext;
	CButton	m_btnGoChild;
	CButton	m_btnDelBody;
	CButton	m_btnInstNext;
	CButton	m_btnInstChild;
	CButton	m_btnSetContent;
	CListBox	m_attachs;
	CComboBox	m_propNames;
	CString	m_bodyContent;
	CString	m_displayName;
	CString	m_propValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageTreeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	~CMessageTreeDlg();

	void SetMessage(IStoreFolder *pFolder, STOREFOLDERID msgId);

	void FillCombo();
	void UpdatePropValue(CString newValue=_T(""));
	void UpdateBodyInfo();
	void UpdateAttachments();
	CString GetContentType(HBODY hBody);

	IMimeMessage *m_pMimeMsg;
	IMimeAllocator *m_pAllocator;
	IMimePropertySet *m_pPropertySet;
	HBODY m_hCurBody;
	BOOL m_isTextBody;
	BOOL m_isMultiPart;
	BOOL m_isParentMultiPart;
	CString m_cntType;

protected:

	// Generated message map functions
	//{{AFX_MSG(CMessageTreeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnEditchangeComboPropName();
	afx_msg void OnSelchangeComboPropName();
	afx_msg void OnButtonGoChild();
	afx_msg void OnButtonGoNext();
	afx_msg void OnButtonGoParent();
	afx_msg void OnButtonGoPrev();
	afx_msg void OnButtonRemProp();
	afx_msg void OnButtonSaveChanges();
	afx_msg void OnButtonSetContent();
	afx_msg void OnButtonSetPropValue();
	afx_msg void OnButtonInsChild();
	afx_msg void OnButtonInsNext();
	afx_msg void OnButtonDelBody();
	afx_msg void OnAddAttach();
	afx_msg void OnRemAttach();
	afx_msg void OnSelchangeListAttachs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGETREEDLG_H__FA8C97EB_23EB_4A80_BA8E_B852F9A19CEB__INCLUDED_)
