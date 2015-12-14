//**********************************************************************************************************************
#include "IPAddress.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace network
{
//**********************************************************************************************************************
/*static*/ const IPAddress IPAddress::Loopback(INADDR_LOOPBACK);
/*static*/ const IPAddress IPAddress::Any(INADDR_ANY);
/*static*/ const IPAddress IPAddress::Broadcast(INADDR_BROADCAST);
/*static*/ const IPAddress IPAddress::None(INADDR_NONE);

//**********************************************************************************************************************
IPAddress::IPAddress()
{
	_addr.s_addr = 0;
}

//**********************************************************************************************************************
IPAddress::IPAddress(IN_ADDR const& other)
{
	_addr.s_addr = other.s_addr;
}

//**********************************************************************************************************************
IPAddress::IPAddress(IPAddress const& other)
{
	_addr.s_addr = other._addr.s_addr;
}

//**********************************************************************************************************************
IPAddress::IPAddress(uu::u32 ip)
{
	_addr.s_addr = htonl(ip);
}

//**********************************************************************************************************************
IPAddress::IPAddress(uu::u8 a, uu::u8 b, uu::u8 c, uu::u8 d)
{
	uu::u32 ip = (a << 24) | (b << 16) | (c << 8) | d;
	_addr.s_addr = htonl(ip);
}

//**********************************************************************************************************************
IPAddress::IPAddress(char const* ip_dotted_string)
{
	uu::u32 ip = inet_addr(ip_dotted_string);
	_addr.s_addr = ip;
}

//**********************************************************************************************************************
IPAddress& IPAddress::operator=(IPAddress const& other)
{
	_addr.s_addr = other._addr.s_addr;

	return *this;
}

//**********************************************************************************************************************
IPAddress& IPAddress::operator=(IN_ADDR const& other)
{
	_addr.s_addr = other.s_addr;

	return *this;
}

//**********************************************************************************************************************
void IPAddress::SetAddress(uu::u32 ip)
{
	_addr.s_addr = htonl(ip);
}

//**********************************************************************************************************************
bool IPAddress::operator==(IPAddress const& other) const
{
	return (_addr.s_addr == other._addr.s_addr);
}

//**********************************************************************************************************************
bool IPAddress::operator!=(IPAddress const& other) const
{
	return (_addr.s_addr != other._addr.s_addr);
}

//**********************************************************************************************************************
const char* IPAddress::ToString() const
{
	sprintf_s((char*)_str_tmp, sizeof(_str_tmp), "%u.%u.%u.%u",
		(_addr.s_addr) & 0xff,
		(_addr.s_addr >> 8) & 0xff,
		(_addr.s_addr >> 16) & 0xff,
		(_addr.s_addr >> 24) & 0xff);

	return _str_tmp;
}

} //namespace network

} //namespace univubi