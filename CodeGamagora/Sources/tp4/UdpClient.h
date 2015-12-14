 //**********************************************************************************************************************
#pragma once

#include "types.h"
#include "network.h"
#include "IPEndPoint.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace network
{

//**********************************************************************************************************************

class UdpClient
{
public:
	UdpClient();
	UdpClient(IPEndPoint& endPoint);
	UdpClient(uu::u16 port);
	UdpClient(uu::u32 ip, uu::u16 port = 0);
	UdpClient(IPAddress const& ip, uu::u16 port = 0);

	bool IsSocketValid() const { return (_socket != SOCKET_ERROR); }
	bool IsBound() const { return _isBound; }
	bool IsBlocking() const { return _isBlocking; }
	bool IsEnableBroadcast() const { return _isEnableBroadcast; }
	IPEndPoint& GetLocalEndPoint() const { return (IPEndPoint&)_addr; }
	
	uu::u32 GetAddress() const { return _addr.GetAddress(); }
	uu::u16 GetPort() const { return _addr.GetPort(); }

	bool EnableBroadcast(bool value);
	bool Blocking(bool value);

	bool Bind(IPEndPoint const& endPoint);
	bool IsReadable(time_t timeMs = 0);
	bool IsWritable(time_t timeMs = 0);
	bool SendTo(const char* bytes, int size, IPEndPoint const& endPoint);
	bool RecvFrom(char* bytes, int capacity, int& size, IPEndPoint& endPoint);

	void Close();

protected:
	SOCKET _socket;
	IPEndPoint _addr;
	bool _isBound;
	bool _isBlocking;
	bool _isEnableBroadcast;

	bool _Create();
};

} //namespace network

} //namespace univubi