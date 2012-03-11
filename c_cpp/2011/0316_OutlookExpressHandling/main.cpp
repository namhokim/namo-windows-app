#include <Objbase.h>
#include <stdio.h>
#include <initguid.h>	// CLSID_StoreNamespace, IID_IStoreNamespace 정의를 위해
#include "msoeapi.h"
#include "mimeole.h"

//#pragma comment(lib, "Ole32.lib")

void EnumFolder(
	__in IStoreNamespace *pStoreNamespace,
	__in CONST STOREFOLDERID dwFolderId = FOLDERID_ROOT
);

BOOL GetFolderID(
	__in		IStoreNamespace *pStoreNamespace,
	__in		LPCSTR folderName,
	__out		STOREFOLDERID *pdwFolderId,
	__out_opt	CONST STOREFOLDERID dwParentFolderId = FOLDERID_ROOT
);

void EnumMessage(
	__in		IStoreNamespace *pStoreNamespace,
	__in		CONST STOREFOLDERID dwFolderId
);

void DoEnumMessage(
	__in		IStoreFolder *pFolder
);

void EnumMessageBody(IStoreFolder *pStoreFolder, STOREFOLDERID msgId);
void EnumMessageChildBody(IMimeMessage *pMimeMsg, HBODY m_hCurBody);

LPCSTR SpecificFolderName = "20110315";

int main()
{
	HRESULT hr = CoInitialize(0);
	if(FAILED(hr)) {
		fprintf(stderr, "Error CoInitialize\r\n");
		return S_FALSE;
	}

	// 수행
	IStoreNamespace *m_pStoreNamespace;
	hr = CoCreateInstance(CLSID_StoreNamespace, NULL, CLSCTX_SERVER, 
		IID_IStoreNamespace, (LPVOID*) &m_pStoreNamespace);
	if(FAILED(hr)) {
		fprintf(stderr, "Error creating IStoreNamespace.\r\n");
		CoUninitialize();
		return S_FALSE;
	}

	hr = m_pStoreNamespace->Initialize(NULL, NULL);
	if(FAILED(hr)) {
		fprintf(stderr, "Error initializing IStoreNamespace.\r\n");
		CoUninitialize();
		return S_FALSE;
	}

	// 폴더 Enum
	STOREFOLDERID folderID;
	BOOL bRes = GetFolderID(m_pStoreNamespace, SpecificFolderName, &folderID);
	if(bRes) {
		fprintf(stdout, "%s : %d\r\n", SpecificFolderName, folderID);
	} else {
		fprintf(stderr, "Not Exist follow folder name : %s\r\n", SpecificFolderName);
		CoUninitialize();
		return S_FALSE;
	}

	// 메시지 Enum
	EnumMessage(m_pStoreNamespace, folderID);

	CoUninitialize();
	return S_OK;
}

void EnumFolder(IStoreNamespace *pStoreNamespace, CONST STOREFOLDERID dwFolderId)
{
	FOLDERPROPS props;
	HENUMSTORE hEnum;

	hEnum = NULL;

	// set the size of the structure or the function return error
	props.cbSize = sizeof(FOLDERPROPS);

	HRESULT hr = pStoreNamespace->GetFirstSubFolder(dwFolderId, &props, &hEnum);

	while(SUCCEEDED(hr) && hr != S_FALSE && hEnum != NULL) {

		fprintf(stdout, "%s\r\n", props.szName);

		// enumerate children of this folder too
		EnumFolder(pStoreNamespace, props.dwFolderId);	// recursive call

		hr = pStoreNamespace->GetNextSubFolder(hEnum, &props);
	}

	// close the enum
	if(hEnum) {
		pStoreNamespace->GetSubFolderClose(hEnum);
	}
}

BOOL GetFolderID(IStoreNamespace *pStoreNamespace, LPCSTR folderName, STOREFOLDERID *pdwFolderId, CONST STOREFOLDERID dwParentFolderId)
{
	FOLDERPROPS props;
	HENUMSTORE hEnum;
	BOOL bFound;

	hEnum = NULL;
	bFound = FALSE;

	// set the size of the structure or the function return error
	props.cbSize = sizeof(FOLDERPROPS);

	HRESULT hr = pStoreNamespace->GetFirstSubFolder(dwParentFolderId, &props, &hEnum);

	while(SUCCEEDED(hr) && hr != S_FALSE && hEnum != NULL) {

		if( 0==lstrcmpA(folderName, props.szName) ) {	// matched
			*pdwFolderId = props.dwFolderId;
			bFound = TRUE;
			break;
		}

		// enumerate children of this folder too
		if( GetFolderID(pStoreNamespace, folderName, pdwFolderId, props.dwFolderId) ) {	// recursive call
			bFound = TRUE;
			break;	// if founc sub-folder then break while loop
		}
		
		hr = pStoreNamespace->GetNextSubFolder(hEnum, &props);
	}

	// close the enum
	if(hEnum) {
		pStoreNamespace->GetSubFolderClose(hEnum);
	}

	return bFound;
}

