//**********************************************************************************************************************
#include "game_datacontainer.h"
#include "IPEndPoint.h"
#include "Log.h"

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId CreateEntityRequest::dataContainerId = uu::StringId("CreateEntityRequest");

//**********************************************************************************************************************
bool CreateEntityRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_type) == false) return false;
	if (reader.ReadString(_name) == false) return false;
	if (reader.ReadUInt32(_id) == false) return false;
	if (reader.ReadIPEndPoint(_owner) == false) return false;
	if (reader.ReadFloat(_x) == false) return false;
	if (reader.ReadFloat(_y) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreateEntityRequest::ReadFromNetworkData _owner=%s\n", _owner.ToString());

	return true;
}

//**********************************************************************************************************************
bool CreateEntityRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_type) == false) return false;
	if (writer.WriteString(_name) == false) return false;
	if (writer.WriteUInt32(_id) == false) return false;
	if (writer.WriteIPEndPoint(_owner) == false) return false;
	if (writer.WriteFloat(_x) == false) return false;
	if (writer.WriteFloat(_y) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreateEntityRequest::WriteToNetworkData _owner=%s\n", _owner.ToString());

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId CreateCharacterRequest::dataContainerId = uu::StringId("CreateCharacterRequest");

//**********************************************************************************************************************
bool CreateCharacterRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (CreateEntityRequest::ReadFromNetworkData(reader, from_addr) == false) return false;

	if (reader.ReadFloat(_speed) == false) return false;
	if (reader.ReadFloat(_power) == false) return false;
	if (reader.ReadFloat(_live) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreateCharacterRequest::ReadFromNetworkData _owner=%s\n", _owner.ToString());

	return true;
}

//**********************************************************************************************************************
bool CreateCharacterRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (CreateEntityRequest::WriteToNetworkData(writer) == false) return false;

	if (writer.WriteFloat(_speed) == false) return false;
	if (writer.WriteFloat(_power) == false) return false;
	if (writer.WriteFloat(_live) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreateCharacterRequest::WriteToNetworkData _owner=%s\n", _owner.ToString());

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId CreatePlayerRequest::dataContainerId = uu::StringId("CreatePlayerRequest");

//**********************************************************************************************************************
bool CreatePlayerRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (CreateCharacterRequest::ReadFromNetworkData(reader, from_addr) == false) return false;

	if (reader.ReadUInt32(_coins) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreatePlayerRequest::ReadFromNetworkData _owner=%s\n", _owner.ToString());

	return true;
}

//**********************************************************************************************************************
bool CreatePlayerRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (CreateCharacterRequest::WriteToNetworkData(writer) == false) return false;

	if (writer.WriteUInt32(_coins) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreatePlayerRequest::WriteToNetworkData _owner=%s\n", _owner.ToString());

	return true;
}


