
//**********************************************************************************************************************
#include "session_datacontainer.h"
#include "ClientService.h"
#include "ini.h"
#include "Time.h"
#include "Log.h"

//**********************************************************************************************************************
/*static*/ int Configuration::_handler_ini(void* user, const char* section, const char* name, const char* value)
{
	/*if (_stricmp(section, "server") == 0)
	{
		return 1;
	}*/

	return 0;
}

//**********************************************************************************************************************
Configuration::Configuration(): Singleton<Configuration>()
{
	// set default values

	// local socket configuration
	_bBlocking = false;
	_bBroadcast = false;
	_listenIP = INADDR_ANY;		// bind on all interfaces
	//_listenIP = INADDR_LOOPBACK;
	_listenPort = 0;

	// server configuration
	_server_port = 5555;

	// client configuration
	_joining_session_id = 0;
	_client_leave_after_create = 0;//40000;	// 40secs
	_client_leave_after_join = 0;//10000;	// 10secs

	// common service/session configuration
	_service_name = "SessionServerTP4";
	_max_players = 4;
	_c2s_echo_ttl = 5000; // 30secs
	_s2c_echo_ttl = (time_t)(_c2s_echo_ttl * 1.3f);

	// game area size
	_area_width = 720;
	_area_height = (uu::u32)(_area_width * 0.75f);
	_timer_spawn_enemy = 30000;	// 30secs
	_timer_spawn_coin = 30000;	// 30secs
}

//**********************************************************************************************************************
bool Configuration::ReadFromFile(const char* fileName)
{
	if (fileName == nullptr)
		return false;

	char fullFileName[MAX_PATH+1];
	DWORD err = GetFullPathName(fileName, MAX_PATH, fullFileName, nullptr);
	if (err == 0 || err > MAX_PATH)
		return false;

	fullFileName[err] = 0;

	if (ini_parse(fullFileName, Configuration::_handler_ini, nullptr)  < 0)
		return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId SessionDescriptor::dataContainerId = uu::StringId("SessionDescriptor");

//**********************************************************************************************************************
SessionDescriptor::SessionDescriptor():
	_session_id(0),
	_max_players(0),
	_nb_players(0),
	_time_start(0),
	_time_update(0),
	_bToRemove(false)
{
}

//**********************************************************************************************************************
SessionDescriptor::~SessionDescriptor()
{
	_clients.clear();
}

//**********************************************************************************************************************
SessionDescriptor::SessionDescriptor(SessionDescriptor const& other):
	_session_id(other._session_id),
	_max_players(other._max_players),
	_nb_players(other._nb_players),
	_time_start(other._time_start),
	_time_update(other._time_update),
	_bToRemove(other._bToRemove)
{
	for(auto it = other._clients.begin(); it != other._clients.end(); it++)
	{
		SessionClient const& client = *it;

		_clients.push_back(client);
	}
}

//**********************************************************************************************************************
SessionDescriptor& SessionDescriptor::operator=(SessionDescriptor const& other)
{
	_session_id = other._session_id;
	_max_players = other._max_players;
	_nb_players = other._nb_players;
	_time_start = other._time_start;
	_time_update = other._time_update;
	_bToRemove = other._bToRemove;

	for(auto it = other._clients.begin(); it != other._clients.end(); it++)
	{
		SessionClient const& client = *it;

		_AddClient(client);
	}

	return *this;
}

//**********************************************************************************************************************
void SessionDescriptor::_AddClient(SessionClient const& client)
{
	for(auto it = _clients.begin(); it != _clients.end(); it++)
	{
		if ((*it)._addr == client._addr)
		{
			(*it) = client;
			return;
		}
	}

	_clients.push_back(client);

	_nb_players = (uu::u8)_clients.size();
	_time_update = uu::Time::GetSynchTime();
}

//**********************************************************************************************************************
void SessionDescriptor::_AddClient(std::string const& _player_name, uu::network::IPEndPoint const& _addr, bool bIsHost)
{
	_AddClient(SessionClient(_player_name, _addr, bIsHost));
}

//**********************************************************************************************************************
void SessionDescriptor::_RemoveClient(SessionClient const& client)
{
	_RemoveClient(client._addr);
}

//**********************************************************************************************************************
void SessionDescriptor::_RemoveClient(uu::network::IPEndPoint const& _addr)
{
	for(auto it = _clients.begin(); it != _clients.end(); it++)
	{
		SessionClient& client = *it;

		if (client._addr == _addr)
		{
			if (client._bIsHost == true)
			{
				_clients.clear();
			}
			else
			{
				_clients.erase(it);
			}

			_nb_players = (uu::u8)_clients.size();
			_time_update = uu::Time::GetSynchTime();

			return;
		}
	}
}

//**********************************************************************************************************************
SessionClient* SessionDescriptor::GetSessionHost() const
{
	for(auto it = _clients.begin(); it != _clients.end(); it++)
	{
		SessionClient* client = (SessionClient*)&*it;

		if (client->_bIsHost == true)
			return client;
	}

	return nullptr;
}

//**********************************************************************************************************************
SessionClient* SessionDescriptor::_SetSessionClientAsLocal(SessionClient const& client)
{
	SessionClient* ptr = GetSessionClient(client._addr);
	if (ptr == nullptr)
		return nullptr;

	*ptr = client;

	return ptr;
}

//**********************************************************************************************************************
SessionClient* SessionDescriptor::GetSessionClient(uu::network::IPEndPoint const& addr) const
{
	Log(LogType::eError, LogModule::eSession, true, "GetSessionClient(%s)\n", addr.ToString());

	for(auto it = _clients.begin(); it != _clients.end(); it++)
	{
		SessionClient* client = (SessionClient*)&*it;

		if (client->_addr == addr)
			return client;
	}

	return nullptr;
}

//**********************************************************************************************************************
bool SessionDescriptor::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadUInt32(_session_id) == false) return false;
	if (reader.ReadByte(_max_players) == false) return false;
	if (reader.ReadByte(_nb_players) == false) return false;
	if (reader.ReadInt64(_time_start) == false) return false;
	if (reader.ReadInt64(_time_update) == false) return false;

	_clients.clear();

	SessionClient client;
	while (client.ReadFromNetworkData(reader, from_addr) == true)
	{
		_clients.push_back(client);
	}

	return true;
}

