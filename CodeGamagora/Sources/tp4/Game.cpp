//**********************************************************************************************************************
#include "types.h"
#include "Game.h"
#include "Time.h"
#include "game_datacontainer.h"
#include "GameObjects.h"
#include "Log.h"

//**********************************************************************************************************************
#ifdef _DEBUG
#pragma comment(lib, "sfml-system-s-d.lib")
#pragma comment(lib, "sfml-window-s-d.lib")
#pragma comment(lib, "sfml-graphics-s-d.lib")

#else //_DEBUG
#pragma comment(lib, "sfml-system-s.lib")
#pragma comment(lib, "sfml-window-s.lib")
#pragma comment(lib, "sfml-graphics-s.lib")

#endif //_DEBUG

#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")

#pragma comment(lib, "glew.lib")
#pragma comment(lib, "freetype.lib")
#pragma comment(lib, "jpeg.lib")

#include "SFML/Graphics.hpp"

//**********************************************************************************************************************
//**********************************************************************************************************************
extern const char* _static_session_state_strings[];

static  const char* _static_game_state_strings[] = { "none", "session", "lobby", "game", "end" };

//**********************************************************************************************************************
//**********************************************************************************************************************
/*static*/ uu::u32 Game::_entity_id = 0;

static uu::StringId _timer_spawn_enemy = uu::StringId("TimerSpawnEnemy");
static uu::StringId _timer_spawn_coin = uu::StringId("TimerSpawnCoin");
static uu::StringId _timer_refresh_point = uu::StringId("TimerRefreshPoint");

//**********************************************************************************************************************
//**********************************************************************************************************************
static bool compare_sprite_y(Entity* first, Entity* second)
{
	sf::Vector2f position_first;
	sf::Vector2f position_second;

	first->GetPosition(position_first);
	second->GetPosition(position_second);

	return (position_first.y < position_second.y);
}

//**********************************************************************************************************************
//**********************************************************************************************************************
Game::Timer::Timer():
	_duration(0),
	_elapsed_time(0)
{
}

//**********************************************************************************************************************
Game::Timer::Timer(uu::StringId const& name, time_t duration):
	_name(name),
	_duration(duration)
{
	_elapsed_time = uu::Time::GetSynchTime() + _duration;
}

//**********************************************************************************************************************
Game::Timer::Timer(Timer const& other):
	_name(other._name),
	_duration(other._duration),
	_elapsed_time(other._elapsed_time)
{
}

//**********************************************************************************************************************
Game::Timer& Game::Timer::operator=(Timer const& other)
{
	_name = other._name;
	_duration = other._duration;
	_elapsed_time = other._elapsed_time;

	return *this;
}

//**********************************************************************************************************************
bool Game::Timer::IsElapsed(time_t time) const
{
	return (time >= _elapsed_time);
}

//**********************************************************************************************************************
void Game::Timer::ReArm(time_t duration)
{
	_duration = duration;
	_elapsed_time = uu::Time::GetSynchTime() + _duration;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
Game::Game(): Singleton<Game>(),
	_state(none),
	_next_state(none),
	_time_now(0),
	_frame(0),
	_bInLoop(false),
	_bDebugDraw(false),
	_window(nullptr),
	_playerEntity(nullptr),
	isHost(false)
{
}

//**********************************************************************************************************************
Game::~Game()
{
	_CleanTimers();
	_CleanUI();
	_DeleteAllEntities();
	_discovered_sessions.clear();
}

//**********************************************************************************************************************
Player* Game::_CreateLocalPlayer()
{
	char tmp[256];
	sprintf_s(tmp, sizeof(tmp), "player_%d", GetCurrentThreadId());

	uu::StringId const type = GetRandomPlayerType();

	Entity* entity = CreateLocalEntity(type, tmp);

	return dynamic_cast<Player*>(entity);
}

//**********************************************************************************************************************
Enemy* Game::CreateLocalEnemy()
{
	char tmp[256];
	sprintf_s(tmp, sizeof(tmp), "enemy_%d", Game::_entity_id + 1);

	uu::StringId const type = GetRandomEnemyType();

	Entity* entity;
	entity = CreateLocalEntity(type, tmp);

	return dynamic_cast<Enemy*>(entity);
}

//**********************************************************************************************************************
void Game::_AddEntity(Entity* entity)
{
	Log(LogType::eTrace, LogModule::eGame, true, "[_AddEntity] entity=%s\n", entity->ToString());

	auto it = std::find(_entities.begin(), _entities.end(), entity);
	if (it == _entities.end())
	{
		_entities.push_back(entity);
	}
}

//**********************************************************************************************************************
void Game::_RemoveEntity(Entity* entity)
{
	Log(LogType::eTrace, LogModule::eGame, true, "[_RemoveEntity] entity=%s\n", entity->ToString());

	auto it = std::find(_entities.begin(), _entities.end(), entity);
	if (it != _entities.end())
	{
		if (_playerEntity == *it)
			_playerEntity = nullptr;

		if (_bInLoop == true)
		{
			Entity& entity = *(*it);

			entity.MarkForDelete();
		}
		else
		{
			_entities.erase(it);
		}
	}
}

//**********************************************************************************************************************
void Game::_RemoveEntities()
{
	auto it = _entities.begin();
	while (it != _entities.end())
	{
		Entity& entity = *(*it);

		if (entity._bRemove == true)
		{
			if (_playerEntity == *it)
				_playerEntity = nullptr;

			delete(*it);
			it = _entities.erase(it);
			continue;
		}
		else
		{
			it++;
		}
	}
}

//**********************************************************************************************************************
Entity* Game::GetEntity(uu::u32 id) const
{
	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		Entity& entity = *(*it);

		if (entity._bRemove == false && entity._id == id)
		{
			//Log(LogType::eTrace, LogModule::eGame, true, "[GetEntity] entity=%s\n", entity.ToString());
			return *it;
		}
	}

	Log(LogType::eError, LogModule::eGame, true, "[GetEntity] entity %lu not found\n", id);
	return nullptr;
}