void EnumMessage(IStoreNamespace *pStoreNamespace, CONST STOREFOLDERID dwFolderId)
{
	HRESULT hr;
	IStoreFolder *m_pStoreFolder;
	FOLDERPROPS folderProps;

	m_pStoreFolder = NULL;

	// 폴더열기
	hr = pStoreNamespace->OpenFolder(dwFolderId, 0, &m_pStoreFolder);
	if(FAILED(hr)) {
		fprintf(stderr, "Error opening folder.\r\n");
		return;
	}
	folderProps.cbSize = sizeof(FOLDERPROPS);

	hr = pStoreNamespace->GetFolderProps(dwFolderId, 0, &folderProps);
	if(FAILED(hr)) {
		fprintf(stderr, "Error getting Folder Properties.\r\n");
		return;
	}

	// 메시지
	if(m_pStoreFolder) {
		DoEnumMessage(m_pStoreFolder);

		m_pStoreFolder->Release();
	}
}

void DoEnumMessage(IStoreFolder *pStoreFolder)
{
	MESSAGEPROPS msgProps;
	HENUMSTORE hEnumMsg;

	hEnumMsg = NULL;
	msgProps.cbSize = sizeof(MESSAGEPROPS);

	// as we want the subject and other staff we get all the properties.
	// you can use MSGPROPS_FAST as first parameter to get only a few properties of the message.
	HRESULT hr = pStoreFolder->GetFirstMessage(0,
		0,
		MESSAGEID_FIRST,
		&msgProps,
		&hEnumMsg);
	while(SUCCEEDED(hr) && hr != S_FALSE) {
		fprintf(stdout, "%s\r\n", msgProps.pszDisplayFrom);

		// 메시지 바디 조회
		EnumMessageBody(pStoreFolder, msgProps.dwMessageId);

		// free the message properties as they are allocated by IStoreFolder.
		pStoreFolder->FreeMessageProps(&msgProps);

		hr = pStoreFolder->GetNextMessage(hEnumMsg, 0, &msgProps);
	}

	// close the enum
	if(hEnumMsg) {
		pStoreFolder->GetMessageClose(hEnumMsg);
	}
}

void EnumMessageBody(IStoreFolder *pFolder, STOREFOLDERID msgId)
{
	IMimeMessage *m_pMimeMsg;
	//IMimeAllocator *m_pAllocator;
	
	HBODY m_hCurBody;
	HRESULT hr;

	m_pMimeMsg = NULL;
	//m_pAllocator = NULL;
	

	// first get IMimeMessage interface using IStoreFolder and the message id.
	hr = pFolder->OpenMessage(msgId,
							IID_IMimeMessage,
							(LPVOID*) &m_pMimeMsg);
	if(FAILED(hr)) {
		fprintf(stderr, "CMessageTreeDlg::SetMessage: OpenMessage.\r\n");
		return;
	}

	// get root body
	hr = m_pMimeMsg->GetBody(IBL_ROOT, 0, &m_hCurBody);	// child (root : IBL_ROOT)
	if(FAILED(hr)) {
		fprintf(stderr, "OEMessage::SetMessage: Cannot get root body.\r\n");
		return;
	}

	// get child body
	EnumMessageChildBody(m_pMimeMsg, m_hCurBody);

	if(m_pMimeMsg) {
		m_pMimeMsg->Release();
		m_pMimeMsg = NULL;
	}
	/*if(m_pAllocator) {
		m_pAllocator->Release();
	}*/
	
}

void EnumMessageChildBody(IMimeMessage *pMimeMsg, HBODY m_hCurBody)
{
	HRESULT hr;
	HBODY hBody;
	IMimePropertySet *m_pPropertySet;
	IMimeBody *pMimeBody = NULL;
	IStream *pBodyStream = NULL;

	m_pPropertySet = NULL;

	hr = pMimeMsg->GetBody(IBL_FIRST, m_hCurBody, &hBody);
	if(FAILED(hr)) {
		fprintf(stderr, "Cannot get body.\r\n");
	} else {

		hr = pMimeMsg->BindToObject(hBody, IID_IMimePropertySet, 
			(LPVOID *) &m_pPropertySet);
		if(FAILED(hr)) {
			fprintf(stderr, "OEMessage::UpdateBodyInfo: BindToObject IID_IMimePropertySet.\r\n");
			return;
		}

		while(1) {
			hr = pMimeMsg->BindToObject(hBody, IID_IMimeBody, (LPVOID *) &pMimeBody);
			if(FAILED(hr)) {
				fprintf(stderr, "CMessageTreeDlg::UpdateBodyInfo: BindToObject\r\n");
				break;
			}

			// Get body as a stream
			hr = pMimeBody->GetData(IET_UNICODE, &pBodyStream);
			if(FAILED(hr)) {
				fprintf(stderr, "OEMessage::GetBodyText: GetData\r\n");
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
		}

		if(m_pPropertySet) {
			m_pPropertySet->Release();
		}
	}
}