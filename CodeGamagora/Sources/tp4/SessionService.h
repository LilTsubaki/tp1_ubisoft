//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "Singleton.h"
#include "IPEndPoint.h"
#include "UdpClient.h"
#include "DataContainer.h"
#include "Win32Console.h"

//**********************************************************************************************************************
class Configuration: public Singleton<Configuration>
{
public:
	Configuration();

	bool ReadFromFile(const char* fileName);

protected:
	static int _handler_ini(void* user, const char* section, const char* name, const char* value);

public:
	// local socket configuration
	bool		_bBlocking;				// socket blocking mode
	bool		_bBroadcast;			// socket broadcast mode
	uu::u32		_listenIP;				// socket local ip to bind
	uu::u16		_listenPort;			// socket local port to bind

	// server configuration
	uu::u16		_server_port;			// server listening port

	// client configuration
	uu::u32		_joining_session_id;		// client joining session id
	time_t		_client_leave_after_create;	// time when client leaves session after create
	time_t		_client_leave_after_join;	// time when client leaves session after join

	// common service/session configuration
	std::string	_service_name;			// service name
	uu::u8		_max_players;			// session max players
	time_t		_c2s_echo_ttl;			// client to server echo sent period, in ms
	time_t		_s2c_echo_ttl;			// server echo check period, in ms

	// game area size
	uu::u32		_area_width;
	uu::u32		_area_height;
	time_t		_timer_spawn_enemy;		// timer period to spawn enemy
	time_t		_timer_spawn_coin;		// timer period to spawn coins
};

//**********************************************************************************************************************
class SessionClient: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	SessionClient();
	SessionClient(std::string const& _player_name, uu::network::IPEndPoint const& _addr, bool bIsHost);
	SessionClient(SessionClient const& other);
	SessionClient& operator=(SessionClient const& other);

	bool operator==(SessionClient const& other) const;
	bool operator!=(SessionClient const& other) const;

	bool IsHost() const { return _bIsHost; }
	bool IsLocal() const { return _bIsLocal; }

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return SessionClient::dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::network::IPEndPoint _addr;
	std::string _player_name;
	bool _bIsHost;

public:
	// not serialized
	bool _bIsLocal;
	time_t _last_echo;
	bool _bToRemove;
};

//**********************************************************************************************************************
class SessionDescriptor: public uu::network::DataContainer
{
	friend class SessionService;
	friend class ServerService;
	friend class ClientService;

public:
	static uu::StringId dataContainerId;

public:
	SessionDescriptor();
	~SessionDescriptor();
	SessionDescriptor(SessionDescriptor const& other);
	SessionDescriptor& operator=(SessionDescriptor const& other);

	SessionClient* GetSessionHost() const;
	SessionClient* GetSessionClient(uu::network::IPEndPoint const& addr) const;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return SessionDescriptor::dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

protected:
	SessionClient* _SetSessionClientAsLocal(SessionClient const& client);
	void _AddClient(SessionClient const& client);
	void _AddClient(std::string const& _player_name, uu::network::IPEndPoint const& _addr, bool bIsHost);
	void _RemoveClient(SessionClient const& client);
	void _RemoveClient(uu::network::IPEndPoint const& _addr);

public:
	uu::u32 _session_id;
	uu::u8 _max_players;
	uu::u8 _nb_players;
	time_t _time_start;
	time_t _time_update;

public:
	// not serialized
	bool _bToRemove;

protected:
	std::vector<SessionClient> _clients;

};

//**********************************************************************************************************************
class INetworkDataListener
{
public:
	virtual bool OnNetworkData(uu::u32 dataContainerId, void* bytes, int size, uu::network::IPEndPoint const& from_addr) = 0;
};

//**********************************************************************************************************************
class ISessionListener
{
public:
	virtual void OnEnterSession(SessionDescriptor const& descriptor) = 0;
	virtual void OnLeaveSession(SessionDescriptor const& descriptor) = 0;
	virtual void OnClientEnterSession(SessionDescriptor const& descriptor, SessionClient const& client) = 0;
	virtual void OnClientLeaveSession(SessionDescriptor const& descriptor, SessionClient const& client) = 0;
	virtual void OnDiscoverSession(SessionDescriptor const& descriptor) = 0;
	virtual void OnCloseSession(SessionDescriptor const& descriptor) = 0;

};

//**********************************************************************************************************************
class SessionService
{
public:
	SessionService();
	virtual ~SessionService();

	virtual bool Initialize();
	virtual bool FirstUpdate();
	virtual bool Update();
	virtual bool Terminate();

	SessionClient const& GetLocalSessionClient() const { return _local_client; }

	void AddNetworkDataListener(INetworkDataListener* listener);
	void RemoveNetworkDataListener(INetworkDataListener* listener);
	void AddSessionListener(ISessionListener* listener);
	void RemoveSessionListener(ISessionListener* listener);

	bool SendDataContainerToSessionExclude(uu::network::DataContainer& datacontainer, SessionDescriptor const& session, uu::network::IPEndPoint const& exclude_addr);
	bool SendDataContainerToSession(uu::network::DataContainer& datacontainer, SessionDescriptor const& session);
	bool SendDataContainer(uu::network::DataContainer& datacontainer, uu::network::IPEndPoint const& to_addr);

protected:

	bool _PassDataToListeners(uu::u32 dataContainerId, void* bytes, int size, uu::network::IPEndPoint const& from_addr);

protected:
	uu::network::UdpClient _udpClient;
	SessionClient _local_client;

	std::vector<INetworkDataListener*> _data_listeners;
	std::vector<ISessionListener*> _session_listeners;

};
