//**********************************************************************************************************************
#include <stdio.h>

#include "session_datacontainer.h"
#include "ServerService.h"
#include "Time.h"
#include "Log.h"

//**********************************************************************************************************************
ServerService::ServerService(): SessionService(),
	_session_id(0)
{
}

//**********************************************************************************************************************
ServerService::~ServerService()
{
	_sessions_list.clear();
}

//**********************************************************************************************************************
void ServerService::_CleanSessions()
{
	auto it_s = _sessions_list.begin();
	while (it_s != _sessions_list.end())
	{
		SessionDescriptor& session = it_s->second;

		if (session._bToRemove == true || session._clients.empty() == true)
		{
			Log(LogType::eWarning, LogModule::eServer, true, "remove session %lu\n", session._session_id);
			it_s = _sessions_list.erase(it_s);
		}
		else
		{
			auto it_c = session._clients.begin();
			while (it_c != session._clients.end())
			{
				SessionClient& client = *it_c;
				
				if (client._bToRemove == true)
				{
					Log(LogType::eWarning, LogModule::eServer, true, "remove client %s\n", client._addr.ToString());
					it_c = session._clients.erase(it_c);
				}
				else
				{
					it_c++;
				}
			}

			if (session._clients.empty() == true)
			{
				Log(LogType::eWarning, LogModule::eServer, true, "remove session %lu\n", session._session_id);
				it_s = _sessions_list.erase(it_s);
			}
			else
			{
				it_s++;
			}
		}
	}
}

//**********************************************************************************************************************
void ServerService::_ManageSessions()
{
	for (auto it_s = _sessions_list.begin(); it_s != _sessions_list.end(); it_s++)
	{
		SessionDescriptor& session = it_s->second;

		for (auto it_c = session._clients.begin(); it_c != session._clients.end(); it_c++)
		{
			SessionClient& client = *it_c;

			if ((_time_now - client._last_echo) > Configuration::GetInstance()._s2c_echo_ttl)
			{
				Log(LogType::eWarning, LogModule::eServer, true, "client %s has timed out...\n", client._addr.ToString());

				if (client._bIsHost == true)
				{
					// client is host, clean session
					_NotifyCloseSession(session, client._addr);

					session._bToRemove = true;
				}
				else
				{
					// client leaves session
					_NotifySessionClientLeave(session, client);

					client._bToRemove = true;
				}
			}
		}
	}

	_CleanSessions();

	Win32Console::SetTitle("SessionServer \"%s\" -- %lu sessions", _udpClient.GetLocalEndPoint().ToString(), _sessions_list.size());
}

//**********************************************************************************************************************
SessionDescriptor* ServerService::_InsertSession(uu::network::IPEndPoint const& host_addr, std::string const& host_player_name, uu::u8 max_players)
{
	SessionDescriptor session;

	session._session_id = ++_session_id;
	session._max_players = max_players;
	session._AddClient(host_player_name, host_addr, true);
	session._time_start = uu::Time::GetSynchTime();

	_sessions_list[_session_id] = session;
	//_sessions_list.insert(std::pair<uu::u32, SessionDescriptor*>(_session_id, session));

	return &_sessions_list[_session_id];
}

//**********************************************************************************************************************
bool ServerService::_RemoveSession(uu::u32 _session_id)
{
	auto it = _sessions_list.find(_session_id);

	if (it == _sessions_list.end())
	{
		Log(LogType::eWarning, LogModule::eServer, true, "unable to remove session %lu\n", _session_id);
		return false;
	}

	_sessions_list.erase(it);

	return true;
}

//**********************************************************************************************************************
SessionDescriptor* ServerService::_FindSession(uu::u32 _session_id) const
{
	auto it = _sessions_list.find(_session_id);

	if (it == _sessions_list.end())
	{
		Log(LogType::eWarning, LogModule::eServer, true, "unable to find session %lu\n", _session_id);
		return nullptr;
	}

	return (SessionDescriptor*)&(it->second);
}

//**********************************************************************************************************************
bool ServerService::FirstUpdate()
{
	//_session_id = (uu::u32)(uu::Time::GetEpochTimeMs() & 0x0000ffff);
	_session_id = GetCurrentThreadId();

	_time_now = uu::Time::GetSynchTime();

	_ManageSessions();

	return true;
}

//**********************************************************************************************************************
bool ServerService::Update()
{
	char buffer[1300];
	int recv_size;
	uu::network::IPEndPoint from_addr;

	while (_udpClient.RecvFrom(buffer, sizeof(buffer), recv_size, from_addr) == true)
	{
		_time_now = uu::Time::GetSynchTime();

		if (recv_size > 0)
			_OnNetworkData(buffer, recv_size, from_addr);

		_ManageSessions();
	}

	return false;	// end of loop of the server
}