//**********************************************************************************************************************
void  Game::GetEntitiesList(std::vector<Entity*>& list, uu::StringId const& type, uu::u32 exclude_id) const
{
	list.clear();

	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		Entity& entity = *(*it);

		if (entity._bRemove == false && entity._id != exclude_id && entity.IsA(type))
		{
			list.push_back(*it);
		}
	}
}

//**********************************************************************************************************************
void  Game::GetEntitiesListFiltered(std::vector<Entity*>& list, uu::StringId const& type, bool bLocal, uu::u32 exclude_id) const
{
	list.clear();

	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		Entity& entity = *(*it);

		if (entity._bRemove == false && entity._id != exclude_id && entity.IsMaster() == bLocal && entity.IsA(type))
		{
			list.push_back(*it);
		}
	}
}

//**********************************************************************************************************************
void Game::AddWidget(Widget& widget)
{
	auto it = std::find(_widgets.begin(), _widgets.end(), &widget);
	if (it == _widgets.end())
	{
		_widgets.push_back(&widget);
	}
}

//**********************************************************************************************************************
void Game::RemoveWidget(Widget& widget)
{
	auto it = std::find(_widgets.begin(), _widgets.end(), &widget);
	if (it != _widgets.end())
	{
		_widgets.erase(it);
	}
}

//**********************************************************************************************************************
Widget* Game::GetWidget(uu::u32 id) const
{
	for (auto it = _widgets.begin(); it != _widgets.end(); it++)
	{
		Widget& widget = *(*it);

		if (widget._id == id)
			return &widget;
	}

	return nullptr;
}

//**********************************************************************************************************************
Game::Timer&  Game::_AddTimer(uu::StringId const& name, time_t duration)
{
	Timer* timer = new Timer(name, duration);
	_timers.push_back(timer);

	return *timer;
}

//**********************************************************************************************************************
void  Game::_RemoveTimer(Game::Timer& timer)
{
}

//**********************************************************************************************************************
void Game::_CleanUI()
{
	RemoveWidget(_label_game_state);
	RemoveWidget(_label_network_state);
	RemoveWidget(_create_session_button);

	// delete dynamic widgets
	for (auto it = _widgets.begin(); it != _widgets.end(); it++)
	{
		delete(*it);
	}

	_widgets.clear();
}

//**********************************************************************************************************************
bool Game::Initialize()
{
	Game::_entity_id = (GetCurrentThreadId() << 16);

	_window = new sf::RenderWindow(sf::VideoMode(Configuration::GetInstance()._area_width, Configuration::GetInstance()._area_height), "Game", sf::Style::Titlebar | sf::Style::Close);
	if (_window == nullptr)
	{
		return false;
	}

	//_window->setVerticalSyncEnabled(true);
	//_window->setFramerateLimit(60);

	_world.Initialize();

	_session_service.Initialize();

	_session_service.AddSessionListener(this);
	_session_service.AddNetworkDataListener(this);

	return true;
}

