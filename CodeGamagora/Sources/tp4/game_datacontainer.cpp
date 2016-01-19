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
	if (reader.ReadFloat(_view_range) == false) return false;
	if (reader.ReadFloat(_attack_range) == false) return false;
	if (reader.ReadFloat(_detect_range) == false) return false;
	if (reader.ReadUInt32(_entity_to_follow) == false) return false;
	if (reader.ReadUInt32(_entity_to_attack) == false) return false;

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
	if (writer.WriteFloat(_view_range) == false) return false;
	if (writer.WriteFloat(_attack_range) == false) return false;
	if (writer.WriteFloat(_detect_range) == false) return false;
	if (writer.WriteUInt32(_entity_to_follow) == false) return false;
	if (writer.WriteUInt32(_entity_to_attack) == false) return false;

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

	if (reader.ReadUInt32(_bomb_id) == false) return false;
	if (reader.ReadUInt32(_enemy_id) == false) return false;
	if (reader.ReadUInt32(_coins) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreatePlayerRequest::ReadFromNetworkData _owner=%s\n", _owner.ToString());

	return true;
}

//**********************************************************************************************************************
bool CreatePlayerRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (CreateCharacterRequest::WriteToNetworkData(writer) == false) return false;

	if (writer.WriteUInt32(_bomb_id) == false) return false;
	if (writer.WriteUInt32(_enemy_id) == false) return false;
	if (writer.WriteUInt32(_coins) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreatePlayerRequest::WriteToNetworkData _owner=%s\n", _owner.ToString());

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId CreateEnemyRequest::dataContainerId = uu::StringId("CreateEnemyRequest");

//**********************************************************************************************************************
bool CreateEnemyRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (CreateCharacterRequest::ReadFromNetworkData(reader,from_addr) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreateEnemyRequest::ReadFromNetworkData _owner=%s\n", _owner.ToString());

	return true;
}

//**********************************************************************************************************************
bool CreateEnemyRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (CreateCharacterRequest::WriteToNetworkData(writer) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreateEnemyRequest::WriteToNetworkData _owner=%s\n", _owner.ToString());

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId CreateBombRequest::dataContainerId = uu::StringId("CreateBombRequest");

//**********************************************************************************************************************
bool CreateBombRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (CreateEntityRequest::ReadFromNetworkData(reader, from_addr) == false) return false;

	if (reader.ReadInt64(_explosion_time) == false) return false;
	if (reader.ReadFloat(_explosion_radius) == false) return false;
	if (reader.ReadFloat(_current_radius) == false) return false;
	if (reader.ReadFloat(_power) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreateBombRequest::ReadFromNetworkData _owner=%s\n", _owner.ToString());

	return true;
}

//**********************************************************************************************************************
bool CreateBombRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (CreateEntityRequest::WriteToNetworkData(writer) == false) return false;

	if (writer.WriteInt64(_explosion_time) == false) return false;
	if (writer.WriteFloat(_explosion_radius) == false) return false;
	if (writer.WriteFloat(_current_radius) == false) return false;
	if (writer.WriteFloat(_power) == false) return false;

	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreateBombRequest::WriteToNetworkData _owner=%s\n", _owner.ToString());

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId GotoObjectRequest::dataContainerId = uu::StringId("GotoObjectRequest");

//**********************************************************************************************************************
bool GotoObjectRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_id) == false) return false;
	if (reader.ReadFloat(_x) == false) return false;
	if (reader.ReadFloat(_y) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool GotoObjectRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_id) == false) return false;
	if (writer.WriteFloat(_x) == false) return false;
	if (writer.WriteFloat(_y) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId FollowObjectRequest::dataContainerId = uu::StringId("FollowObjectRequest");

//**********************************************************************************************************************
bool FollowObjectRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_id) == false) return false;
	if (reader.ReadUInt32(_id_to_follow) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool FollowObjectRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_id) == false) return false;
	if (writer.WriteUInt32(_id_to_follow) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId AttackObjectRequest::dataContainerId = uu::StringId("AttackObjectRequest");

//**********************************************************************************************************************
bool AttackObjectRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_id_attacker) == false) return false;
	if (reader.ReadUInt32(_id_to_attack) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool AttackObjectRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_id_attacker) == false) return false;
	if (writer.WriteUInt32(_id_to_attack) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId HitObjectRequest::dataContainerId = uu::StringId("HitObjectRequest");

//**********************************************************************************************************************
bool HitObjectRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_id_attacker) == false) return false;
	if (reader.ReadUInt32(_id_to_hit) == false) return false;
	if (reader.ReadFloat(_hit_value) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool HitObjectRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_id_attacker) == false) return false;
	if (writer.WriteUInt32(_id_to_hit) == false) return false;
	if (writer.WriteFloat(_hit_value) == false) return false;

	return true;
}
