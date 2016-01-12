//**********************************************************************************************************************
#include <stdio.h>

#include "session_datacontainer.h"
#include "ClientService.h"
#include "Time.h"
#include "Log.h"

const char* _static_session_state_strings[] = { "eNone", "eResolveServerAddr", "eResolvingServerAddr", "eGetSessionsList", "eGettingSessionsList", "eGetSessionInfo", "eGettingSessionInfo", "eCreateSession", "eCreatingSession", "eJoinSession", "eJoiningSession", "eInSession", "eLeaveSession", "eLeavingSession", "eTerminate" };

//**********************************************************************************************************************
ClientService::ClientService(): SessionService(),
	_state(eState::eNone),
	_next_state(eState::eNone),
	_time_now(0),
	_last_check(0),
	_last_server_echo(0),
	_client_leave_after(0)
{
}

//**********************************************************************************************************************
ClientService::~ClientService()
{
}

//**********************************************************************************************************************
bool ClientService::FirstUpdate()
{
	char tmp[256];
	sprintf_s(tmp, sizeof(tmp), "client_%d", GetCurrentThreadId());

	_local_client._player_name = tmp;
	_local_client._addr = _udpClient.GetLocalEndPoint();
	_local_client._bIsLocal = true;

	_time_now = uu::Time::GetSynchTime();

	if (Configuration::GetInstance()._joining_session_id != 0)
	{
		_client_leave_after = Configuration::GetInstance()._client_leave_after_join;
	}
	else
	{
		_client_leave_after = Configuration::GetInstance()._client_leave_after_create;
	}

	_ChangeState(eResolveServerAddr);

	return true;
}

//**********************************************************************************************************************
bool ClientService::Update()
{
	char buffer[1300];
	int recv_size;
	uu::network::IPEndPoint from_addr;

	_time_now = uu::Time::GetSynchTime();

	// change state machine if needs
	if (_next_state != _state)
	{
		Log(LogType::eTrace, LogModule::eClient, true, "changing state %s -> %s\n", _static_session_state_strings[_state], _static_session_state_strings[_next_state]);

		if (_current_session._session_id != 0)
		{
			Win32Console::SetTitle("SessionClient \"%s\" -- %s -- session_id = %lu", _local_client._player_name.c_str(), _static_session_state_strings[_next_state], _current_session._session_id);
		}
		else
		{
			Win32Console::SetTitle("SessionClient \"%s\" -- %s", _local_client._player_name.c_str(), _static_session_state_strings[_next_state]);
		}

		_state = _next_state;
		_next_state = _state;
		_last_check = _time_now;

		if (_state == eInSession)
		{
			// notify session listeners
			for (auto it = _session_listeners.begin(); it != _session_listeners.end(); it++)
			{
				(*it)->OnEnterSession(_current_session);
			}
		}
	}

	while (_udpClient.IsReadable(10) && _udpClient.RecvFrom(buffer, sizeof(buffer), recv_size, from_addr) == true)
	{
		if (recv_size > 0)
			_OnNetworkData(buffer, recv_size, from_addr);
	}
	
	if ((_time_now -_last_server_echo) > Configuration::GetInstance()._c2s_echo_ttl)
	{
		if (_EchoToServer() == true)
		{
			_last_server_echo = _time_now;
		}
	}

	switch(_state)
	{
	case eResolveServerAddr:	return _UpdateResolveServerAddr();		break;
	case eResolvingServerAddr:	return _UpdateResolvingServerAddr();	break;
	case eGetSessionsList:		return _UpdateGetSessionsList();		break;
	case eGettingSessionsList:	return _UpdateGettingSessionsList();	break;
	case eGetSessionInfo:		return _UpdateGetSessionInfo();			break;
	case eGettingSessionInfo:	return _UpdateGettingSessionInfo();		break;
	case eCreateSession:		return _UpdateCreateSession();			break;
	case eCreatingSession:		return _UpdateCreatingSession();		break;
	case eJoinSession:			return _UpdateJoinSession();			break;
	case eJoiningSession:		return _UpdateJoiningSession();			break;
	case eInSession:			return _UpdateInSession();				break;
	case eLeaveSession:			return _UpdateLeaveSession();			break;
	case eLeavingSession:		return _UpdateLeavingSession();			break;
	case eTerminate:			return _UpdateTerminate();				break;
	}

	return true;
}

