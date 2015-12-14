//**********************************************************************************************************************
#include "Game.h"
#include "Character.h"
#include "Maths.h"
#include "Vector2.h"
#include "Ellipse2.h"

#include "SFML/Graphics.hpp"
#include "draw_utils.h"
#include "Log.h"
#include "game_datacontainer.h"

//**********************************************************************************************************************
uu::StringId Character::type = uu::StringId("Character");

//**********************************************************************************************************************
//**********************************************************************************************************************
Character::Character(const char* name):
	Animable(name),
	_state(none)
{
	_init_values._speed = 1.0f;
	_init_values._power = 10.f;
	_init_values._live = 100.f;

	_current_values = _init_values;

	_current_anim_name = "idle";
	_current_anim_loop = true;

	_label.SetFontSize(14);
	_label.SetStyle(sf::Text::Regular);
}

//**********************************************************************************************************************
Character::~Character()
{
}

//**********************************************************************************************************************
uu::network::DataContainer* Character::CreateContainer() const
{
	return new CreateCharacterRequest();
}

//**********************************************************************************************************************
void Character::ReadFromContainer(uu::network::DataContainer const& container)
{
	Entity::ReadFromContainer(container);

	CreateCharacterRequest const& data = dynamic_cast<CreateCharacterRequest const&>(container);

	_current_values._speed = data._speed;
	_current_values._power = data._power;
	_current_values._live = data._live;
}

//**********************************************************************************************************************
void Character::WriteToContainer(uu::network::DataContainer& container) const
{
	Entity::WriteToContainer(container);

	CreateCharacterRequest& data = dynamic_cast<CreateCharacterRequest&>(container);

	data._speed = _current_values._speed;
	data._power = _current_values._power;
	data._live = _current_values._live;

}

//**********************************************************************************************************************
void Character::SetValues(float speed, float power, float live, float view_range, float attack_range, float detect_range)
{
	_current_values._speed = speed;
	_current_values._power = power;
	_current_values._live = live;

}

//**********************************************************************************************************************
bool Character::Initialize()
{
	_current_values = _init_values;

	Animable::Initialize();

	_SetState(idle);

	return true;
}

//**********************************************************************************************************************
bool Character::Update(time_t time_now)
{
	switch (_state)
	{
	case idle:									break;
	case moveto:	_RefreshMoveTo(time_now);	break;
	case dead:		_RefreshDead(time_now);		break;
	default:									break;
	}

	if (_target != _position)
	{
		_orientation = _target - _position;
		uu::maths::Vector2Normalize(_orientation);
	}

	_SetAnimation(_current_anim_name.c_str(), _current_anim_loop);

	Animable::Update(time_now);

	_label.SetText("%s:  live=%lu", _name.c_str(), (uu::u32)_current_values._live);

	return true;
}

//**********************************************************************************************************************
bool Character::_SetState(State new_state)
{
	//if (_state != new_state)
	{
		if (_state != dead)
		{
			switch (new_state)
			{
			case none:
			break;

			case idle:
				_target = _position;
				_current_anim_name = "idle";	_current_anim_loop = true;
			break;

			case moveto:
				_current_anim_name = "go";		_current_anim_loop = true;
			break;

			case dead:
				_target = _position;
				_current_anim_name = "death";	_current_anim_loop = false;
			break;

			default:
			break;
			}

			_state = new_state;
		}
		else
		{
			if (new_state == none)
			{
				_sprite.SetVisible(false);
				_state = new_state;
			}
		}
	}

	return (_state == new_state);
}

//**********************************************************************************************************************
void Character::_RefreshMoveTo(time_t time_now)
{
	if (uu::maths::Vector2Distance(_position, _target) <= 1.5f)
	{
		_SetState(idle);
		return;
	}

	_position += ((_orientation) * _current_values._speed);

	_current_anim_name = "go";	_current_anim_loop = true;
}

//**********************************************************************************************************************
void Character::_RefreshDead(time_t time_now)
{
	if (_sprite.IsAnimationEnded())
	{
		_SetState(none);
		_sprite.SetVisible(false);
		return;
	}

	_current_anim_name = "death";	_current_anim_loop = false;
}

//**********************************************************************************************************************
void Character::Draw(sf::RenderTarget& target, bool bDebugDraw)
{
	if (_sprite.IsVisible() == false)
		return;

	if (IsMaster())
	{
		// draw the ellipse for local entity in blue
		uu::Ellipse2f ellipse;
		GetPositionEllipse(ellipse);

		ellipse._radius *= 20.f;

		DrawEllipse(target, ellipse._center, ellipse._radius, sf::Color::Blue, 3);
	}

	sf::FloatRect rect = _sprite.GetGlobalBounds();

	if (bDebugDraw)
	{
		// if debug, draw the sprite bounding box
		//DrawRect(target, rect, sf::Color::White);

		if (_state != idle)
		{
			// if debug, draw a line from _position to _target
			DrawLine(target, _position, _target, sf::Color::Yellow);
		}
	}

	// draw the sprite
	Animable::Draw(target, bDebugDraw);

	// draw the live gauge
	rect.top -= 4;
	rect.height = 5;
	rect.left = rect.left + (rect.width *.5f) - 20.f;
	rect.width = 40.f;

	float percent = _current_values._live * 100.f;
	if (_init_values._live != 0)
		percent /= _init_values._live;

	DrawGauge(target, percent, rect, sf::Color::White, sf::Color::Red);

	// draw the info label
	_label.SetFontColor(IsMaster() ? sf::Color::Blue : sf::Color::White);
	_label.Draw(target);
}

//**********************************************************************************************************************
bool Character::PickTest(float x, float y) const
{
	if (IsDead() == true)
		return false;

	return Animable::PickTest(x, y);
}

//**********************************************************************************************************************
void Character::GoTo(float x, float y)
{
	if (IsDead() == true)
		return;

	Log(LogType::eTrace, LogModule::eGame, true, "Character::GoTo(%f,%f): entity=%s\n", x, y, ToString());

	_target.x = x;	_target.y = y;

	_SetState(moveto);

	if (IsMaster())
	{
		MoveCharacterRequest request;
		request._id = _id;
		request._x = x;
		request._y = y;

		BroadcastDataContainerToReplicas(request);
	}

}

//**********************************************************************************************************************
void Character::Hit(uu::u32 id_attacker, float hit_value)
{
	if (IsDead() == true)
		return;

	if (id_attacker == _id)
		return;

	Entity* attacker = dynamic_cast<Entity*>(Game::GetInstance().GetEntity(id_attacker));
	if (attacker == nullptr)
		return;

	Log(LogType::eTrace, LogModule::eGame, true, "Character::Hit(%lu,%f): entity=%s\n", id_attacker, hit_value, ToString());

	_current_values._live -= hit_value;
	if (_current_values._live <= 0)
	{
		_current_values._live = 0;
		_SetState(dead);
	}
}

//**********************************************************************************************************************
bool Character::IsNear(sf::Vector2f const& point, float distance) const
{
	if (IsDead() == true)
		return false;

	return Animable::IsNear(point, distance);
}

//**********************************************************************************************************************
Enemy::Enemy(const char* name) : Character(name)
{
	_init_values._speed = 0.8f;
}

uu::network::DataContainer* Enemy::CreateContainer() const
{
	return NULL;
}

//**********************************************************************************************************************
DarkWarrior::DarkWarrior() : Enemy("DarkWarrior")
{
	_init_values._power = 25.f;
}

//**********************************************************************************************************************
Goomba::Goomba() : Character("Goomba")
{
}