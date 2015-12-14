//**********************************************************************************************************************
#pragma once
//**********************************************************************************************************************
// Disable warning messages
#pragma warning( disable : 4700 4100 4127)

//**********************************************************************************************************************
#include <winsock2.h>
#include "types.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace network
{
	bool Init();
	void Terminate();

	u16 HostToNetwork(u16 host_value);
	s16 HostToNetwork(s16 host_value);
	u32 HostToNetwork(u32 host_value);
	s32 HostToNetwork(s32 host_value);
	u64 HostToNetwork(u64 host_value);
	s64 HostToNetwork(s64 host_value);

	u16 NetworkToHost(u16 network_value);
	s16 NetworkToHost(s16 network_value);
	u32 NetworkToHost(u32 network_value);
	s32 NetworkToHost(s32 network_value);
	u64 NetworkToHost(u64 network_value);
	s64 NetworkToHost(s64 network_value);

} //namespace network

} //namespace univubi