//**********************************************************************************************************************
bool ClientService::Terminate()
{
	_UpdateLeaveSession();

	return SessionService::Terminate();
}

//**********************************************************************************************************************
bool ClientService::CreateSession()
{
	_ChangeState(eCreateSession);
	_last_check = 0;

	return true;
}

//**********************************************************************************************************************
bool ClientService::JoinSession(uu::u32 session_id)
{
	Configuration::GetInstance()._joining_session_id = session_id;
	_last_check = 0;

	return true;
}

//**********************************************************************************************************************
bool ClientService::LeaveSession()
{
	_ChangeState(eLeaveSession);
	_last_check = 0;

	return true;
}

//**********************************************************************************************************************
void ClientService::_ChangeState(eState new_state)
{
	if (_state != new_state && _next_state != new_state)
	{
		_next_state = new_state;
	}
}

//**********************************************************************************************************************
void ClientService::_OnNetworkData(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eClient, true, "%lu bytes received from %s\n", size, from_addr.ToString());

	uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);

	uu::u32 dataContainerId;
	reader.ReadUInt32(dataContainerId);
	reader.Reset();

	if (EchoAnswer::dataContainerId == dataContainerId)
	{
		if (eResolvingServerAddr == _state)
		{
			EchoAnswer answer;

			if (answer.ReadFromNetworkData(reader, from_addr) == false)
			{
				Log(LogType::eError, LogModule::eClient, true, "unable to read datacontainer EchoAnswer\n");

				_ChangeState(eTerminate);
				return;
			}

			_OnEchoAnswer(answer, from_addr);
		}
	}
	else if (CreateSessionAnswer::dataContainerId == dataContainerId)
	{
		if (eCreatingSession == _state)
		{
			CreateSessionAnswer answer;

			if (answer.ReadFromNetworkData(reader, from_addr) == false)
			{
				Log(LogType::eError, LogModule::eClient, true, "unable to read datacontainer CreateSessionAnswer\n");

				_ChangeState(eTerminate);
				return;
			}

			_OnCreateSessionAnswer(answer, from_addr);
		}
	}
	else if (JoinSessionAnswer::dataContainerId == dataContainerId)
	{
		if (eJoiningSession == _state)
		{
			JoinSessionAnswer answer;

			if (answer.ReadFromNetworkData(reader, from_addr) == false)
			{
				Log(LogType::eError, LogModule::eClient, true, "unable to read datacontainer JoinSessionAnswer\n");

				_ChangeState(eTerminate);
				return;
			}

			_OnJoinSessionAnswer(answer, from_addr);
		}
	}
	else if (LeaveSessionAnswer::dataContainerId == dataContainerId)
	{
		if (eLeavingSession == _state)
		{
			LeaveSessionAnswer answer;

			if (answer.ReadFromNetworkData(reader, from_addr) == false)
			{
				Log(LogType::eError, LogModule::eClient, true, "unable to read datacontainer LeaveSessionAnswer\n");

				_ChangeState(eTerminate);
				return;
			}

			_OnLeaveSessionAnswer(answer, from_addr);
		}
	}
	else if (GetSessionInfoAnswer::dataContainerId == dataContainerId)
	{
		if (eGettingSessionInfo == _state || eGettingSessionsList == _state)
		{
			GetSessionInfoAnswer answer;

			if (answer.ReadFromNetworkData(reader, from_addr) == false)
			{
				Log(LogType::eError, LogModule::eClient, true, "unable to read datacontainer GetSessionInfoAnswer\n");

				_ChangeState(eTerminate);
				return;
			}

			_OnGetSessionInfoAnswer(answer, from_addr);
		}
	}
	else if (GetSessionsListAnswer::dataContainerId == dataContainerId)
	{
		if (eGettingSessionsList == _state)
		{
			GetSessionsListAnswer answer;

			if (answer.ReadFromNetworkData(reader, from_addr) == false)
			{
				Log(LogType::eError, LogModule::eClient, true, "unable to read datacontainer GetSessionsListAnswer\n");

				_ChangeState(eTerminate);
				return;
			}

			_OnGetSessionsListAnswer(answer, from_addr);
		}
	}
	else if (JoinSessionEvent::dataContainerId == dataContainerId)
	{
		if (eInSession == _state)
		{
			JoinSessionEvent event;

			if (event.ReadFromNetworkData(reader, from_addr) == false)
			{
				Log(LogType::eError, LogModule::eClient, true, "unable to read datacontainer JoinSessionEvent\n");

				_ChangeState(eTerminate);
				return;
			}

			_OnJoinSessionEvent(event, from_addr);
		}
	}
	else if (LeaveSessionEvent::dataContainerId == dataContainerId)
	{
		if (eInSession == _state)
		{
			LeaveSessionEvent event;

			if (event.ReadFromNetworkData(reader, from_addr) == false)
			{
				Log(LogType::eError, LogModule::eClient, true, "unable to read datacontainer LeaveSessionEvent\n");

				_ChangeState(eTerminate);
				return;
			}

			_OnLeaveSessionEvent(event, from_addr);
		}
	}
	else if (CloseSessionEvent::dataContainerId == dataContainerId)
	{
		if (eInSession == _state)
		{
			CloseSessionEvent event;

			if (event.ReadFromNetworkData(reader, from_addr) == false)
			{
				Log(LogType::eError, LogModule::eClient, true, "unable to read datacontainer CloseSessionEvent\n");

				_ChangeState(eTerminate);
				return;
			}

			_OnCloseSessionEvent(event, from_addr);
		}
	}
	else if (_PassDataToListeners(dataContainerId, bytes, size, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eClient, true, "unknown datacontainer\n");
	}
}

