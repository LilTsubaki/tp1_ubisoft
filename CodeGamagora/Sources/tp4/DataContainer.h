//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "serializer.h"
#include "StringId.h"
#include "IPEndPoint.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace network
{

//**********************************************************************************************************************

class DataContainer
{
public:
	virtual uu::StringId const& GetDataContainerId() const = 0;
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr) = 0;
	virtual bool WriteToNetworkData(uu::Writer& writer) = 0;

protected:
	bool _ReadDataContainerId(uu::Reader& reader)
	{
		uu::u32 dataContainerId;

		if (reader.ReadUInt32(dataContainerId) == false) return false;

		if (GetDataContainerId() != dataContainerId)
			return false;

		return true;
	}

	bool _WriteDataContainerId(uu::Writer& writer)
	{
		return writer.WriteUInt32(GetDataContainerId().Id());
	}

};

} //namespace network

} //namespace univubi