//**********************************************************************************************************************
bool Game::FirstUpdate()
{
	// update session service
	_session_service.FirstUpdate();

	// create ui
	_font = _resourceManager.LoadFont("data/arial.ttf");

	if (_font != nullptr)
	{
		_label_game_state.SetStyle(_font, 14, sf::Text::Style::Bold, sf::Color::White);
		_label_game_state.SetPosition(0, 0);
		AddWidget(_label_game_state);
	
		_label_network_state.SetStyle(_font, 20, sf::Text::Style::Regular, sf::Color(128, 128, 128));
		_label_network_state.SetPosition(0, 20);
		AddWidget(_label_network_state);

		_create_session_button.SetText("Create Session");
		_create_session_button.SetPosition(20, 40);
		_create_session_button.SetStyle(_font, 20, sf::Text::Style::Regular, sf::Color::White, 1.f, sf::Color::White);
		//AddWidget(_create_session_button);
	}

	_ChangeState(session);

	return true;
}

//**********************************************************************************************************************
void Game::_ManageEntities()
{
	_bInLoop = true;

	// update entities
	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		Entity& entity = *(*it);
		entity.Update(_time_now);
	}

	_bInLoop = false;

	// remove entities to remove
	_RemoveEntities();

	//std::sort(_entities.begin(), _entities.end(), compare_sprite_y);
	_entities.sort(compare_sprite_y);
}

//**********************************************************************************************************************
void  Game::_CleanTimers()
{
	for (auto it = _timers.begin(); it != _timers.end(); it++)
	{
		delete(*it);
	}

	_timers.clear();
}

//**********************************************************************************************************************
void  Game::_ManageTimers()
{
	_bInLoop = true;

	auto it = _timers.begin();
	while (it != _timers.end())
	{
		Timer& timer = *(*it);

		if (timer.IsElapsed(_time_now))
		{
			_OnTimer(timer);

			if (timer.IsElapsed(_time_now))
			{
				it = _timers.erase(it);
				continue;
			}
		}

		it++;
	}

	_bInLoop = false;
}

//**********************************************************************************************************************
void Game::_OnTimer(Game::Timer& timer)
{
	if (_timer_spawn_enemy == timer._name)
	{
		Entity* entity = CreateLocalEnemy();
		if (entity != nullptr)
		{
			DispatchCreateEntityToSessionClients(*entity);
		}

		timer.ReArm(Configuration::GetInstance()._timer_spawn_enemy);
	}
	else if (_timer_spawn_coin == timer._name)
	{
		Entity* entity = CreateLocalEntity(Coin::type, "coin");
		if (entity != nullptr)
		{
			DispatchCreateEntityToSessionClients(*entity);
		}

		timer.ReArm(Configuration::GetInstance()._timer_spawn_coin);
	}
	else if (_timer_refresh_point == timer._name)
	{
		scoringManager->sendScore(uu::Time::GetSynchTime());
		timer.ReArm(Configuration::GetInstance()._timer_refresh_point);
	}
}

//**********************************************************************************************************************
void Game::_DeleteEntitiesFromClient(SessionClient const& client)
{
	auto it = _entities.begin();
	while (it != _entities.end())
	{
		Entity& entity = *(*it);

		if (client._addr == entity._peer)
		{
			if (_playerEntity == *it)
				_playerEntity = nullptr;

			delete(*it);
			it = _entities.erase(it);
			continue;
		}
		else
		{
			it++;
		}
	}
}

//**********************************************************************************************************************
void Game::_DeleteAllEntities()
{
	_playerEntity = nullptr;

	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		delete(*it);
	}

	_entities.clear();
}


//**********************************************************************************************************************
void Game::_ChangeState(State new_state)
{
	if (_state != new_state)
	{
		_next_state = new_state;
	}
}

//**********************************************************************************************************************
void Game::_OnEnterStateSession()
{
}

//**********************************************************************************************************************
void Game::_OnEnterStateLobby()
{
	// create local stuff
	// create local player

	_playerEntity = _CreateLocalPlayer();

	// dispatch local stuff to all session's client
	DispatchCreateEntityToSessionClients(*_playerEntity);

	_ChangeState(game);
}

//**********************************************************************************************************************
void Game::_OnEnterStateGame()
{
	_AddTimer(_timer_spawn_enemy, Configuration::GetInstance()._timer_spawn_enemy);
	_AddTimer(_timer_spawn_coin, Configuration::GetInstance()._timer_spawn_coin);

	if (isHost) {
		Log(LogType::eError, LogModule::eGame, true, "on passe par ici, et on repassera par là");
		scoringManager = new ScoringManager();
		_AddTimer(_timer_refresh_point, Configuration::GetInstance()._timer_refresh_point);
	}
		
}

//**********************************************************************************************************************
void Game::_OnEnterStateEnd()
{
	// delete all
	_DeleteAllEntities();
}

