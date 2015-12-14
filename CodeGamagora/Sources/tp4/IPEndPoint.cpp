//**********************************************************************************************************************
#include "IPEndPoint.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace network
{
//**********************************************************************************************************************
IPEndPoint::IPEndPoint()
{
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = 0;
	_addr.sin_addr.s_addr = 0;
}

//**********************************************************************************************************************
IPEndPoint::IPEndPoint(SOCKADDR_IN const& other)
{
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = other.sin_family;
	_addr.sin_port = other.sin_port;
	_addr.sin_addr.s_addr = other.sin_addr.s_addr;
}

//**********************************************************************************************************************
IPEndPoint::IPEndPoint(IPEndPoint const& other)
{
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = other._addr.sin_family;
	_addr.sin_port = other._addr.sin_port;
	_addr.sin_addr.s_addr = other._addr.sin_addr.s_addr;
}

//**********************************************************************************************************************
IPEndPoint::IPEndPoint(uu::u16 port)
{
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = 0;
}

//**********************************************************************************************************************
IPEndPoint::IPEndPoint(IPAddress const& ip, uu::u16 port)
{
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = ip.GetInAddr().s_addr;
}

//**********************************************************************************************************************
IPEndPoint::IPEndPoint(uu::u32 ip, uu::u16 port)
{
	_addr.sin_family = AF_INET;
	_addr.sin_port = htons(port);
	_addr.sin_addr.s_addr = htonl(ip);
}

//**********************************************************************************************************************
IPEndPoint& IPEndPoint::operator=(IPEndPoint const& other)
{
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = other._addr.sin_family;
	_addr.sin_port = other._addr.sin_port;
	_addr.sin_addr.s_addr = other._addr.sin_addr.s_addr;

	return *this;
}

//**********************************************************************************************************************
IPEndPoint& IPEndPoint::operator=(SOCKADDR_IN const& other)
{
	memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = other.sin_family;
	_addr.sin_port = other.sin_port;
	_addr.sin_addr.s_addr = other.sin_addr.s_addr;

	return *this;
}

//**********************************************************************************************************************
void IPEndPoint::SetAddress(uu::u32 ip)
{
	_addr.sin_addr.s_addr = htonl(ip);
}

//**********************************************************************************************************************
void IPEndPoint::SetPort(uu::u16 port)
{
	_addr.sin_port = htons(port);
}

//**********************************************************************************************************************
bool IPEndPoint::operator==(IPEndPoint const& other) const
{
	return (_addr.sin_addr.s_addr == other._addr.sin_addr.s_addr
		&& _addr.sin_port == other._addr.sin_port
		&& _addr.sin_family == other._addr.sin_family);
}

//**********************************************************************************************************************
bool IPEndPoint::operator!=(IPEndPoint const& other) const
{
	return (_addr.sin_addr.s_addr != other._addr.sin_addr.s_addr
		|| _addr.sin_port != other._addr.sin_port
		|| _addr.sin_family != other._addr.sin_family);
}

//**********************************************************************************************************************
const char* IPEndPoint::ToString() const
{
	sprintf_s((char*)_str_tmp, sizeof(_str_tmp), "%u.%u.%u.%u:%u",
		(_addr.sin_addr.s_addr) & 0xff,
		(_addr.sin_addr.s_addr >> 8) & 0xff,
		(_addr.sin_addr.s_addr >> 16) & 0xff,
		(_addr.sin_addr.s_addr >> 24) & 0xff,
		ntohs(_addr.sin_port));

	return _str_tmp;
}

} //namespace network

} //namespace univubi