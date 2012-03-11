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

#include "stdafx.h"
#include "demo.h"
#include "MessageTreeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageTreeDlg dialog


CMessageTreeDlg::CMessageTreeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageTreeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessageTreeDlg)
	m_bodyContent = _T("");
	m_displayName = _T("");
	m_propValue = _T("");
	//}}AFX_DATA_INIT

	m_pMimeMsg = NULL;
	m_pAllocator = NULL;
	m_pPropertySet = NULL;
	m_isMultiPart = FALSE;
	m_isParentMultiPart = FALSE;
}


void CMessageTreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageTreeDlg)
	DDX_Control(pDX, IDC_REM_ATTACH, m_btnRemAttach);
	DDX_Control(pDX, IDC_BUTTON_GO_PREV, m_btnGoPrev);
	DDX_Control(pDX, IDC_BUTTON_GO_PARENT, m_btnGoParent);
	DDX_Control(pDX, IDC_BUTTON_GO_NEXT, m_btnGoNext);
	DDX_Control(pDX, IDC_BUTTON_GO_CHILD, m_btnGoChild);
	DDX_Control(pDX, IDC_BUTTON_DEL_BODY, m_btnDelBody);
	DDX_Control(pDX, IDC_BUTTON_INS_NEXT, m_btnInstNext);
	DDX_Control(pDX, IDC_BUTTON_INS_CHILD, m_btnInstChild);
	DDX_Control(pDX, IDC_BUTTON_SET_CONTENT, m_btnSetContent);
	DDX_Control(pDX, IDC_LIST_ATTACHS, m_attachs);
	DDX_Control(pDX, IDC_COMBO_PROP_NAME, m_propNames);
	DDX_Text(pDX, IDC_EDIT_CONTENT, m_bodyContent);
	DDX_Text(pDX, IDC_EDIT_DISPLAY_NAME, m_displayName);
	DDX_Text(pDX, IDC_EDIT_PROP_VALUE, m_propValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageTreeDlg, CDialog)
	//{{AFX_MSG_MAP(CMessageTreeDlg)
	ON_CBN_EDITCHANGE(IDC_COMBO_PROP_NAME, OnEditchangeComboPropName)
	ON_CBN_SELCHANGE(IDC_COMBO_PROP_NAME, OnSelchangeComboPropName)
	ON_BN_CLICKED(IDC_BUTTON_GO_CHILD, OnButtonGoChild)
	ON_BN_CLICKED(IDC_BUTTON_GO_NEXT, OnButtonGoNext)
	ON_BN_CLICKED(IDC_BUTTON_GO_PARENT, OnButtonGoParent)
	ON_BN_CLICKED(IDC_BUTTON_GO_PREV, OnButtonGoPrev)
	ON_BN_CLICKED(IDC_BUTTON_REM_PROP, OnButtonRemProp)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_CHANGES, OnButtonSaveChanges)
	ON_BN_CLICKED(IDC_BUTTON_SET_CONTENT, OnButtonSetContent)
	ON_BN_CLICKED(IDC_BUTTON_SET_PROP_VALUE, OnButtonSetPropValue)
	ON_BN_CLICKED(IDC_BUTTON_INS_CHILD, OnButtonInsChild)
	ON_BN_CLICKED(IDC_BUTTON_INS_NEXT, OnButtonInsNext)
	ON_BN_CLICKED(IDC_BUTTON_DEL_BODY, OnButtonDelBody)
	ON_BN_CLICKED(IDC_ADD_ATTACH, OnAddAttach)
	ON_BN_CLICKED(IDC_REM_ATTACH, OnRemAttach)
	ON_LBN_SELCHANGE(IDC_LIST_ATTACHS, OnSelchangeListAttachs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageTreeDlg message handlers
CMessageTreeDlg::~CMessageTreeDlg()
{
	if(m_pMimeMsg) {
		m_pMimeMsg->Release();
		m_pMimeMsg = NULL;
	}
	if(m_pAllocator) {
		m_pAllocator->Release();
	}
	if(m_pPropertySet) {
		m_pPropertySet->Release();
	}
}

BOOL CMessageTreeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HRESULT hr;

	// TODO: Add extra initialization here
	hr = CoCreateInstance(CLSID_IMimeAllocator, 
							NULL, CLSCTX_INPROC_SERVER, 
							IID_IMimeAllocator, 
							(LPVOID *) &m_pAllocator);
	if(m_pAllocator == NULL) {
		OutputDebugString("CMessageTreeDlg::OnInitDialog: m_pAllocator.\n");
	}

	m_btnRemAttach.EnableWindow(FALSE);

	UpdateBodyInfo();
	UpdateAttachments();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMessageTreeDlg::SetMessage(IStoreFolder *pFolder, STOREFOLDERID msgId)
{
	ASSERT(pFolder != NULL);

	HRESULT hr;

	// first get IMimeMessage interface using IStoreFolder and the message id.
	hr = pFolder->OpenMessage(msgId,
							IID_IMimeMessage,
							(LPVOID*) &m_pMimeMsg);
	if(FAILED(hr)) {
		OutputDebugString("CMessageTreeDlg::SetMessage: OpenMessage.\n");
		MessageBox(_T("Cannot open message."), _T("Demo Error"), MB_OK);
		return;
	}
	
	hr = m_pMimeMsg->GetBody(IBL_ROOT, 0, &m_hCurBody);
	if(FAILED(hr)) {
		OutputDebugString("OEMessage::SetMessage: Cannot get root body.\n");
		MessageBox(_T("Cannot get root body."), _T("Demo Error"), MB_OK);
		return;
	}
}

CString CMessageTreeDlg::GetContentType(HBODY hBody)
{
	HRESULT hr;
	PROPVARIANT propValue;
	CString cntType = _T("");

	propValue.vt = VT_LPSTR;

	hr = m_pMimeMsg->GetBodyProp(hBody, PIDTOSTR(PID_HDR_CNTTYPE), 0, &propValue);
	if(SUCCEEDED(hr)) {
		cntType = propValue.pszVal;
		cntType.MakeLower();

		PropVariantClear(&propValue);
	}

	return cntType;
}

void CMessageTreeDlg::UpdateBodyInfo()
{
	IMimeBody *pMimeBody = NULL;
	LPSTR displayName;
	ENCODINGTYPE encType;
	IStream *pBodyStream = NULL;
	ULONG ulRead;
	WCHAR lpszwBody[2048];
	char lpszBody[2048];
	HRESULT hr;
	CString parentCntType;
	HBODY hBody;

	if(m_pPropertySet) {
		m_pPropertySet->Release();
		m_pPropertySet = NULL;
	}

	hr = m_pMimeMsg->BindToObject(m_hCurBody, IID_IMimePropertySet, 
								(LPVOID *) &m_pPropertySet);
	if(FAILED(hr)) {
		OutputDebugString("OEMessage::UpdateBodyInfo: BindToObject IID_IMimePropertySet.\n");
		return;
	}

	m_propValue = _T("");
	
	FillCombo();

	while(1) {
		hr = m_pMimeMsg->BindToObject(m_hCurBody,
									IID_IMimeBody,
									(LPVOID *) &pMimeBody);
		if(FAILED(hr)) {
			OutputDebugString("CMessageTreeDlg::UpdateBodyInfo: BindToObject\n");
			break;
		}

		hr = pMimeBody->GetDisplayName(&displayName);
		if(SUCCEEDED(hr)) {
			m_displayName = displayName;
			CoTaskMemFree(displayName);
		}
		else {
			m_displayName = _T("");
		}
		
		encType = IET_BINARY;
		m_isTextBody = FALSE;

		m_cntType = GetContentType(m_hCurBody);

		// if the body is a 'text' treat as a text. Otherwise, read it as a buffer char
		// by char.
		if(m_cntType.Find(_T("text")) == 0) {
			encType = IET_UNICODE;
			m_isTextBody = TRUE;
		}

		m_isMultiPart = (m_cntType.Find(_T("multipart")) == 0);

		m_isParentMultiPart = FALSE;

		m_btnGoChild.EnableWindow(FALSE);
		m_btnGoParent.EnableWindow(FALSE);
		m_btnGoNext.EnableWindow(FALSE);
		m_btnGoPrev.EnableWindow(FALSE);

		hr = m_pMimeMsg->GetBody(IBL_PARENT, m_hCurBody, &hBody);
		if(SUCCEEDED(hr)) {
			m_btnGoParent.EnableWindow(TRUE);
			parentCntType = GetContentType(hBody);
			m_isParentMultiPart = (parentCntType.Find(_T("multipart")) == 0);
		}
		hr = m_pMimeMsg->GetBody(IBL_FIRST, m_hCurBody, &hBody);
		if(SUCCEEDED(hr)) {
			m_btnGoChild.EnableWindow(TRUE);
		}
		hr = m_pMimeMsg->GetBody(IBL_NEXT, m_hCurBody, &hBody);
		if(SUCCEEDED(hr)) {
			m_btnGoNext.EnableWindow(TRUE);
		}
		hr = m_pMimeMsg->GetBody(IBL_PREVIOUS, m_hCurBody, &hBody);
		if(SUCCEEDED(hr)) {
			m_btnGoPrev.EnableWindow(TRUE);
		}

		// only text bodies can be set. The others are body trees and must be constructed.
		m_btnSetContent.EnableWindow(m_isTextBody);

		// only multipart bodies can have children.
		m_btnInstChild.EnableWindow(m_isMultiPart);
		m_btnInstNext.EnableWindow(m_isParentMultiPart);

		// multipart bodies cannot be deleted.
		m_btnDelBody.EnableWindow(!m_isMultiPart);

		m_bodyContent = _T("");
		
		// Get body as a stream
		hr = pMimeBody->GetData(IET_UNICODE,
								&pBodyStream);
		if(FAILED(hr)) {
			OutputDebugString("OEMessage::GetBodyText: GetData\n");
			break;
		}

		// if it is a text when we read it it comes unicode.
		if(encType == IET_UNICODE) {
			// for text bodies
			do {
				// Read the IStream into our buffer
				hr = pBodyStream->Read(lpszwBody,
										sizeof(lpszwBody)-sizeof(WCHAR),
										&ulRead);
				if(FAILED(hr)) {
					OutputDebugString("OEMessage::GetBodyText: Read\n");
				}
				else if(ulRead != 0) {
					// Null terminate it
					lpszwBody[ulRead/2] = '\0';
					m_bodyContent += (WCHAR *) lpszwBody;
				}
			} while(ulRead != 0);
		}
		else {
			do {
				// Read the IStream into our buffer
				hr = pBodyStream->Read(lpszBody,
										sizeof(lpszBody)-sizeof(char),
										&ulRead);
				if(FAILED(hr)) {
					OutputDebugString("OEMessage::GetBodyText: Read\n");
				}
				else if(ulRead != 0) {
					// Null terminate it
					lpszBody[ulRead] = '\0';
					m_bodyContent += lpszBody;
				}
			} while(ulRead != 0);
		}

		pBodyStream->Release();

		break;
	}

	if(pMimeBody) {
		pMimeBody->Release();
	}

	UpdateData(FALSE);
}

void CMessageTreeDlg::FillCombo()
{
	ASSERT(m_pMimeMsg != NULL);

	IMimeEnumProperties *pEnum = NULL;
	ENUMPROPERTY eProp = {0};
	ULONG cFetched;
	HRESULT hr;

	m_propNames.ResetContent();
	
	// enum properties of the body.
	hr = m_pPropertySet->EnumProps(0, &pEnum);
	if(FAILED(hr)) {
		OutputDebugString("OEMessage::FillCombo: EnumProps.\n");
		return;
	}

	hr = pEnum->Next(1, &eProp, &cFetched);

	while(SUCCEEDED(hr) && hr != S_FALSE) {
		m_propNames.AddString(eProp.pszName);

		hr = m_pAllocator->FreeEnumPropertyArray(1, &eProp, FALSE);
		if(FAILED(hr)) {
			OutputDebugString("OEMessage::FillCombo: FreeEnumPropertyArray.\n");
		}

		hr = pEnum->Next(1, &eProp, &cFetched);
	}

	if(m_propNames.GetCount()) {
		m_propNames.SetCurSel(0);
		UpdatePropValue();
	}

	if(pEnum) {
		pEnum->Release();
	}
}

void CMessageTreeDlg::UpdatePropValue(CString newValue)
{
	PROPVARIANT propValue;
	HRESULT hr;
	CString cmbValue;

	if(newValue.IsEmpty()) {
		m_propNames.GetWindowText(cmbValue);
	}
	else {
		cmbValue = newValue;
	}

	if(cmbValue.IsEmpty()) {
		m_propValue = _T("Invalid");
	}
	else {
		propValue.vt = VT_LPSTR;
		hr = m_pPropertySet->GetProp((LPCTSTR) cmbValue, 0, &propValue);
		if(SUCCEEDED(hr)) {
			//m_propNames.AddString(propValue.pszVal);
			m_propValue = propValue.pszVal;
			m_pAllocator->PropVariantClear(&propValue);
		}
		else {
			m_propValue = _T("Invalid");
		}
	}

	UpdateData(FALSE);
}

void CMessageTreeDlg::UpdateAttachments()
{
	ULONG attachCount, i, j;
	HBODY *bodyAttachs = NULL;
	HRESULT hr;
	IMimeBody *pMimeBody;
	LPSTR display;
	int nItem;

	m_attachs.ResetContent();

	hr = m_pMimeMsg->GetAttachments(&attachCount, &bodyAttachs);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot get attachments: GetAttachments."), _T("Demo Error"), MB_OK);
		return;
	}

	// keep only bodies of type IBT_ATTACHMENT.
	for(i=0; i<attachCount;) {
		hr = m_pMimeMsg->IsBodyType(bodyAttachs[i], IBT_ATTACHMENT);
		if(hr != S_OK) {
			for(j=i+1; j<attachCount; j++) {
				bodyAttachs[j-1] = bodyAttachs[j];
			}

			attachCount--;
		}
		else {
			// for the attachments, get display name of the body to add to the listbox.
			hr = m_pMimeMsg->BindToObject(bodyAttachs[i],
											IID_IMimeBody,
											(LPVOID *) &pMimeBody);
			if(SUCCEEDED(hr)) {
				hr = pMimeBody->GetDisplayName(&display);
				if(SUCCEEDED(hr)) {
					nItem = m_attachs.AddString(display);
					m_attachs.SetItemData(nItem, (DWORD) bodyAttachs[i]);

					CoTaskMemFree(display);
				}

			}

			i++;
		}
	}

	if(bodyAttachs) {
		CoTaskMemFree(bodyAttachs);
	}
}

