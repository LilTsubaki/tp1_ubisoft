//**********************************************************************************************************************
#include "UdpClient.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace network
{
//**********************************************************************************************************************
UdpClient::UdpClient():
	_socket(INVALID_SOCKET),
	_isBound(false),
	_isBlocking(true),
	_isEnableBroadcast(false)
{
	_Create();
}

//**********************************************************************************************************************
UdpClient::UdpClient(IPEndPoint& endPoint):
	_socket(INVALID_SOCKET),
	_isBound(false),
	_isBlocking(true),
	_isEnableBroadcast(false)
{
	if (_Create() == true)
	{
		Bind(endPoint);
	}
}

//**********************************************************************************************************************
UdpClient::UdpClient(uu::u16 port):
	_socket(INVALID_SOCKET),
	_isBound(false),
	_isBlocking(true),
	_isEnableBroadcast(false)
{
	if (_Create() == true)
	{
		IPEndPoint endPoint(port);
		Bind(endPoint);
	}
}

//**********************************************************************************************************************
UdpClient::UdpClient(IPAddress const& ip, uu::u16 port):
	_socket(INVALID_SOCKET),
	_isBound(false),
	_isBlocking(true),
	_isEnableBroadcast(false)
{
	if (_Create() == true)
	{
		IPEndPoint endPoint(ip, port);
		Bind(endPoint);
	}
}

//**********************************************************************************************************************
UdpClient::UdpClient(uu::u32 ip, uu::u16 port):
	_socket(INVALID_SOCKET),
	_isBound(false),
	_isBlocking(true),
	_isEnableBroadcast(false)
{
	if (_Create() == true)
	{
		IPEndPoint endPoint(ip, port);
		Bind(endPoint);
	}
}

//**********************************************************************************************************************
bool UdpClient::_Create()
{
	if (_socket == INVALID_SOCKET)
	{
		_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}

	return (_socket != INVALID_SOCKET);
}

//**********************************************************************************************************************
bool UdpClient::Bind(IPEndPoint const& endPoint)
{
	SOCKADDR_IN& sock_addr = endPoint.GetSockAddr();

	if (_Create() == false)
	{
		return false;
	}

	if (bind(_socket, (SOCKADDR*)&sock_addr, sizeof(SOCKADDR_IN)) != SOCKET_ERROR)
	{
		SOCKADDR_IN& local_addr = _addr.GetSockAddr();
		int addr_len = sizeof(SOCKADDR_IN);
		if (getsockname(_socket, (SOCKADDR*)&local_addr, &addr_len) != SOCKET_ERROR)
		{
		}
		else
		{
			local_addr = sock_addr;
		}

		_isBound = true;
	}

	return _isBound;
}

//**********************************************************************************************************************
bool UdpClient::EnableBroadcast(bool value)
{
	// enable broadcast
	BOOL bvalue = value == true ? 1 : 0;
	if (setsockopt(_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bvalue, sizeof(BOOL)) != SOCKET_ERROR)
	{
		_isEnableBroadcast = value;
		return true;
	}

	return false;
}

//**********************************************************************************************************************
bool UdpClient::IsReadable(time_t timeMs)
{
	struct timeval tv = { (long)(timeMs/1000), (long)((timeMs%1000)*1000) };
	fd_set rset;
	FD_ZERO(&rset);
	FD_SET(_socket, &rset);

	int  err = select((int)_socket + 1, (struct fd_set *)&rset, 0, 0, &tv);

	if (err == SOCKET_ERROR)
		return false;

	if (FD_ISSET(_socket, &rset))
		return true;

	return false;
}

//**********************************************************************************************************************
bool UdpClient::IsWritable(time_t timeMs)
{
	struct timeval tv = { (long)(timeMs/1000), (long)((timeMs%1000)*1000) };
	fd_set wset;
	FD_ZERO(&wset);
	FD_SET(_socket, &wset);

	int  err = select((int)_socket + 1, 0, 0, (struct fd_set *)&wset, &tv);

	if (err == SOCKET_ERROR)
		return false;

	if (FD_ISSET(_socket, &wset))
		return true;

	return false;
}

//**********************************************************************************************************************
bool UdpClient::Blocking(bool value)
{
	// set non blocking mode
	u_long boption = value == true ? 0 : 1;
	if (ioctlsocket(_socket, FIONBIO, &boption) != SOCKET_ERROR)
	{
		_isBlocking = value;
		return true;
	}

	return false;
}

//**********************************************************************************************************************
bool UdpClient::SendTo(const char* bytes, int size, IPEndPoint const& endPoint)
{
	SOCKADDR_IN& sock_addr = endPoint.GetSockAddr();

	if (sendto(_socket, bytes , size , 0, (SOCKADDR*)&sock_addr, sizeof(SOCKADDR_IN)) > 0)
	{
		return true;
	}

	return false;
}

//**********************************************************************************************************************
bool UdpClient::RecvFrom(char* bytes, int capacity, int& size, IPEndPoint& endPoint)
{
	SOCKADDR_IN& sock_addr = endPoint.GetSockAddr();
	size_t from_addr_len = sizeof(SOCKADDR_IN);

	size = recvfrom(_socket, bytes , capacity , 0, (SOCKADDR*)&sock_addr, (int *)&from_addr_len);
	if (size > 0)
	{
		return true;
	}
	
	if (size == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err == WSAECONNRESET)
		{
			size = 0;
			return true;
		}
	}

	size = 0;
	return false;
}

//**********************************************************************************************************************
void UdpClient::Close()
{
	closesocket(_socket);
	_socket = INVALID_SOCKET;
	_isBound = false;
	_isBlocking = true;
	_isEnableBroadcast = false;
}

} //namespace network

} //namespace univubi