//**********************************************************************************************************************
bool ClientService::_UpdateResolveServerAddr()
{
	EchoRequest request;
	request._service_name = Configuration::GetInstance()._service_name;

	//uu::network::IPAddress ip(10,2,6,8);
	//uu::network::IPAddress ip("159.84.141.84");
	uu::network::IPAddress ip = uu::network::IPAddress::Broadcast;

	if (SendDataContainer(request, uu::network::IPEndPoint(ip, Configuration::GetInstance()._server_port)) == true)
	{
		_ChangeState(eResolvingServerAddr);
	}
	else
	{
		Log(LogType::eError, LogModule::eClient, true, "unable to send EchoRequest\n");

		_ChangeState(eTerminate);
	}

	return true;
}

//**********************************************************************************************************************
bool ClientService::_UpdateResolvingServerAddr()
{
	if ((_time_now -_last_check) > 5000)
	{
		_ChangeState(eResolveServerAddr);
	}

	return true;
}

//**********************************************************************************************************************
void ClientService::_OnEchoAnswer(EchoAnswer const& answer, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eDataContainer, true, "EchoAnswer received from %s  yourip=%s\n", from_addr.ToString(), answer._your_addr.ToString());

	_local_client._addr = answer._your_addr;
	_server_addr = answer._service_addr;
	_server_addr = from_addr;

	if (Configuration::GetInstance()._joining_session_id != 0)
	{
		_ChangeState(eGetSessionInfo);
	}
	else
	{
		_ChangeState(eGetSessionsList);
	}
}

//**********************************************************************************************************************
bool ClientService::_UpdateGetSessionInfo()
{
	GetSessionInfoRequest request;
	request._session_id = Configuration::GetInstance()._joining_session_id;

	if (SendDataContainer(request, _server_addr) == true)
	{
		_ChangeState(eGettingSessionInfo);
	}
	else
	{
		Log(LogType::eError, LogModule::eClient, true, "unable to send GetSessionInfoRequest\n");

		_ChangeState(eTerminate);
	}

	return true;
}

//**********************************************************************************************************************
bool ClientService::_UpdateGetSessionsList()
{
	GetSessionsListRequest request;

	if (SendDataContainer(request, _server_addr) == true)
	{
		_ChangeState(eGettingSessionsList);
	}
	else
	{
		Log(LogType::eError, LogModule::eClient, true, "unable to send GetSessionsListRequest\n");

		_ChangeState(eTerminate);
	}

	return true;
}

//**********************************************************************************************************************
bool ClientService::_UpdateGettingSessionsList()
{
	/*if ((_time_now -_last_check) > 10000)
	{
		_ChangeState(eCreateSession);
	}*/

	return true;
}

