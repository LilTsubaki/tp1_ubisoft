//**********************************************************************************************************************
#pragma once

//**********************************************************************************************************************
namespace sf
{
class RenderWindow;
}

//**********************************************************************************************************************
#include "types.h"
#include "Singleton.h"

#include "draw_ui.h"
#include "ClientService.h"
#include "Entity.h"
#include "ResourceManager.h"
#include "World.h"
#include "Character.h"
#include "GameObjects.h"

//**********************************************************************************************************************
class Game: public Singleton<Game>, public INetworkDataListener, public ISessionListener
{
public:
	enum State
	{
		none,
		session,
		lobby,
		game,
		end,
	};

public:
	Game();
	virtual ~Game();

	bool Initialize();
	bool FirstUpdate();
	bool Update();
	bool Terminate();

	State GetState() const { return _state; }

	Entity* GetEntity(uu::u32 id) const;
	void GetEntitiesListFiltered(std::vector<Entity*>& list, uu::StringId const& type, bool bLocal, uu::u32 exclude_id) const;
	void GetEntitiesList(std::vector<Entity*>& list, uu::StringId const& type, uu::u32 exclude_id) const;

	SessionClient const& GetLocalSessionClient() const { return _session_service.GetLocalSessionClient(); }
	SessionClient* GetSessionHost() const { return _session_service.GetSessionHost(); }
	//SessionClient* GetSessionClient(uu::network::IPEndPoint const& addr) const { return _session_service.GetSessionClient(addr); }
	//SessionDescriptor const& GetCurrentSession() const { return _session_service.GetCurrentSession(); }

	Entity* TryPickEntity(sf::Vector2f const& point);

	Entity* CreateNetworkEntity(uu::StringId const& type, std::string const& name, uu::u32 id, uu::network::IPEndPoint const& owner);
	Entity* CreateLocalEntity(uu::StringId const& type, std::string const& name);
	Enemy* CreateLocalEnemy();

	bool SendDataContainerToSessionClients(uu::network::DataContainer& datacontainer);
	bool SendDataContainer(uu::network::DataContainer& datacontainer, uu::network::IPEndPoint const& to_addr);

	void DispatchLocalEntitiesToClient(SessionClient const& client);
	void DispatchCreateEntityToClient(Entity const& entity, SessionClient const& client);
	void DispatchCreateEntityToSessionClients(Entity const& entity);

	void AddWidget(Widget& widget);
	void RemoveWidget(Widget& widget);
	Widget* GetWidget(uu::u32 id) const;

protected:
	// state machine management
	State _state;
	State _next_state;
	void _ChangeState(State new_state);

	void _OnEnterStateSession();
	void _OnEnterStateLobby();
	void _OnEnterStateGame();
	void _OnEnterStateEnd();

	//network stuff
	// INetworkDataListener override
	virtual bool OnNetworkData(uu::u32 dataContainerId, void* bytes, int size, uu::network::IPEndPoint const& from_addr);

	// ISessionListener overrides
	virtual void OnEnterSession(SessionDescriptor const& descriptor);
	virtual void OnLeaveSession(SessionDescriptor const& descriptor);
	virtual void OnClientEnterSession(SessionDescriptor const& descriptor, SessionClient const& client);
	virtual void OnClientLeaveSession(SessionDescriptor const& descriptor, SessionClient const& client);
	virtual void OnDiscoverSession(SessionDescriptor const& descriptor);
	virtual void OnCloseSession(SessionDescriptor const& descriptor);

	void _OnCreateEntityRequest(void* bytes, int size, uu::network::IPEndPoint const& from_addr);
	void _OnCreatePlayerRequest(void* bytes, int size, uu::network::IPEndPoint const& from_addr);
	

	// SFML stuff
	void _OnDraw(sf::RenderTarget& target);
	void _OnInputEvent(sf::Event& event, sf::RenderWindow& window);
	void _OnUI(Widget& widget, sf::Event::MouseButtonEvent const& event);
	Widget* _TryPickWidget(sf::Vector2f const& point);
	void _CleanUI();

	void _AddEntity(Entity* entity);
	void _RemoveEntity(Entity* entity);
	Entity* _CreateEntityByType(uu::StringId const& type, std::string const& name, uu::u32 id, bool bLocal, uu::network::IPEndPoint const& owner);
	void _ManageEntities();
	void _DeleteEntitiesFromClient(SessionClient const& client);
	void _DeleteAllEntities();

	Player* _CreateLocalPlayer();

	void _RemoveEntities();

	class Timer
	{
	public:
		Timer();
		Timer(uu::StringId const& name, time_t duration);
		Timer(Timer const& other);
		Timer& operator=(Timer const& other);

		bool IsElapsed(time_t time) const;
		void ReArm(time_t duration);

		uu::StringId _name;
		time_t _duration;
		time_t _elapsed_time;
	};

	Timer& _AddTimer(uu::StringId const& name, time_t duration);
	void _RemoveTimer(Timer& timer);
	void _ManageTimers();
	void _CleanTimers();
	void _OnTimer(Timer& timer);

protected:
	time_t _time_now;
	uu::u32 _frame;
	bool _bInLoop;
	bool _bDebugDraw;

	World _world;

	// session game
	ClientService _session_service;
	std::list<SessionDescriptor> _discovered_sessions;

	// game entities
	std::list<Entity*> _entities;
	static uu::u32 _entity_id;

	// UI
	std::list<Widget*> _widgets;

	// SFML stuff
	ResourceManager _resourceManager;
	sf::RenderWindow* _window;
	sf::Font* _font;

	Player* _playerEntity;

	Label _label_game_state;
	Label _label_network_state;
	Button _create_session_button;

	// simple timers
	std::list<Timer*> _timers;
};