//**********************************************************************************************************************
bool SessionDescriptor::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteUInt32(_session_id) == false) return false;
	if (writer.WriteByte(_max_players) == false) return false;
	if (writer.WriteByte(_nb_players) == false) return false;
	if (writer.WriteInt64(_time_start) == false) return false;
	if (writer.WriteInt64(_time_update) == false) return false;

	for(auto it = _clients.begin(); it !=_clients.end(); it++)
	{
		SessionClient& client = *it;

		if (client.WriteToNetworkData(writer) == false) return false;
	}

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
uu::StringId SessionClient::dataContainerId = uu::StringId("SessionClient");

//**********************************************************************************************************************
SessionClient::SessionClient():
	_bIsHost(false),
	_bIsLocal(false),
	_last_echo(0),
	_bToRemove(false)
{
	_last_echo = uu::Time::GetSynchTime();
}

//**********************************************************************************************************************
SessionClient::SessionClient(std::string const& _player_name, uu::network::IPEndPoint const& _addr, bool bIsHost):
	_addr(_addr),
	_player_name(_player_name),
	_bIsHost(bIsHost),
	_bIsLocal(false),
	_last_echo(0),
	_bToRemove(false)
{
	_last_echo = uu::Time::GetSynchTime();
}

//**********************************************************************************************************************
SessionClient::SessionClient(SessionClient const& other):
	_addr(other._addr),
	_player_name(other._player_name),
	_bIsHost(other._bIsHost),
	_bIsLocal(other._bIsLocal),
	_last_echo(other._last_echo),
	_bToRemove(other._bToRemove)
{
}

//**********************************************************************************************************************
SessionClient& SessionClient::operator=(SessionClient const& other)
{
	_addr = other._addr;
	_player_name = other._player_name;
	_bIsHost = other._bIsHost;
	_bIsLocal = other._bIsLocal;
	_last_echo = other._last_echo;
	_bToRemove = other._bToRemove;

	return *this;
}

//**********************************************************************************************************************
bool SessionClient::operator==(SessionClient const& other) const
{
	return (_addr == other._addr);
}

//**********************************************************************************************************************
bool SessionClient::operator!=(SessionClient const& other) const
{
	return (_addr != other._addr);
}

//**********************************************************************************************************************
bool SessionClient::ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr)
{
	if (_ReadDataContainerId(reader) == false) return false;

	if (reader.ReadIPEndPoint(_addr) == false) return false;
	if (reader.ReadString(_player_name) == false) return false;
	if (reader.ReadBool(_bIsHost) == false) return false;

	return true;
}

