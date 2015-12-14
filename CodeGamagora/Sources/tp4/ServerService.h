//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "SessionService.h"
#include "session_datacontainer.h"

//**********************************************************************************************************************
class SessionDescriptor;

//**********************************************************************************************************************
class ServerService: public SessionService
{
public:
	ServerService();
	~ServerService();

	virtual bool FirstUpdate();
	virtual bool Update();

protected:
	void _OnNetworkData(void* bytes, int size, uu::network::IPEndPoint const& from_addr);

	void _OnEchoRequest(EchoRequest const& request, uu::network::IPEndPoint const& from_addr);
	void _OnEchoEvent(EchoEvent const& event, uu::network::IPEndPoint const& from_addr);
	void _OnCreateSessionRequest(CreateSessionRequest const& request, uu::network::IPEndPoint const& from_addr);
	void _OnJoinSessionRequest(JoinSessionRequest const& request, uu::network::IPEndPoint const& from_addr);
	void _OnLeaveSessionRequest(LeaveSessionRequest const& request, uu::network::IPEndPoint const& from_addr);
	void _OnGetSessionInfoRequest(GetSessionInfoRequest const& request, uu::network::IPEndPoint const& from_addr);
	void _OnGetSessionsListRequest(GetSessionsListRequest const& request, uu::network::IPEndPoint const& from_addr);

	SessionDescriptor* _FindSession(uu::u32 _session_id) const;
	SessionDescriptor* _InsertSession(uu::network::IPEndPoint const& host_addr, std::string const& host_player_name, uu::u8 max_players);
	bool _RemoveSession(uu::u32 _session_id);

	void _ManageSessions();
	void _CleanSessions();

	void _NotifyCloseSession(SessionDescriptor const& session, uu::network::IPEndPoint const& from_addr);
	void _NotifySessionClientEnter(SessionDescriptor const& session, SessionClient const& client);
	void _NotifySessionClientLeave(SessionDescriptor const& session, SessionClient const& client);

protected:
	time_t _time_now;
	uu::u32 _session_id;

	typedef std::map<uu::u32, SessionDescriptor> sessions_map_t;
	sessions_map_t _sessions_list;
};