//**********************************************************************************************************************
bool Game::Update()
{
	_time_now = uu::Time::GetSynchTime();
	_frame++;

	// update session service
	if (_session_service.Update() == false)
	{
		// end of game, return false
		return false;
	}

	_label_game_state.SetText("Game state: %s  entities=%lu  time=%llu  frame=%lu", _static_game_state_strings[_state], _entities.size(), _time_now, _frame);

	_label_network_state.SetText("Network state: %s", _static_session_state_strings[_session_service.GetState()]);

	if (_session_service.GetState() > ClientService::eResolvingServerAddr && _session_service.GetState() < ClientService::eGetSessionInfo)
	{
		AddWidget(_create_session_button);
	}

	// change state machine if needs
	if (_next_state != _state)
	{
		Log(LogType::eTrace, LogModule::eGame, true, "changing state %s -> %s\n", _static_game_state_strings[_state], _static_game_state_strings[_next_state]);

		_state = _next_state;
		_next_state = _state;

		switch (_state)
		{
		case session:	_OnEnterStateSession();	break;
		case lobby:		_OnEnterStateLobby();	break;
		case game:		_OnEnterStateGame();	break;
		case end:		_OnEnterStateEnd();		break;
		}
	}

	// update entities
	_ManageTimers();
	_ManageEntities();

	// refresh diwplay window
	if (_window != nullptr)
	{
		if (_window->isOpen() == false)
		{
			// end of game, return false
			return false;
		}

		// manage window events
		sf::Event event;
		while (_window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				_window->close();
			}
			else if (event.type >= sf::Event::KeyPressed && event.type <= sf::Event::JoystickDisconnected)
			{
				_OnInputEvent(event, *_window);
			}
		}
	
		// draw
		_window->clear(sf::Color::Black);

		_OnDraw(*_window);

		// flush display
		_window->display();
	}

	return (_state != end);
}

//**********************************************************************************************************************
bool Game::Terminate()
{
	_world.Terminate();

	_session_service.RemoveSessionListener(this);
	_session_service.RemoveNetworkDataListener(this);

	// term entities
	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		Entity& entity = *(*it);
		entity.Terminate();
	}

	_session_service.Terminate();

	if (_window != nullptr)
	{
		delete _window;
		_window = nullptr;
	}

	_timers.clear();

	return true;
}

//**********************************************************************************************************************
void Game::_OnDraw(sf::RenderTarget& target)
{
	_bInLoop = true;

	if (_session_service.GetState() == ClientService::eInSession)
	{
		_world.Draw(target);

		float y = 40.f;
		// draw entities
		for (auto it = _entities.begin(); it != _entities.end(); it++)
		{
			Entity& entity = *(*it);

			if (entity.IsA(Character::type))
			{
				Character& ch = dynamic_cast<Character&>(entity);
				ch._label.SetPosition(0, y);
				y += 16.f;
			}

			entity.Draw(*_window, _bDebugDraw);
		}
	}

	// draw ui
	for (auto it = _widgets.begin(); it != _widgets.end(); it++)
	{
		Widget& widget = *(*it);
		widget.Draw(*_window);
	}

	_bInLoop = false;
}

//**********************************************************************************************************************
void Game::OnDiscoverSession(SessionDescriptor const& descriptor)
{
	Log(LogType::eTrace, LogModule::eGame, true, "OnDiscoverSession(session_id = %lu)\n", descriptor._session_id);

	_discovered_sessions.push_back(descriptor);

	Button* button = new Button;

	float y = 60;
	y += 30 * _discovered_sessions.size();
	SessionClient* host = descriptor.GetSessionHost();
	if (host != nullptr)
	{
		button->SetText("Join session %lu   host: \"%s\"  on %s", descriptor._session_id, host->_player_name.c_str(), host->_addr.ToString());
	}
	else
	{
		button->SetText("Join session %lu", descriptor._session_id);
	}

	button->SetId(descriptor._session_id);
	button->SetPosition(20, y);
	button->SetStyle(_font, 20, sf::Text::Style::Regular, sf::Color::White, 1.f, sf::Color::White);
	AddWidget(*button);
}

//**********************************************************************************************************************
void Game::OnEnterSession(SessionDescriptor const& descriptor)
{
	Log(LogType::eTrace, LogModule::eGame, true, "OnEnterSession(session_id = %lu)\n", descriptor._session_id);

	_ChangeState(lobby);
}

//**********************************************************************************************************************
void Game::OnLeaveSession(SessionDescriptor const& descriptor)
{
	Log(LogType::eTrace, LogModule::eGame, true, "OnLeaveSession(session_id = %lu)\n", descriptor._session_id);

	_ChangeState(end);
}

