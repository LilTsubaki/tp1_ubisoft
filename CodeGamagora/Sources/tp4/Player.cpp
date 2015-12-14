#include "gameobjects.h"
#include "player.h"
#include "Bomb.h"
#include "Game.h"

//**********************************************************************************************************************
Player::Player(const char* name) : Character(name),
	_coins(0)
{
}

uu::network::DataContainer* Player::CreateContainer() const
{
	return new CreatePlayerRequest();
}

void Player::ReadFromContainer(uu::network::DataContainer const& container)
{
	Character::ReadFromContainer(container);

	CreatePlayerRequest const& data = dynamic_cast<CreatePlayerRequest const&>(container);

	_coins = data._coins;
}

void Player::WriteToContainer(uu::network::DataContainer& container) const
{
	Character::WriteToContainer(container);

	CreatePlayerRequest& data = dynamic_cast<CreatePlayerRequest&>(container);

	data._coins = _coins;
}

bool Player::Update(time_t time_now)
{
	if (IsDead())
	{
		if (GetState() == none)
		{
			// respawn player
			_current_values = _init_values;
			_current_anim_name = "idle";
			_current_anim_loop = true;
			_SetState(idle);
		}
		return Character::Update(time_now);
	}

	std::vector<Entity*> list;
	Game::GetInstance().GetEntitiesList(list, Coin::type, 0);
	if (list.empty() == false)
	{
		for (auto it = list.begin(); it != list.end(); it++)
		{
			Coin& coin = dynamic_cast<Coin&>(*(*it));

			if (coin.IsNear(_position, 40.f) == true)
			{
				coin.Collect();
				_coins++;
			}
		}

		list.clear();
	}

	_label.SetText("%s:  live=%lu  coins=%lu", _name.c_str(), (uu::u32)_current_values._live, _coins);

	return Character::Update(time_now);
}

void Player::InvokeLocalCreature()
{
	Enemy* enemy = nullptr;

	if (_enemy_id != 0)
	{
		enemy = dynamic_cast<Enemy*>(Game::GetInstance().GetEntity(_enemy_id));
	}

	if (enemy == nullptr)
	{
		_enemy_id = 0;

		enemy = dynamic_cast<Enemy*>(Game::GetInstance().CreateLocalEnemy());

		if (enemy != nullptr)
		{
			_enemy_id = enemy->GetId();

			Game::GetInstance().DispatchCreateEntityToSessionClients(*enemy);
		}
	}

//	return enemy;
}

Bomb* Player::SpawnLocalBomb()
{
	Bomb* bomb = nullptr;

	if (_bomb_id != 0)
	{
		bomb = dynamic_cast<Bomb*>(Game::GetInstance().GetEntity(_bomb_id));
	}

	if (bomb == nullptr)
	{
		_bomb_id = 0;

		bomb = dynamic_cast<Bomb*>(Game::GetInstance().CreateLocalEntity(Bomb::type, "bomb"));

		if (bomb != nullptr)
		{
			_bomb_id = bomb->GetId();
			bomb->SetPosition(_position);
		}
	}

	return bomb;
}

//**********************************************************************************************************************
MageBlue::MageBlue() : Player("MageBlue")
{
}

//**********************************************************************************************************************
Paladin::Paladin() : Player("Paladin")
{
	_init_values._power = 25.f;
}