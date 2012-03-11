#pragma once

#include <Rpc.h>


// Minimum supported : Windows 2000 Professional/Server
// Error reported : Windows Vista and Windows Server 2008���� IPv6�� ���� ERROR_NO_DATA�� �Ǿ��ٴ� ���� ����

// Parameter :
//	Out UUID struct
//
// Returns :
//	RPC_S_OK/NO_ERROR (0L)			- (���ɼ� o  ��κ�)
//  RPC_S_UUID_LOCAL_ONLY (1824L)	- (���ɼ� o : ��Ʈ��ũī�尡 ���ų� ��� ��Ȱ��ȭ �Ǿ��� ��)
//  RPC_S_UUID_NO_ADDRESS (1739L)	- (���ɼ� ��)
//  ERROR_BUFFER_OVERFLOW (111L)	- (���ɼ� x : vector<> �Ҵ� ���н�)
//  ERROR_INVALID_DATA (13L)		- (���ɼ� ��)
//  ERROR_INVALID_PARAMETER (87L)	- (���ɼ� x)
//  ERROR_NO_DATA (232L)			- (���ɼ� x : �������̽��� ���� ��� ��ȣ��)
//	ERROR_NOT_SUPPORTED (50L)		- (���ɼ� o) // OS : Windows 2000 ����
DWORD UuidGenWithMac(
	__out UUID __RPC_FAR * Uuid
	);
