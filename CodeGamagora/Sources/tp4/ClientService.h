//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "SessionService.h"
#include "session_datacontainer.h"

//**********************************************************************************************************************
class ClientService: public SessionService
{
public:
	ClientService();
	~ClientService();

	// SessionService overrides
	virtual bool FirstUpdate();
	virtual bool Update();
	virtual bool Terminate();

	enum eState
	{
		eNone = 0,
		eResolveServerAddr,
		eResolvingServerAddr,
		eGetSessionsList,
		eGettingSessionsList,
		eGetSessionInfo,
		eGettingSessionInfo,
		eCreateSession,
		eCreatingSession,
		eJoinSession,
		eJoiningSession,
		eInSession,
		eLeaveSession,
		eLeavingSession,
		eTerminate,
	};

	eState GetState() const { return _state; }

	SessionClient* GetSessionHost() const { return _current_session.GetSessionHost(); }
	SessionClient* GetSessionClient(uu::network::IPEndPoint const& addr) const { return _current_session.GetSessionClient(addr); }
	SessionDescriptor const& GetCurrentSession() const { return _current_session; }

	bool CreateSession();
	bool JoinSession(uu::u32 session_id);
	bool LeaveSession();

protected:
	void _OnNetworkData(void* bytes, int size, uu::network::IPEndPoint const& from_addr);

	void _OnEchoAnswer(EchoAnswer const& answer, uu::network::IPEndPoint const& from_addr);
	void _OnCreateSessionAnswer(CreateSessionAnswer const& answer, uu::network::IPEndPoint const& from_addr);
	void _OnJoinSessionAnswer(JoinSessionAnswer const& answer, uu::network::IPEndPoint const& from_addr);
	void _OnLeaveSessionAnswer(LeaveSessionAnswer const& answer, uu::network::IPEndPoint const& from_addr);
	void _OnGetSessionInfoAnswer(GetSessionInfoAnswer const& answer, uu::network::IPEndPoint const& from_addr);
	void _OnGetSessionsListAnswer(GetSessionsListAnswer const& answer, uu::network::IPEndPoint const& from_addr);
	void _OnJoinSessionEvent(JoinSessionEvent const& event, uu::network::IPEndPoint const& from_addr);
	void _OnLeaveSessionEvent(LeaveSessionEvent const& event, uu::network::IPEndPoint const& from_addr);
	void _OnCloseSessionEvent(CloseSessionEvent const& event, uu::network::IPEndPoint const& from_addr);

	void _ChangeState(eState new_state);

	bool _EchoToServer();

protected:
	uu::network::IPEndPoint _server_addr;
	SessionDescriptor _current_session;
	time_t _client_leave_after;

	// state machine management
	eState _state;
	eState _next_state;

	time_t _time_now;
	time_t _last_check;
	time_t _last_server_echo;

	bool _UpdateResolveServerAddr();
	bool _UpdateResolvingServerAddr();
	bool _UpdateGetSessionsList();
	bool _UpdateGettingSessionsList();
	bool _UpdateGetSessionInfo();
	bool _UpdateGettingSessionInfo();
	bool _UpdateCreateSession();
	bool _UpdateCreatingSession();
	bool _UpdateJoinSession();
	bool _UpdateJoiningSession();
	bool _UpdateInSession();
	bool _UpdateLeaveSession();
	bool _UpdateLeavingSession();
	bool _UpdateTerminate();
};
