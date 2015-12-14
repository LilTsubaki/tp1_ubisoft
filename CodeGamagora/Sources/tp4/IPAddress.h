//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "network.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace network
{
//**********************************************************************************************************************

class IPAddress
{
public:
	static const IPAddress Loopback;
	static const IPAddress Any;
	static const IPAddress Broadcast;
	static const IPAddress None;

	IPAddress();
	IPAddress(uu::u32 ip);
	IPAddress(IPAddress const& other);
	IPAddress(IN_ADDR const& other);
	IPAddress(uu::u8 a, uu::u8 b, uu::u8 c, uu::u8 d);
	IPAddress(char const* ip_dotted_string);

	IPAddress& operator=(IPAddress const& other);
	IPAddress& operator=(IN_ADDR const& other);

	bool operator==(IPAddress const& other) const;
	bool operator!=(IPAddress const& other) const;

	IN_ADDR& GetInAddr() const { return (IN_ADDR&)_addr; }

	uu::u32 GetAddress() const { return ntohl(_addr.s_addr); }
	void SetAddress(uu::u32 ip);

	const char* ToString() const;

protected:
	IN_ADDR _addr;
	
	char _str_tmp[64];

};

} //namespace network

} //namespace univubi