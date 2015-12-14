//**********************************************************************************************************************
#pragma once

#include "types.h"

#include "serializer.h"
#include "DataContainer.h"
#include "StringId.h"
#include "IPEndPoint.h"
#include "SessionService.h"

//**********************************************************************************************************************
class EchoRequest: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	std::string _service_name;
	time_t _time_echo;
};

//**********************************************************************************************************************
class EchoAnswer: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	std::string _service_name;
	time_t _time_echo;
	uu::network::IPEndPoint _your_addr;
	uu::network::IPEndPoint _service_addr;
};

//**********************************************************************************************************************
class EchoEvent: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);
};

//**********************************************************************************************************************
class CreateSessionRequest: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	std::string _host_player_name;
	uu::u8 _max_players;
};

//**********************************************************************************************************************
class CreateSessionAnswer: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	bool _result;
	SessionDescriptor _descriptor;
};

//**********************************************************************************************************************
class JoinSessionRequest: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _session_id;
	std::string _player_name;
};

//**********************************************************************************************************************
class JoinSessionAnswer: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	bool _result;
	SessionDescriptor _descriptor;
};

//**********************************************************************************************************************
class JoinSessionEvent: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _session_id;
	SessionClient _client;
};

//**********************************************************************************************************************
class LeaveSessionRequest: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _session_id;
};

//**********************************************************************************************************************
class LeaveSessionAnswer: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	bool _result;
	uu::u32 _session_id;
};

//**********************************************************************************************************************
class LeaveSessionEvent: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _session_id;
	SessionClient _client;
};

//**********************************************************************************************************************
class GetSessionsListRequest: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
};

//**********************************************************************************************************************
class GetSessionsListAnswer: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u8 _nb_sessions;
	std::vector<uu::u32> _sessions;
};

//**********************************************************************************************************************
class GetSessionInfoRequest: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _session_id;
};

//**********************************************************************************************************************
class GetSessionInfoAnswer: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	bool _result;
	SessionDescriptor _descriptor;
};

//**********************************************************************************************************************
class CloseSessionEvent: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _session_id;
};