void CMessageTreeDlg::OnEditchangeComboPropName() 
{
	UpdateData(TRUE);

	UpdatePropValue();
}

void CMessageTreeDlg::OnSelchangeComboPropName() 
{
	CString newValue;
	int nSel;

	nSel = m_propNames.GetCurSel();
	if(nSel == CB_ERR) {
		m_propValue = _T("Invalid");
		OutputDebugString("CMessageTreeDlg::OnSelchangeComboPropName: Cannot get property name.\n");
		return;
	}

	m_propNames.GetLBText(nSel, newValue);

	UpdatePropValue(newValue);
}

void CMessageTreeDlg::OnButtonGoChild() 
{
	HBODY hBody;
	HRESULT hr;

	hr = m_pMimeMsg->GetBody(IBL_FIRST, m_hCurBody, &hBody);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot get body."), _T("Demo Error"), MB_OK);
	}
	else {
		m_hCurBody = hBody;
		UpdateBodyInfo();
	}
}

void CMessageTreeDlg::OnButtonGoNext() 
{
	HBODY hBody;
	HRESULT hr;

	hr = m_pMimeMsg->GetBody(IBL_NEXT, m_hCurBody, &hBody);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot get body."), _T("Demo Error"), MB_OK);
	}
	else {
		m_hCurBody = hBody;
		UpdateBodyInfo();
	}
}

