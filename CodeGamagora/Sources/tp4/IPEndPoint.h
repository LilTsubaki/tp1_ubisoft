//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "network.h"
#include "IPAddress.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace network
{
//**********************************************************************************************************************

class IPEndPoint
{
public:
	IPEndPoint();
	IPEndPoint(uu::u16 port);
	IPEndPoint(uu::u32 ip, uu::u16 port = 0);
	IPEndPoint(IPAddress const& ip, uu::u16 port = 0);
	IPEndPoint(IPEndPoint const& other);
	IPEndPoint(SOCKADDR_IN const& other);

	IPEndPoint& operator=(IPEndPoint const& other);
	IPEndPoint& operator=(SOCKADDR_IN const& other);

	bool operator==(IPEndPoint const& other) const;
	bool operator!=(IPEndPoint const& other) const;

	SOCKADDR_IN& GetSockAddr() const { return (SOCKADDR_IN&)_addr; }

	bool IsValid() const { return (_addr.sin_port != 0 && _addr.sin_addr.s_addr != 0); }

	uu::u16 GetPort() const { return ntohs(_addr.sin_port); }
	uu::u32 GetAddress() const { return ntohl(_addr.sin_addr.s_addr); }

	void SetAddress(uu::u32 ip);
	void SetPort(uu::u16 port);

	const char* ToString() const;

protected:
	SOCKADDR_IN _addr;

	char _str_tmp[64];

};

} //namespace network

} //namespace univubi