//**********************************************************************************************************************
void ClientService::_OnGetSessionsListAnswer(GetSessionsListAnswer const& answer, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eDataContainer, true, "GetSessionsListAnswer received from %s\n", from_addr.ToString());

	if (answer._nb_sessions > 0)
	{
		GetSessionInfoRequest request;

		for (auto it = answer._sessions.begin(); it != answer._sessions.end(); it++)
		{
			request._session_id = *it;

			if (SendDataContainer(request, _server_addr) == false)
			{
				Log(LogType::eError, LogModule::eClient, true, "unable to send GetSessionInfoRequest\n");

				_ChangeState(eTerminate);
			}
		}

		_ChangeState(eGettingSessionInfo);
	}
	/*else
	{
		_ChangeState(eCreateSession);
	}*/
}

//**********************************************************************************************************************
bool ClientService::_UpdateGettingSessionInfo()
{
	if (Configuration::GetInstance()._joining_session_id != 0)
	{
		if ((_time_now -_last_check) > 5000)
		{
			_ChangeState(eGetSessionInfo);
		}
	}
	else
	{
		// waiting command (create / join)
	}

	return true;
}

//**********************************************************************************************************************
void ClientService::_OnGetSessionInfoAnswer(GetSessionInfoAnswer const& answer, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eDataContainer, true, "GetSessionInfoAnswer received from %s\n", from_addr.ToString());

	if (answer._result == true)
	{
		if (Configuration::GetInstance()._joining_session_id != 0)
		{
			_ChangeState(eJoinSession);
		}
		else
		{
			// notify session listeners
			for (auto it = _session_listeners.begin(); it != _session_listeners.end(); it++)
			{
				(*it)->OnDiscoverSession(answer._descriptor);
			}
		}
	}
	else
	{
		Log(LogType::eError, LogModule::eClient, true, "unable to get session info %lu\n", answer._descriptor._session_id);

		_ChangeState(eTerminate);
	}
}

//**********************************************************************************************************************
bool ClientService::_UpdateCreateSession()
{
	CreateSessionRequest request;
	request._host_player_name = _local_client._player_name;
	request._max_players = Configuration::GetInstance()._max_players;

	if (SendDataContainer(request, _server_addr) == true)
	{
		_ChangeState(eCreatingSession);
	}
	else
	{
		Log(LogType::eError, LogModule::eClient, true, "unable to send CreateSessionRequest\n");

		_ChangeState(eTerminate);
	}

	return true;
}

//**********************************************************************************************************************
bool ClientService::_UpdateCreatingSession()
{
	/*if ((_time_now -_last_check) > 5000)
	{
		_ChangeState(eCreateSession);
	}*/

	return true;
}

//**********************************************************************************************************************
void ClientService::_OnCreateSessionAnswer(CreateSessionAnswer const& answer, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eDataContainer, true, "CreateSessionAnswer received from %s\n", from_addr.ToString());

	if (answer._result == true)
	{
		_current_session = answer._descriptor;
		_current_session._SetSessionClientAsLocal(_local_client);

		Log(LogType::eTrace, LogModule::eClient, true, "in session %lu\n", _current_session._session_id);

		_ChangeState(eInSession);
	}
	else
	{
		Log(LogType::eError, LogModule::eClient, true, "unable to create session\n");

		_ChangeState(eTerminate);
	}
}

//**********************************************************************************************************************
bool ClientService::_UpdateJoinSession()
{
	JoinSessionRequest request;
	request._session_id = Configuration::GetInstance()._joining_session_id;
	request._player_name = _local_client._player_name;

	if (SendDataContainer(request, _server_addr) == true)
	{
		_ChangeState(eJoiningSession);
	}
	else
	{
		Log(LogType::eError, LogModule::eClient, true, "unable to send JoinSessionRequest\n");

		_ChangeState(eTerminate);
	}

	return true;
}

//**********************************************************************************************************************
bool ClientService::_UpdateJoiningSession()
{
	if ((_time_now -_last_check) > 5000)
	{
		_ChangeState(eJoinSession);
	}

	return true;
}