void CMessageTreeDlg::OnButtonGoParent() 
{
	HBODY hBody;
	HRESULT hr;

	hr = m_pMimeMsg->GetBody(IBL_PARENT, m_hCurBody, &hBody);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot get body."), _T("Demo Error"), MB_OK);
	}
	else {
		m_hCurBody = hBody;
		UpdateBodyInfo();
	}
}

void CMessageTreeDlg::OnButtonGoPrev() 
{
	HBODY hBody;
	HRESULT hr;

	hr = m_pMimeMsg->GetBody(IBL_PREVIOUS, m_hCurBody, &hBody);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot get body."), _T("Demo Error"), MB_OK);
	}
	else {
		m_hCurBody = hBody;
		UpdateBodyInfo();
	}
}

void CMessageTreeDlg::OnButtonRemProp() 
{
	HRESULT hr;
	CString cmbValue;

	UpdateData(TRUE);

	m_propNames.GetWindowText(cmbValue);

	hr = m_pMimeMsg->DeleteBodyProp(m_hCurBody, cmbValue);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot remove property."), _T("Demo Error"), MB_OK);
	}
	else {
		int nSel;

		nSel = m_propNames.GetCurSel();
		if(nSel != CB_ERR) {
			m_propNames.DeleteString(nSel);

			if(m_propNames.GetCount() > 0) {
				m_propNames.SetCurSel(0);
			}

			UpdatePropValue();
		}
	}
}