//**********************************************************************************************************************
bool SessionClient::WriteToNetworkData(uu::Writer& writer)
{
	if (_WriteDataContainerId(writer) == false) return false;

	if (writer.WriteIPEndPoint(_addr) == false) return false;
	if (writer.WriteString(_player_name) == false) return false;
	if (writer.WriteBool(_bIsHost) == false) return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
SessionService::SessionService()
{
}

//**********************************************************************************************************************
SessionService::~SessionService()
{
	_data_listeners.clear();
	_session_listeners.clear();

	_udpClient.Close();
}

//**********************************************************************************************************************
void SessionService::AddNetworkDataListener(INetworkDataListener* listener)
{
	auto it = std::find(_data_listeners.begin(), _data_listeners.end(), listener);
	if (it == _data_listeners.end())
	{
		_data_listeners.push_back(listener);
	}
}

//**********************************************************************************************************************
void SessionService::RemoveNetworkDataListener(INetworkDataListener* listener)
{
	auto it = std::find(_data_listeners.begin(), _data_listeners.end(), listener);
	if (it != _data_listeners.end())
	{
		_data_listeners.erase(it);
	}
}

//**********************************************************************************************************************
void SessionService::AddSessionListener(ISessionListener* listener)
{
	auto it = std::find(_session_listeners.begin(), _session_listeners.end(), listener);
	if (it == _session_listeners.end())
	{
		_session_listeners.push_back(listener);
	}
}

//**********************************************************************************************************************
void SessionService::RemoveSessionListener(ISessionListener* listener)
{
	auto it = std::find(_session_listeners.begin(), _session_listeners.end(), listener);
	if (it != _session_listeners.end())
	{
		_session_listeners.erase(it);
	}
}

//**********************************************************************************************************************
bool SessionService::_PassDataToListeners(uu::u32 dataContainerId, void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	bool bResult = false;

	for (auto it = _data_listeners.begin(); it != _data_listeners.end(); it++)
	{
		bResult |= (*it)->OnNetworkData(dataContainerId, bytes, size, from_addr);
	}

	return bResult;
}

//**********************************************************************************************************************
bool SessionService::Initialize()
{
	Configuration& config = Configuration::GetInstance();

	uu::network::IPEndPoint local_bind(config._listenIP, config._listenPort);

	if (_udpClient.Bind(local_bind) == false)
	{
		Log(LogType::eError, LogModule::eSession, true, "unable to bind local port on %s\n", local_bind.ToString());
		return false;
	}

	Log(LogType::eTrace, LogModule::eSession, true, "local socket bound on %s\n",  _udpClient.GetLocalEndPoint().ToString());

	if (_udpClient.EnableBroadcast(config._bBroadcast) == false)
	{
		Log(LogType::eError, LogModule::eSession, true, "unable to en/disable broadcast on socket\n");
		return false;
	}

	if (_udpClient.Blocking(config._bBlocking) == false)
	{
		Log(LogType::eError, LogModule::eSession, true, "unable to en/disable blocking mode on socket\n");
		return false;
	}

	return true;
}

//**********************************************************************************************************************
bool SessionService::FirstUpdate()
{
	return true;
}

//**********************************************************************************************************************
bool SessionService::Update()
{
	return false;
}

//**********************************************************************************************************************
bool SessionService::Terminate()
{
	_udpClient.Close();

	return true;
}

//**********************************************************************************************************************
bool SessionService::SendDataContainerToSessionExclude(uu::network::DataContainer& datacontainer, SessionDescriptor const& session, uu::network::IPEndPoint const& exclude_addr)
{
	if (session._clients.empty() == true)
		return false;

	char buffer[1300];

	uu::Writer writer(buffer, sizeof(buffer), uu::Endianness::eNetworkEndian);

	if (datacontainer.WriteToNetworkData(writer) == false)
	{
		Log(LogType::eError, LogModule::eSession, true, "unable to write datacontainer \"%s\"\n", datacontainer.GetDataContainerId().Content().c_str());
		return false;
	}

	for (auto it = session._clients.begin();  it != session._clients.end(); it++)
	{
		if (_local_client._addr != (*it)._addr && (*it)._addr != exclude_addr)
		{
			if (_udpClient.SendTo((char*)writer.GetData(), (int)writer.GetSize(), (*it)._addr) == false)
			{
				return false;
			}
		}
	}

	return true;
}

//**********************************************************************************************************************
bool SessionService::SendDataContainerToSession(uu::network::DataContainer& datacontainer, SessionDescriptor const& session)
{
	if (session._clients.empty() == true)
		return false;

	char buffer[1300];

	uu::Writer writer(buffer, sizeof(buffer), uu::Endianness::eNetworkEndian);

	if (datacontainer.WriteToNetworkData(writer) == false)
	{
		Log(LogType::eError, LogModule::eSession, true, "unable to write datacontainer \"%s\"\n", datacontainer.GetDataContainerId().Content().c_str());
		return false;
	}

	for (auto it = session._clients.begin();  it != session._clients.end(); it++)
	{
		if (_local_client._addr != (*it)._addr)
		{
			if (_udpClient.SendTo((char*)writer.GetData(), (int)writer.GetSize(), (*it)._addr) == false)
			{
				return false;
			}
		}
	}

	return true;
}

//**********************************************************************************************************************
bool SessionService::SendDataContainer(uu::network::DataContainer& datacontainer, uu::network::IPEndPoint const& to_addr)
{
	char buffer[1300];

	uu::Writer writer(buffer, sizeof(buffer), uu::Endianness::eNetworkEndian);

	if (datacontainer.WriteToNetworkData(writer) == false)
	{
		Log(LogType::eError, LogModule::eSession, true, "unable to write datacontainer \"%s\"\n", datacontainer.GetDataContainerId().Content().c_str());
		return false;
	}

	Log(LogType::eTrace, LogModule::eClient, true, "sending %lu bytes to %s\n", writer.GetSize(), to_addr.ToString());

	return _udpClient.SendTo((char*)writer.GetData(), (int)writer.GetSize(), to_addr);
}

//**********************************************************************************************************************
