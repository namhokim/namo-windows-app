#ifndef _PHY_ADDR_H_
#define _PHY_ADDR_H_

#include <vector>
#include "tstring.h"
namespace Network
{
	enum Adapters {
		TypeOhter		= 1,
		TypeEthernet	= 6,
		TypeTokenring	= 9,
		TypeFDDI		= 15,
		TypePPP			= 23,
		TypeLoopback	= 24,
		TypeSLIP		= 28,
	};

	typedef std::vector<std::tstring> PhyAddrVec;
	PhyAddrVec GetMyPhyAddrs(Adapters type = TypeEthernet);
} // end of namespace Network
#endif