void CMessageTreeDlg::OnButtonSetPropValue() 
{
	PROPVARIANT propValue;
	HRESULT hr;
	CString cmbValue;

	UpdateData(TRUE);

	m_propNames.GetWindowText(cmbValue);

	propValue.vt = VT_LPSTR;
	propValue.pszVal = (LPSTR) m_pAllocator->Alloc(m_propValue.GetLength()+1);

	if(propValue.pszVal == NULL) {
		MessageBox(_T("Cannot set property: Alloc."), _T("Demo Error"), MB_OK);
		return;
	}

//	strcpy(propValue.pszVal, m_propValue);
	strcpy_s(propValue.pszVal, m_propValue.GetLength(), m_propValue);

	hr = m_pMimeMsg->SetBodyProp(m_hCurBody, cmbValue, NOFLAGS, &propValue);
	
	m_pAllocator->Free(propValue.pszVal);

	if(FAILED(hr)) {
		MessageBox(_T("Cannot set property: SetBodyProp."), _T("Demo Error"), MB_OK);
	}
	else {
		int nSel;

		nSel = m_propNames.GetCurSel();
		if(nSel == CB_ERR) {
			m_propNames.AddString(cmbValue);
		}
	}
}

void CMessageTreeDlg::OnButtonSaveChanges() 
{
	HRESULT hr;

	hr = m_pMimeMsg->Commit(COMMIT_REUSESTORAGE);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot save changes."), _T("Demo Error"), MB_OK);
	}
}

