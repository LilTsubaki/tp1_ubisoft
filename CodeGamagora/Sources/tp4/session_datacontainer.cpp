//**********************************************************************************************************************
#include "session_datacontainer.h"

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId EchoRequest::dataContainerId = uu::StringId("EchoRequest");

//**********************************************************************************************************************
bool EchoRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadString(_service_name) == false) return false;
	if (reader.ReadInt64(_time_echo) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool EchoRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteString(_service_name) == false) return false;
	if (writer.WriteInt64(_time_echo) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId EchoEvent::dataContainerId = uu::StringId("EchoEvent");

//**********************************************************************************************************************
bool EchoEvent::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool EchoEvent::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId EchoAnswer::dataContainerId = uu::StringId("EchoAnswer");

//**********************************************************************************************************************
bool EchoAnswer::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadString(_service_name) == false) return false;
	if (reader.ReadInt64(_time_echo) == false) return false;
	if (reader.ReadIPEndPoint(_your_addr) == false) return false;
	if (reader.ReadIPEndPoint(_service_addr) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool EchoAnswer::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteString(_service_name) == false) return false;
	if (writer.WriteInt64(_time_echo) == false) return false;
	if (writer.WriteIPEndPoint(_your_addr) == false) return false;
	if (writer.WriteIPEndPoint(_service_addr) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId CreateSessionRequest::dataContainerId = uu::StringId("CreateSessionRequest");

//**********************************************************************************************************************
bool CreateSessionRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadString(_host_player_name) == false) return false;
	if (reader.ReadByte(_max_players) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool CreateSessionRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteString(_host_player_name) == false) return false;
	if (writer.WriteByte(_max_players) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId CreateSessionAnswer::dataContainerId = uu::StringId("CreateSessionAnswer");

//**********************************************************************************************************************
bool CreateSessionAnswer::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadBool(_result) == false) return false;

	if (_descriptor.ReadFromNetworkData(reader, from_addr) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool CreateSessionAnswer::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteBool(_result) == false) return false;

	if (_descriptor.WriteToNetworkData(writer) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId JoinSessionRequest::dataContainerId = uu::StringId("JoinSessionRequest");

//**********************************************************************************************************************
bool JoinSessionRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_session_id) == false) return false;
	if (reader.ReadString(_player_name) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool JoinSessionRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_session_id) == false) return false;
	if (writer.WriteString(_player_name) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId JoinSessionAnswer::dataContainerId = uu::StringId("JoinSessionAnswer");

//**********************************************************************************************************************
bool JoinSessionAnswer::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadBool(_result) == false) return false;
	if (_descriptor.ReadFromNetworkData(reader, from_addr) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool JoinSessionAnswer::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteBool(_result) == false) return false;
	if (_descriptor.WriteToNetworkData(writer) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId JoinSessionEvent::dataContainerId = uu::StringId("JoinSessionEvent");

//**********************************************************************************************************************
bool JoinSessionEvent::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_session_id) == false) return false;
	if (_client.ReadFromNetworkData(reader, from_addr) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool JoinSessionEvent::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_session_id) == false) return false;
	if (_client.WriteToNetworkData(writer) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId LeaveSessionRequest::dataContainerId = uu::StringId("LeaveSessionRequest");

//**********************************************************************************************************************
bool LeaveSessionRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_session_id) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool LeaveSessionRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_session_id) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId LeaveSessionAnswer::dataContainerId = uu::StringId("LeaveSessionAnswer");

//**********************************************************************************************************************
bool LeaveSessionAnswer::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadBool(_result) == false) return false;
	if (reader.ReadUInt32(_session_id) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool LeaveSessionAnswer::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteBool(_result) == false) return false;
	if (writer.WriteUInt32(_session_id) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId LeaveSessionEvent::dataContainerId = uu::StringId("LeaveSessionEvent");

//**********************************************************************************************************************
bool LeaveSessionEvent::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_session_id) == false) return false;
	if (_client.ReadFromNetworkData(reader, from_addr) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool LeaveSessionEvent::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_session_id) == false) return false;
	if (_client.WriteToNetworkData(writer) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId GetSessionsListRequest::dataContainerId = uu::StringId("GetSessionsListRequest");

//**********************************************************************************************************************
bool GetSessionsListRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool GetSessionsListRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId GetSessionsListAnswer::dataContainerId = uu::StringId("GetSessionsListAnswer");

//**********************************************************************************************************************
bool GetSessionsListAnswer::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadByte(_nb_sessions) == false) return false;

	_sessions.clear();

	uu::u32 session_id;
	while (reader.ReadUInt32(session_id) == true)
	{
		_sessions.push_back(session_id);
	}

	return true;
}

//**********************************************************************************************************************
bool GetSessionsListAnswer::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteByte(_nb_sessions) == false) return false;

	for(auto it = _sessions.begin(); it !=_sessions.end(); it++)
	{
		if (writer.WriteUInt32(*it) == false) return false;
	}

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId GetSessionInfoRequest::dataContainerId = uu::StringId("GetSessionInfoRequest");

//**********************************************************************************************************************
bool GetSessionInfoRequest::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_session_id) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool GetSessionInfoRequest::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_session_id) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId GetSessionInfoAnswer::dataContainerId = uu::StringId("GetSessionInfoAnswer");

//**********************************************************************************************************************
bool GetSessionInfoAnswer::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadBool(_result) == false) return false;
	if (_descriptor.ReadFromNetworkData(reader, from_addr) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool GetSessionInfoAnswer::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteBool(_result) == false) return false;
	if (_descriptor.WriteToNetworkData(writer) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId CloseSessionEvent::dataContainerId = uu::StringId("CloseSessionEvent");

//**********************************************************************************************************************
bool CloseSessionEvent::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_session_id) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool CloseSessionEvent::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_session_id) == false) return false;

	return true;
}