//**********************************************************************************************************************
void Game::OnCloseSession(SessionDescriptor const& descriptor)
{
	Log(LogType::eTrace, LogModule::eGame, true, "OnCloseSession(session_id = %lu)\n", descriptor._session_id);

	_ChangeState(end);
}

//**********************************************************************************************************************
void Game::OnClientEnterSession(SessionDescriptor const& descriptor, SessionClient const& client)
{
	Log(LogType::eTrace, LogModule::eGame, true, "OnClientEnterSession(session_id = %lu, client = %s)\n", descriptor._session_id, client._addr.ToString());

	// dispatch local stuff to entering client
	DispatchLocalEntitiesToClient(client);
}

//**********************************************************************************************************************
void Game::OnClientLeaveSession(SessionDescriptor const& descriptor, SessionClient const& client)
{
	Log(LogType::eTrace, LogModule::eGame, true, "OnClientLeaveSession(session_id = %lu, client = %s)\n", descriptor._session_id, client._addr.ToString());

	// delete network stuff from left client
	_DeleteEntitiesFromClient(client);
}

//**********************************************************************************************************************
// Ici on recoit chaque paquet
// filtrage par type
// appel de la méthode appropriée
bool Game::OnNetworkData(uu::u32 dataContainerId, void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	//Log(LogType::eError, LogModule::eGame, true, "BAAAAAAAAAAHHHHHHHH%lu" , dataContainerId);

	if (CreateEntityRequest::dataContainerId == dataContainerId)
	{
		_OnCreateEntityRequest(bytes, size, from_addr);
		return true;
	}

	if (CreatePlayerRequest::dataContainerId == dataContainerId)
	{
		_OnCreatePlayerRequest(bytes, size, from_addr);
		return true;
	}

	if (CreateEnemyRequest::dataContainerId == dataContainerId)
	{
		_OnCreateEnemyRequest(bytes, size, from_addr);
		return true;
	}

	if (CreateBombRequest::dataContainerId == dataContainerId)
	{
		_OnCreateBombRequest(bytes, size, from_addr);
		return true;
	}

	if (GotoObjectRequest::dataContainerId == dataContainerId)
	{
		_OnGotoObjectRequest(bytes, size, from_addr);
		return true;
	}

	if (FollowObjectRequest::dataContainerId == dataContainerId)
	{
		_OnFollowObjectRequest(bytes, size, from_addr);
		return true;
	}

	if (AttackObjectRequest::dataContainerId == dataContainerId)
	{
		_OnAttackObjectRequest(bytes, size, from_addr);
		return true;
	}

	if (HitObjectRequest::dataContainerId == dataContainerId)
	{
		_OnHitObjectRequest(bytes, size, from_addr);
		return true;
	}

	

	if (UpdateScoreContainer::dataContainerId == dataContainerId)
	{
		_OnUpdateScoreContainer(bytes, size, from_addr);
		return true;
	}

	if (KillEnemyContainer::dataContainerId == dataContainerId)
	{
		_OnKillEnemyContainer(bytes, size, from_addr);
		return true;
	}

	return false;
}

//**********************************************************************************************************************
void Game::_OnCreateEntityRequest(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eGame, true, "CreateEntityRequest received from %s\n", from_addr.ToString());

	uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);
	CreateEntityRequest request;

	if (request.ReadFromNetworkData(reader, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to read datacontainer CreateEntityRequest\n");
		return;
	}

	uu::StringId const& type = GetKnownType(request._type);

	Entity* entity = CreateNetworkEntity(type, request._name, request._id, request._owner);
	if (entity != nullptr)
	{
		entity->SetPosition(request._x, request._y);
	}
}

//**********************************************************************************************************************
void Game::_OnCreatePlayerRequest(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eGame, true, "CreatePlayerRequest received from %s\n", from_addr.ToString());

	uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);
	CreatePlayerRequest request;

	if (request.ReadFromNetworkData(reader, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to read datacontainer CreatePlayerRequest\n");
		return;
	}

	uu::StringId const& type = GetKnownType(request._type);

	Entity* entity = CreateNetworkEntity(type, request._name, request._id, request._owner);
	if (entity != nullptr)
	{
		entity->ReadFromContainer(request);
		entity->SetPosition(request._x, request._y);
	}
}