void CMessageTreeDlg::OnButtonSetContent() 
{
	HRESULT hr;
	ULONG ulLength, ulWritten;
	BSTR bstr = NULL;
	IStream *pStream = NULL;
	IMimeBody *pMimeBody = NULL;
	PROPVARIANT propValue;

	UpdateData(TRUE);

	while(1) {
		// create a new stream to write in the new body
		hr = CreateStreamOnHGlobal(NULL,
									TRUE,
									&pStream);
		if(FAILED(hr)) {
			MessageBox(_T("Cannot set content: CreateStreamOnHGlobal."), _T("Demo Error"), MB_OK);
			break;
		}

		// compute the new body length + the zero that terminates the string
		ulLength = m_bodyContent.GetLength() + 1;

		// there are better ways to do it but this is the easiest
		bstr = m_bodyContent.AllocSysString();

		// write in the new body
		hr = pStream->Write((LPWSTR) bstr,
							ulLength * sizeof(WCHAR),
							&ulWritten);
		if(FAILED(hr)) {
			MessageBox(_T("Cannot set content: Write."), _T("Demo Error"), MB_OK);
			break;
		}

		// Commit the stream
		hr = pStream->Commit(STGC_DEFAULT);
		if(FAILED(hr)) {
			MessageBox(_T("Cannot set content: Commit."), _T("Demo Error"), MB_OK);
			break;
		}

		// bind body handle to IMimeBody interface
		hr = m_pMimeMsg->BindToObject(m_hCurBody,
										IID_IMimeBody,
										(LPVOID *) &pMimeBody);
		if(FAILED(hr)) {
			MessageBox(_T("Cannot set content: Commit."), _T("Demo Error"), MB_OK);
			break;
		}

		CString priCon, secCon;

		propValue.vt = VT_LPSTR;

		// get content-type property to save the body.
		hr = m_pMimeMsg->GetBodyProp(m_hCurBody, PIDTOSTR(PID_HDR_CNTTYPE), 0, &propValue);
		if(FAILED(hr) || hr == S_FALSE) {
			MessageBox(_T("Cannot set content: GetBodyProp."), _T("Demo Error"), MB_OK);
			break;
		}

		// this property has the format of 'primaryType/secondaryType'
		char *sep = strchr(propValue.pszVal, '/');

		if(sep == NULL) {
			MessageBox(_T("Cannot set content: Content Type error."), _T("Demo Error"), MB_OK);
			PropVariantClear(&propValue);
			break;
		}

		secCon = sep+1;
		*sep = 0;
		priCon = propValue.pszVal;

		PropVariantClear(&propValue);

		// save the data in this new stream into the body using the save conent-type it had before.
		hr = pMimeBody->SetData(IET_UNICODE,
								priCon,
								secCon,
								IID_IStream,
								pStream);
		if(FAILED(hr)) {
			MessageBox(_T("Cannot set content: SetData."), _T("Demo Error"), MB_OK);
			break;
		}

		break;
	}

	if(bstr) {
		::SysFreeString(bstr);
	}
	if(pMimeBody) {
		pMimeBody->Release();
	}
	if(pStream) {
		pStream->Release();
	}
}


