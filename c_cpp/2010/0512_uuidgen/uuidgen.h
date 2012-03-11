#pragma once

#include <Rpc.h>


// Minimum supported : Windows 2000 Professional/Server
// Error reported : Windows Vista and Windows Server 2008에서 IPv6를 사용시 ERROR_NO_DATA가 되었다는 보고가 있음

// Parameter :
//	Out UUID struct
//
// Returns :
//	RPC_S_OK/NO_ERROR (0L)			- (가능성 o  대부분)
//  RPC_S_UUID_LOCAL_ONLY (1824L)	- (가능성 o : 네트워크카드가 없거나 모두 비활성화 되었을 때)
//  RPC_S_UUID_NO_ADDRESS (1739L)	- (가능성 △)
//  ERROR_BUFFER_OVERFLOW (111L)	- (가능성 x : vector<> 할당 실패시)
//  ERROR_INVALID_DATA (13L)		- (가능성 △)
//  ERROR_INVALID_PARAMETER (87L)	- (가능성 x)
//  ERROR_NO_DATA (232L)			- (가능성 x : 인터페이스가 없을 경우 미호출)
//	ERROR_NOT_SUPPORTED (50L)		- (가능성 o) // OS : Windows 2000 이하
DWORD UuidGenWithMac(
	__out UUID __RPC_FAR * Uuid
	);