//**********************************************************************************************************************
void ClientService::_OnJoinSessionAnswer(JoinSessionAnswer const& answer, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eDataContainer, true, "JoinSessionAnswer received from %s\n", from_addr.ToString());

	if (answer._result == true)
	{
		_current_session = answer._descriptor;
		_current_session._SetSessionClientAsLocal(_local_client);

		Log(LogType::eTrace, LogModule::eClient, true, "in session %lu\n", _current_session._session_id);

		for(auto it = _current_session._clients.begin(); it != _current_session._clients.end(); it++)
		{
			SessionClient const& client = *it;
			Log(LogType::eError, LogModule::eClient, true, "  client %s  name=%s  isHost=%s  isLocal=%s\n",
				client._addr.ToString(),
				client._player_name.c_str(),
				client._bIsHost == true ? "true" : "false",
				client._bIsLocal == true ? "true" : "false");
		}

		_ChangeState(eInSession);
	}
	else
	{
		Log(LogType::eError, LogModule::eClient, true, "unable to join session %lu\n", answer._descriptor._session_id);

		_ChangeState(eTerminate);
	}
}

//**********************************************************************************************************************
bool ClientService::_UpdateInSession()
{
	if (_client_leave_after > 0 && ((_time_now -_last_check) > _client_leave_after))
	{
		_ChangeState(eLeaveSession);
	}

	return true;
}

//**********************************************************************************************************************
bool ClientService::_UpdateLeaveSession()
{
	if (_current_session._session_id > 0)
	{
		LeaveSessionRequest request;
		request._session_id = _current_session._session_id;

		if (SendDataContainer(request, _server_addr) == true)
		{
			_ChangeState(eLeavingSession);
		}
		else
		{
			Log(LogType::eError, LogModule::eClient, true, "unable to send LeaveSessionRequest\n");

			_ChangeState(eTerminate);
		}
	}
	else
	{
		_ChangeState(eTerminate);
	}

	return true;
}

//**********************************************************************************************************************
bool ClientService::_UpdateLeavingSession()
{
	if ((_time_now -_last_check) > 5000)
	{
		_ChangeState(eTerminate);
	}

	return true;
}

//**********************************************************************************************************************
void ClientService::_OnLeaveSessionAnswer(LeaveSessionAnswer const& answer, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eDataContainer, true, "LeaveSessionAnswer received from %s\n", from_addr.ToString());

	if (answer._result == true)
	{
	}
	else
	{
	}

	_ChangeState(eTerminate);
}

//**********************************************************************************************************************
bool ClientService:: _UpdateTerminate()
{
	// notify session listeners
	for (auto it = _session_listeners.begin(); it != _session_listeners.end(); it++)
	{
		(*it)->OnLeaveSession(_current_session);
	}

	Win32Console::PrintColor(OLIVE, "End of ClientService\n");

	return false;	// end of loop of the client
}

//**********************************************************************************************************************
void ClientService::_OnJoinSessionEvent(JoinSessionEvent const& event, uu::network::IPEndPoint const& from_addr)
{
	if (_current_session._session_id == event._session_id)
	{
		Log(LogType::eTrace, LogModule::eDataContainer, true, "JoinSessionEvent received from %s\n", from_addr.ToString());

		_current_session._AddClient(event._client);

		// notify session listeners
		for (auto it = _session_listeners.begin(); it != _session_listeners.end(); it++)
		{
			(*it)->OnClientEnterSession(_current_session, event._client);
		}
	}
}

//**********************************************************************************************************************
void ClientService::_OnLeaveSessionEvent(LeaveSessionEvent const& event, uu::network::IPEndPoint const& from_addr)
{
	if (_current_session._session_id == event._session_id)
	{
		Log(LogType::eTrace, LogModule::eDataContainer, true, "LeaveSessionEvent received from %s\n", from_addr.ToString());

		_current_session._RemoveClient(event._client);

		// notify session listeners
		for (auto it = _session_listeners.begin(); it != _session_listeners.end(); it++)
		{
			(*it)->OnClientLeaveSession(_current_session, event._client);
		}
	}
}

//**********************************************************************************************************************
void ClientService::_OnCloseSessionEvent(CloseSessionEvent const& event, uu::network::IPEndPoint const& from_addr)
{
	if (_current_session._session_id == event._session_id)
	{
		Log(LogType::eTrace, LogModule::eDataContainer, true, "CloseSessionEvent received from %s\n", from_addr.ToString());

		// notify session listeners
		for (auto it = _session_listeners.begin(); it != _session_listeners.end(); it++)
		{
			(*it)->OnCloseSession(_current_session);
		}
	}
}

//**********************************************************************************************************************
bool ClientService::_EchoToServer()
{
	if (_server_addr.IsValid())
	{
		EchoEvent event;

		if (SendDataContainer(event, _server_addr) == true)
		{
			return true;
		}
	}

	return false;
}