void CMessageTreeDlg::OnButtonInsChild() 
{
	HRESULT hr;
	HBODY newBody;

	hr = m_pMimeMsg->InsertBody((BODYLOCATION) IBL_FIRST, m_hCurBody, &newBody);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot create body: InsertBody."), _T("Demo Error"), MB_OK);
	}
	else {
		m_hCurBody = newBody;
		UpdateBodyInfo();
	}
}

void CMessageTreeDlg::OnButtonInsNext() 
{
	HRESULT hr;
	HBODY newBody;

	hr = m_pMimeMsg->InsertBody((BODYLOCATION) IBL_NEXT, m_hCurBody, &newBody);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot create body: InsertBody."), _T("Demo Error"), MB_OK);
	}
	else {
		m_hCurBody = newBody;
		UpdateBodyInfo();
	}
}

void CMessageTreeDlg::OnButtonDelBody() 
{
	HRESULT hr;

	hr = m_pMimeMsg->DeleteBody(m_hCurBody, DELETE_PROMOTE_CHILDREN);
	if(FAILED(hr)) {
		MessageBox(_T("Cannot delete body: DeleteBody."), _T("Demo Error"), MB_OK);
	}
	else {
		hr = m_pMimeMsg->GetBody(IBL_ROOT, 0, &m_hCurBody);
		if(FAILED(hr)) {
			MessageBox(_T("Cannot get root body."), _T("Demo Error"), MB_OK);
			return;
		}

		UpdateBodyInfo();
	}
}

void CMessageTreeDlg::OnAddAttach() 
{
	HRESULT hr;
	HBODY hNewBody;
	char filename[1024] = "";
	OPENFILENAME ofn = { 0 };

	ofn.lStructSize  = sizeof(ofn);
	ofn.hwndOwner    = m_hWnd;
	ofn.lpstrFilter  = "All files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFile    = filename;
	ofn.nMaxFile     = sizeof(filename);
	ofn.Flags        = OFN_FILEMUSTEXIST;
	if(GetOpenFileName(&ofn)) {
		hr = m_pMimeMsg->AttachFile(filename, NULL, &hNewBody);
		if(FAILED(hr)) {
			MessageBox(_T("Cannot attach file: AttachFile."), _T("Demo Error"), MB_OK);
		}
		else {
			UpdateAttachments();
		}
	}
}

void CMessageTreeDlg::OnRemAttach() 
{
	int sel;
	HRESULT hr;
	HBODY hBody;

	sel = m_attachs.GetCurSel();

	if(sel >= 0) {
		hBody = (HBODY) m_attachs.GetItemData(sel);
		hr = m_pMimeMsg->DeleteBody(hBody, 0);
		if(FAILED(hr)) {
			OutputDebugString(_T("OnRemAttach: DeleteBody.\n"));
		}

		UpdateAttachments();
	}
}

void CMessageTreeDlg::OnSelchangeListAttachs() 
{
	m_btnRemAttach.EnableWindow(m_attachs.GetCurSel() >= 0);
}