//**********************************************************************************************************************
void ServerService::_OnNetworkData(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eServer, true, "%lu bytes received from %s\n", size, from_addr.ToString());

	uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);

	uu::u32 dataContainerId;
	reader.ReadUInt32(dataContainerId);
	reader.Reset();

	if (EchoRequest::dataContainerId == dataContainerId)
	{
		EchoRequest request;

		if (request.ReadFromNetworkData(reader, from_addr) == false)
		{
			Log(LogType::eError, LogModule::eServer, true, "unable to read datacontainer EchoRequest\n");
			return;
		}

		_OnEchoRequest(request, from_addr);
	}
	else if (EchoEvent::dataContainerId == dataContainerId)
	{
		EchoEvent event;

		if (event.ReadFromNetworkData(reader, from_addr) == false)
		{
			Log(LogType::eError, LogModule::eServer, true, "unable to read datacontainer EchoEvent\n");
			return;
		}

		_OnEchoEvent(event, from_addr);
	}
	else if (CreateSessionRequest::dataContainerId == dataContainerId)
	{
		CreateSessionRequest request;

		if (request.ReadFromNetworkData(reader, from_addr) == false)
		{
			Log(LogType::eError, LogModule::eServer, true, "unable to read datacontainer CreateSessionRequest\n");
			return;
		}

		_OnCreateSessionRequest(request, from_addr);
	}
	else if (JoinSessionRequest::dataContainerId == dataContainerId)
	{
		JoinSessionRequest request;

		if (request.ReadFromNetworkData(reader, from_addr) == false)
		{
			Log(LogType::eError, LogModule::eServer, true, "unable to read datacontainer JoinSessionRequest\n");
			return;
		}

		_OnJoinSessionRequest(request, from_addr);
	}
	else if (LeaveSessionRequest::dataContainerId == dataContainerId)
	{
		LeaveSessionRequest request;

		if (request.ReadFromNetworkData(reader, from_addr) == false)
		{
			Log(LogType::eError, LogModule::eServer, true, "unable to read datacontainer LeaveSessionRequest\n");
			return;
		}

		_OnLeaveSessionRequest(request, from_addr);
	}
	else if (GetSessionInfoRequest::dataContainerId == dataContainerId)
	{
		GetSessionInfoRequest request;

		if (request.ReadFromNetworkData(reader, from_addr) == false)
		{
			Log(LogType::eError, LogModule::eServer, true, "unable to read datacontainer GetSessionInfoRequest\n");
			return;
		}

		_OnGetSessionInfoRequest(request, from_addr);
	}
	else if (GetSessionsListRequest::dataContainerId == dataContainerId)
	{
		GetSessionsListRequest request;

		if (request.ReadFromNetworkData(reader, from_addr) == false)
		{
			Log(LogType::eError, LogModule::eServer, true, "unable to read datacontainer GetSessionsListRequest\n");
			return;
		}

		_OnGetSessionsListRequest(request, from_addr);
	}
	else
	{
		Log(LogType::eError, LogModule::eServer, true, "unknown datacontainer\n");
	}
}

//**********************************************************************************************************************
void ServerService::_OnEchoRequest(EchoRequest const& request, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eDataContainer, true, "EchoRequest received from %s\n", from_addr.ToString());

	if (_stricmp(request._service_name.c_str(), Configuration::GetInstance()._service_name.c_str()) == 0)
	{
		EchoAnswer answer;
		answer._service_name = Configuration::GetInstance()._service_name;
		answer._service_addr = _udpClient.GetLocalEndPoint();
		answer._your_addr = from_addr;

		//reply to request
		if (SendDataContainer(answer, from_addr) == false)
		{
			Log(LogType::eError, LogModule::eServer, true, "unable to send EchoAnswer\n");
		}
	}
}

//**********************************************************************************************************************
void ServerService::_OnEchoEvent(EchoEvent const& event, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eDataContainer, true, "EchoEvent received from %s\n", from_addr.ToString());

	for (auto it_s = _sessions_list.begin(); it_s != _sessions_list.end(); it_s++)
	{
		SessionDescriptor& session = it_s->second;

		for (auto it_c = session._clients.begin(); it_c != session._clients.end(); it_c++)
		{
				SessionClient& client = *it_c;

				if (client._addr == from_addr)
				{
					client._last_echo = _time_now;
				}
		}
	}
}

//**********************************************************************************************************************
void ServerService::_OnCreateSessionRequest(CreateSessionRequest const& request, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreateSessionRequest received from %s\n", from_addr.ToString());

	SessionDescriptor* session = _InsertSession(from_addr, request._host_player_name, request._max_players);

	CreateSessionAnswer answer;
	answer._result = false;

	if (session != nullptr)
	{
		answer._result = true;
		answer._descriptor = *session;
	}

	//reply to request
	if (SendDataContainer(answer, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eServer, true, "unable to send CreateSessionAnswer\n");
	}
}