//**********************************************************************************************************************
void Game::_OnCreateEnemyRequest(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eGame, true, "CreateEnemyRequest received from %s\n", from_addr.ToString());

	uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);
	CreateEnemyRequest request;

	if (request.ReadFromNetworkData(reader, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to read datacontainer CreateEnemyRequest\n");
		return;
	}

	uu::StringId const& type = GetKnownType(request._type);

	Entity* entity = CreateNetworkEntity(type, request._name, request._id, request._owner);
	if (entity != nullptr)
	{
		entity->ReadFromContainer(request);
		entity->SetPosition(request._x, request._y);
	}
}

//**********************************************************************************************************************
void Game::_OnCreateBombRequest(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eGame, true, "CreateBombRequest received from %s\n", from_addr.ToString());

	uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);
	CreateBombRequest request;

	if (request.ReadFromNetworkData(reader, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to read datacontainer CreateBombRequest\n");
		return;
	}

	uu::StringId const& type = GetKnownType(request._type);

	Entity* entity = CreateNetworkEntity(type, request._name, request._id, request._owner);
	if (entity != nullptr)
	{
		entity->ReadFromContainer(request);
		entity->SetPosition(request._x, request._y);
		Entity* p = Game::GetInstance().GetEntity(request._idPlayer);
		Player* player = dynamic_cast<Player*>(p);
		player->setBombId(request._id);
	}
}

//**********************************************************************************************************************
void Game::_OnGotoObjectRequest(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eGame, true, "GotoObjectRequest received from %s\n", from_addr.ToString());

	uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);
	GotoObjectRequest request;

	if (request.ReadFromNetworkData(reader, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to read datacontainer GotoObjectRequest\n");
		return;
	}

	Entity* entity = GetEntity(request._id);
	if (entity != nullptr)
	{
		Character* ch = dynamic_cast<Character*>(entity);
		if (ch != nullptr)
		{
			ch->GoTo(request._x, request._y);
		}
	}
}

//**********************************************************************************************************************
void Game::_OnFollowObjectRequest(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eGame, true, "FollowObjectRequest received from %s\n", from_addr.ToString());

	uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);
	FollowObjectRequest request;

	if (request.ReadFromNetworkData(reader, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to read datacontainer FollowObjectRequest\n");
		return;
	}

	Entity* entity = GetEntity(request._id);
	if (entity != nullptr)
	{
		Character* ch = dynamic_cast<Character*>(entity);
		if (ch != nullptr)
		{
			ch->Follow(request._id_to_follow);
		}
	}
}

//**********************************************************************************************************************
void Game::_OnAttackObjectRequest(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eGame, true, "AttackObjectRequest received from %s\n", from_addr.ToString());

	uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);
	AttackObjectRequest request;

	if (request.ReadFromNetworkData(reader, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to read datacontainer AttackObjectRequest\n");
		return;
	}

	Character* entity = dynamic_cast<Character*>(GetEntity(request._id_attacker));
	if (entity != nullptr)
	{
		entity->Attack(request._id_to_attack);
	}
}

//**********************************************************************************************************************
void Game::_OnHitObjectRequest(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eTrace, LogModule::eGame, true, "HitObjectRequest received from %s\n", from_addr.ToString());

	uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);
	HitObjectRequest request;

	if (request.ReadFromNetworkData(reader, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to read datacontainer HitObjectRequest\n");
		return;
	}

	Character* entity = dynamic_cast<Character*>(GetEntity(request._id_to_hit));
	if (entity != nullptr)
	{
		entity->Hit(request._id_attacker, request._hit_value);
	}
}

void Game::_OnKillEnemyContainer(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	if (isHost) {
		Log(LogType::eError, LogModule::eGame, true, "OnKillEnemyContainer received from %s\n", from_addr.ToString());

		uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);
		KillEnemyContainer request;

		if (request.ReadFromNetworkData(reader, from_addr) == false)
		{
			return;
		}
		scoringManager->addHit(request._id_enemy, request._id_attacker, request._currenttime);
	}
}

void Game::_OnUpdateScoreContainer(void* bytes, int size, uu::network::IPEndPoint const& from_addr)
{
	Log(LogType::eError, LogModule::eGame, true, "OnUpdateScoreContainer received from %s\n", from_addr.ToString());

	uu::Reader reader(bytes, size, uu::Endianness::eNetworkEndian);
	UpdateScoreContainer request;

	if (request.ReadFromNetworkData(reader, from_addr) == false)
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to read datacontainer OnUpdateScoreContainer\n");
		return;
	}

	Player* entity = dynamic_cast<Player*>(GetEntity(request._id_player));
	if (entity != nullptr)
	{
		entity->_score = request._score;
	}
}

