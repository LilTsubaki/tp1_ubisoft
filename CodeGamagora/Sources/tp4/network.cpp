//**********************************************************************************************************************
#include "network.h"
#include "endianness.h"

//**********************************************************************************************************************
#pragma comment(lib, "Ws2_32.lib")

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace network
{
	//**********************************************************************************************************************
	bool Init()
	{
		uu::system::DetermineEndianness();

		WSADATA wsaData;
		WORD  wVersionRequested = MAKEWORD(2,2);

		if (WSAStartup(wVersionRequested, &wsaData) != 0)
		{
			return false;
		}

		return true;
	}

	//**********************************************************************************************************************
	void Terminate()
	{
		WSACleanup();
	}

	//**********************************************************************************************************************
	u16 HostToNetwork(u16 host_value)
	{
		u16 network_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert host_value from BigEndian to NetworkEndian
			return host_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert host_value from LittleEndian to NetworkEndian
			network_value = uu::system::LittleToBigEndian(host_value);
		}

		return network_value;
	}

	//**********************************************************************************************************************
	s16 HostToNetwork(s16 host_value)
	{
		s16 network_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert host_value from BigEndian to NetworkEndian
			return host_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert host_value from LittleEndian to NetworkEndian
			network_value = uu::system::LittleToBigEndian(host_value);
		}

		return network_value;
	}

	//**********************************************************************************************************************
	u32 HostToNetwork(u32 host_value)
	{
		u32 network_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert host_value from BigEndian to NetworkEndian
			return host_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert host_value from LittleEndian to NetworkEndian
			network_value = uu::system::LittleToBigEndian(host_value);
		}

		return network_value;
	}

	//**********************************************************************************************************************
	s32 HostToNetwork(s32 host_value)
	{
		s32 network_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert host_value from BigEndian to NetworkEndian
			return host_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert host_value from LittleEndian to NetworkEndian
			network_value = uu::system::LittleToBigEndian(host_value);
		}

		return network_value;
	}

	//**********************************************************************************************************************
	u64 HostToNetwork(u64 host_value)
	{
		u64 network_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert host_value from BigEndian to NetworkEndian
			return host_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert host_value from LittleEndian to NetworkEndian
			network_value = uu::system::LittleToBigEndian(host_value);
		}

		return network_value;
	}

	//**********************************************************************************************************************
	s64 HostToNetwork(s64 host_value)
	{
		s64 network_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert host_value from BigEndian to NetworkEndian
			return host_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert host_value from LittleEndian to NetworkEndian
			network_value = uu::system::LittleToBigEndian(host_value);
		}

		return network_value;
	}

	//**********************************************************************************************************************
	u16 NetworkToHost(u16 network_value)
	{
		u16 host_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert network_value from NetworkEndian to BigEndian
			return network_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert network_value from NetworkEndian to LittleEndian
			host_value = uu::system::LittleToBigEndian(network_value);
		}

		return host_value;
	}

	//**********************************************************************************************************************
	s16 NetworkToHost(s16 network_value)
	{
		s16 host_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert network_value from NetworkEndian to BigEndian
			return network_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert network_value from NetworkEndian to LittleEndian
			host_value = uu::system::LittleToBigEndian(network_value);
		}

		return host_value;
	}

	//**********************************************************************************************************************
	u32 NetworkToHost(u32 network_value)
	{
		u32 host_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert network_value from NetworkEndian to BigEndian
			return network_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert network_value from NetworkEndian to LittleEndian
			host_value = uu::system::LittleToBigEndian(network_value);
		}

		return host_value;
	}

	//**********************************************************************************************************************
	s32 NetworkToHost(s32 network_value)
	{
		s32 host_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert network_value from NetworkEndian to BigEndian
			return network_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert network_value from NetworkEndian to LittleEndian
			host_value = uu::system::LittleToBigEndian(network_value);
		}

		return host_value;
	}

	//**********************************************************************************************************************
	u64 NetworkToHost(u64 network_value)
	{
		u64 host_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert network_value from NetworkEndian to BigEndian
			return network_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert network_value from NetworkEndian to LittleEndian
			host_value = uu::system::LittleToBigEndian(network_value);
		}

		return host_value;
	}

	//**********************************************************************************************************************
	s64 NetworkToHost(s64 network_value)
	{
		s64 host_value = 0;

		if (uu::system::IsBigEndian())
		{
			//TO DO : convert network_value from NetworkEndian to BigEndian
			return network_value;
		}
		else if (uu::system::IsLittleEndian())
		{
			//TO DO : convert network_value from NetworkEndian to LittleEndian
			host_value = uu::system::LittleToBigEndian(network_value);
		}

		return host_value;
	}

} //namespace network

} //namespace univubi