//**********************************************************************************************************************
void ServerService::_OnJoinSessionRequest(JoinSessionRequest const& request, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eDataContainer, true, "JoinSessionRequest received from %s\n", from_addr.ToString());

	SessionClient* client = nullptr;

	JoinSessionAnswer answer;
	answer._result = false;
	answer._descriptor._session_id = request._session_id;

	SessionDescriptor* session = _FindSession(request._session_id);
	if (session != nullptr)
	{
		if (session->_nb_players < session->_max_players)
		{
			session->_AddClient(request._player_name, from_addr, false);

			client = session->GetSessionClient(from_addr);
			if (client != nullptr)
			{
				answer._result = true;
				answer._descriptor = *session;
			}
		}
	}

	//reply to request
	if (SendDataContainer(answer, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eServer, true, "unable to send JoinSessionAnswer\n");
	}
	else if (answer._result == true && client != nullptr)
	{
		_NotifySessionClientEnter(*session, *client);
	}
}

//**********************************************************************************************************************
void ServerService::_OnLeaveSessionRequest(LeaveSessionRequest const& request, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eServer, true, "LeaveSessionRequest received from %s\n", from_addr.ToString());

	LeaveSessionAnswer answer;
	answer._result = false;
	answer._session_id = request._session_id;

	SessionDescriptor* session = _FindSession(request._session_id);
	if (session != nullptr)
	{
		SessionClient* client = session->GetSessionClient(from_addr);
		if (client == nullptr)
		{
			// client not found
		}
		else
		{
			if (client->_bIsHost == true)
			{
				// client is host, clean session
				_NotifyCloseSession(*session, from_addr);

				session->_bToRemove = true;
				//_RemoveSession(session->_session_id);
			}
			else
			{
				// client leaves session
				_NotifySessionClientLeave(*session, *client);

				client->_bToRemove = true;
				//session->_RemoveClient(from_addr);
			}

			answer._result = true;
		}
	}

	//reply to request
	if (SendDataContainer(answer, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eServer, true, "unable to send LeaveSessionAnswer\n");
	}
}

//**********************************************************************************************************************
void ServerService::_OnGetSessionInfoRequest(GetSessionInfoRequest const& request, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eServer, true, "GetSessionInfoRequest received from %s\n", from_addr.ToString());

	GetSessionInfoAnswer answer;
	answer._result = false;
	answer._descriptor._session_id = request._session_id;

	SessionDescriptor* session = _FindSession(request._session_id);
	if (session != nullptr)
	{
		answer._result = true;
		answer._descriptor = *session;
	}

	//reply to request
	if (SendDataContainer(answer, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eServer, true, "unable to send GetSessionInfoAnswer\n");
	}
}

//**********************************************************************************************************************
void ServerService::_OnGetSessionsListRequest(GetSessionsListRequest const& request, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eServer, true, "GetSessionInfoRequest received from %s\n", from_addr.ToString());

	GetSessionsListAnswer answer;
	answer._nb_sessions = (uu::u8)_sessions_list.size();

	for (auto it = _sessions_list.begin(); it != _sessions_list.end(); it++)
	{
		answer._sessions.push_back(it->first);
	}

	//reply to request
	if (SendDataContainer(answer, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eServer, true, "unable to send GetSessionsListAnswer\n");
	}
}

//**********************************************************************************************************************
void ServerService::_NotifyCloseSession(SessionDescriptor const& session, uu::network::IPEndPoint const& from_addr)
{
	CloseSessionEvent event;
	event._session_id = session._session_id;

	if (SendDataContainerToSessionExclude(event, session, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eServer, true, "unable to send CloseSessionEvent\n");
	}
}

//**********************************************************************************************************************
void ServerService::_NotifySessionClientEnter(SessionDescriptor const& session, SessionClient const& client)
{
	JoinSessionEvent event;
	event._session_id = session._session_id;
	event._client = client;

	if (SendDataContainerToSessionExclude(event, session, client._addr) == false)
	{
		Log(LogType::eError, LogModule::eServer, true, "unable to send JoinSessionEvent\n");
	}
}

//**********************************************************************************************************************
void ServerService::_NotifySessionClientLeave(SessionDescriptor const& session, SessionClient const& client)
{
	LeaveSessionEvent event;
	event._session_id = session._session_id;
	event._client = client;

	if (SendDataContainerToSessionExclude(event, session, client._addr) == false)
	{
		Log(LogType::eError, LogModule::eServer, true, "unable to send LeaveSessionEvent\n");
	}
}