//**********************************************************************************************************************
void Game::_OnUI(Widget& widget, sf::Event::MouseButtonEvent const& event)
{
	bool clean = false;

	if (&widget == &_create_session_button)
	{
		clean = _session_service.CreateSession();
		isHost = true;
	}
	else
	{
		clean = _session_service.JoinSession(widget.GetId());
	}

	if (clean)	// clean UI
	{
		_CleanUI();
		_discovered_sessions.clear();
		AddWidget(_label_game_state);
	}
}

//**********************************************************************************************************************
void Game::_OnInputEvent(sf::Event& event, sf::RenderWindow& window)
{
	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Key::D)
		{
			_bDebugDraw ^= 1;
		}
		else if (event.key.code == sf::Keyboard::Key::Space)
		{
			if (_playerEntity != nullptr)
			{
				/*Bomb* bomb =*/ _playerEntity->SpawnLocalBomb();
			}
		}
		else if (event.key.code == sf::Keyboard::Key::I)
		{
			if (_playerEntity != nullptr)
			{
				_playerEntity->InvokeLocalCreature();
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2f point = _window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

		Widget* widget = _TryPickWidget(point);
		if (widget != nullptr)
		{
			_OnUI(*widget, event.mouseButton);
		}
		else
		{
			Entity* selectedEntity = TryPickEntity(point);

			if (event.mouseButton.button == sf::Mouse::Button::Left || event.mouseButton.button == sf::Mouse::Button::Right)
			{
				if (selectedEntity == nullptr)
				{
					if (_playerEntity != nullptr)
					{
						_playerEntity->GoTo(point);
					}
				}
				else
				{
					if (_playerEntity != nullptr)
					{
						_playerEntity->Attack(selectedEntity->GetId());
					}
				}
			}
		}
	}
}

//**********************************************************************************************************************
Entity* Game::TryPickEntity(sf::Vector2f const& point)
{
	sf::FloatRect rect ;

	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		Entity& entity = *(*it);

		if (entity.PickTest(point.x, point.y))
			return &entity;
	}

	return nullptr;
}

//**********************************************************************************************************************
Widget* Game::_TryPickWidget(sf::Vector2f const& point)
{
	sf::FloatRect rect ;

	for (auto it = _widgets.begin(); it != _widgets.end(); it++)
	{
		Widget& widget = *(*it);

		if (widget.PickTest(point.x, point.y))
			return &widget;
	}

	return nullptr;
}

//**********************************************************************************************************************
Entity* Game::_CreateEntityByType(uu::StringId const& type, std::string const& name, uu::u32 id, bool bLocal, uu::network::IPEndPoint const& owner)
{
	Entity* entity = GetEntity(id);
	if (entity != nullptr)
	{
		Log(LogType::eError, LogModule::eGame, true, "Entity allready exists: entity=%s\n", entity->ToString());
		return entity;
	}

	if (DarkWarrior::type == type) entity = new DarkWarrior();
	else if (MageBlue::type == type) entity = new MageBlue();
    else if (Paladin::type == type) entity = new Paladin();
    else if (Bomb::type == type) entity = new Bomb();
	else if (Box1::type == type) entity = new Box1();
	else if (Box2::type == type) entity = new Box2();
	else if (Box3::type == type) entity = new Box3();
	else if (Coin::type == type) entity = new Coin();
	else if (Flower::type == type) entity = new Flower();
	else if (Goomba::type == type) entity = new Goomba();
	else if (Live::type == type) entity = new Live();
	else if (Star::type == type) entity = new Star();
	else if (Up::type == type) entity = new Up();
	else
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to create Entity because unkwown type=0x%08x\n", type);
		return nullptr;
	}

	if (entity == nullptr)
		return nullptr;

	entity->_id = id;
	entity->_name = name;
	entity->_bMaster = bLocal;
	entity->_peer = owner;

	Log(LogType::eTrace, LogModule::eGame, true, "[_CreateEntityByType] entity  type=0x%08x  ptr=%p\n", type, entity);

	_AddEntity(entity);

	return entity;
}

//**********************************************************************************************************************
Entity* Game::CreateNetworkEntity(uu::StringId const& type, std::string const& name, uu::u32 id, uu::network::IPEndPoint const& owner)
{
	/*SessionClient *client = GetSessionClient(owner);
	if (client == nullptr)
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to create Entity type=0x%08x because unkwown owner %s\n", type, owner.ToString());
		return nullptr;
	}*/
	
	if (owner == GetLocalSessionClient()._addr)
	{
		Log(LogType::eError, LogModule::eGame, true, "unable to create Entity type=0x%08x because owner is me\n", type);
		return nullptr;
	}

	Entity* entity = _CreateEntityByType(type, name, id, false, owner);
	if (entity == nullptr)
		return nullptr;

	entity->Initialize();

	return entity;
}

//**********************************************************************************************************************
Entity* Game::CreateLocalEntity(uu::StringId const& type, std::string const& name)
{
	Entity* entity = _CreateEntityByType(type, name, ++Game::_entity_id, true, GetLocalSessionClient()._addr);
	if (entity == nullptr)
		return nullptr;

	entity->Initialize();

	return entity;
}

//**********************************************************************************************************************
void Game::DispatchLocalEntityGoTo(Character const& character, sf::Vector2f const& point)
{
	Log(LogType::eTrace, LogModule::eGame, true, "Game::DispatchLocalEntityGoTo(%f,%f): character=%s\n", point.x, point.y, character.ToString());

	if (character.IsMaster() == false)
	return;

	GotoObjectRequest request;
	request._id = character._id;
	request._x = point.x;
	request._y = point.y;

	SendDataContainerToSessionClients(request);
}

//**********************************************************************************************************************
void Game::DispatchLocalEntityFollow(Character const& character, uu::u32 id_to_follow)
{
	Log(LogType::eTrace, LogModule::eGame, true, "Game::DispatchLocalEntityFollow(%lu): character=%s\n", id_to_follow, character.ToString());

	if (character.IsMaster() == false)
		return;

	FollowObjectRequest request;
	request._id = character._id;
	request._id_to_follow = id_to_follow;

	SendDataContainerToSessionClients(request);
}

//**********************************************************************************************************************
void Game::DispatchLocalEntityAttack(Character const& character, uu::u32 id_to_attack)
{
	Log(LogType::eTrace, LogModule::eGame, true, "Game::DispatchLocalEntityAttack(%lu): character=%s\n", id_to_attack, character.ToString());

	if (character.IsMaster() == false)
		return;

	AttackObjectRequest request;
	request._id_attacker = character._id;
	request._id_to_attack = id_to_attack;

	SendDataContainerToSessionClients(request);
}

//**********************************************************************************************************************
void Game::DispatchLocalEntityHit(Character const& character, uu::u32 attacker, float hit_value)
{
	Log(LogType::eTrace, LogModule::eGame, true, "Game::DispatchLocalEntityAttack(%lu): character=%s\n", attacker, character.ToString());

	if (character.IsMaster() == false)
		return;

	HitObjectRequest request;
	request._id_attacker = attacker;
	request._id_to_hit = character._id;
	request._hit_value = hit_value;

	SendDataContainerToSessionClients(request);
}

//**********************************************************************************************************************
void Game::DispatchLocalEntitiesToClient(SessionClient const& client)
{
	for (auto it = _entities.begin(); it != _entities.end(); it++)
	{
		Entity& entity = *(*it);

		if (entity.IsMaster())
		{
			DispatchCreateEntityToClient(entity, client);
		}
	}
}

//**********************************************************************************************************************
void Game::DispatchCreateEntityToClient(Entity const& entity, SessionClient const& client)
{
	uu::network::DataContainer* request = entity.CreateContainer();
	if (request != nullptr)
	{
		Log(LogType::eTrace, LogModule::eGame, true, "DispatchCreateEntityToClient: entity=%s\n", entity.ToString());

		entity.WriteToContainer(*request);

		SendDataContainer(*request, client._addr);

		delete(request);
	}
}

//**********************************************************************************************************************
void Game::DispatchCreateEntityToSessionClients(Entity const& entity)
{
	uu::network::DataContainer* request = entity.CreateContainer();
	if (request != nullptr)
	{
		Log(LogType::eTrace, LogModule::eGame, true, "DispatchCreateEntityToSessionClients: entity=%s\n", entity.ToString());

		entity.WriteToContainer(*request);

		SendDataContainerToSessionClients(*request);

		delete(request);
	}
}

//**********************************************************************************************************************
bool Game::SendDataContainerToSessionClients(uu::network::DataContainer& datacontainer)
{
	return _session_service.SendDataContainerToSessionExclude(datacontainer, _session_service.GetCurrentSession(), _session_service.GetLocalSessionClient()._addr);
}

bool Game::BroadcastContainer(uu::network::DataContainer& datacontainer)
{
	return _session_service.SendDataContainerToSession(datacontainer, _session_service.GetCurrentSession(),true);
}


//**********************************************************************************************************************
bool Game::SendDataContainer(uu::network::DataContainer& datacontainer, uu::network::IPEndPoint const& to_addr)
{
	Log(LogType::eTrace, LogModule::eGame, true, "SendDataContainer to %s\n", to_addr.ToString());

	return _session_service.SendDataContainer(datacontainer, to_addr);
}
