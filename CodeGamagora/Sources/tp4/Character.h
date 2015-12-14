//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "Entity.h"
#include "AnimatedSprite.h"
#include "draw_ui.h"
#include "Animable.h"

//**********************************************************************************************************************
class Character: public Animable 
{
	friend class Game;
public:
	static uu::StringId type;

public:
	enum State
	{
		none,
		idle,
		moveto,
		dead,
	};

	struct values_t
	{
		float _speed;
		float _power;
		float _live;
	};

public:
	Character(const char* name);
	virtual ~Character();

	//Entity overrides
	virtual bool Initialize();
	virtual bool Update(time_t time_now);
	virtual void Draw(sf::RenderTarget& target, bool bDebugDraw = false);
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Animable::IsA(type); }
	virtual bool PickTest(float x, float y) const;
	virtual uu::network::DataContainer* CreateContainer() const;

	State GetState() const { return _state; }
	bool IsDead() const { return (_state == none || _state == dead); } 

	virtual void SetValues(float speed, float power, float live, float view_range, float attack_range, float detect_range);

	virtual void SetSpeed(float speed) { _current_values._speed = speed; }
	virtual float GetSpeed() const { return _current_values._speed; }
	virtual void SetPower(float power) { _current_values._power = power; }
	virtual float GetPower() const { return _current_values._power; }
	virtual void SetLive(float live) { _current_values._live = live; }
	virtual float GetLive() const { return _current_values._live; }

	virtual bool IsNear(sf::Vector2f const& point, float distance) const;

	void GoTo(sf::Vector2f const& point) { GoTo(point.x, point.y); }
	void GoTo(float x, float y);
	void Hit(uu::u32 id_attacker, float hit_value);

protected:
	State _state;
	Label _label;

	values_t _init_values;
	values_t _current_values;

	std::string _current_anim_name;
	bool _current_anim_loop;

protected:
	bool _SetState(State new_state);

	void _RefreshMoveTo(time_t time_now);
	void _RefreshDead(time_t time_now);

	//Entity overrides
	virtual void ReadFromContainer(uu::network::DataContainer const& container);
	virtual void WriteToContainer(uu::network::DataContainer& container) const;

};

//**********************************************************************************************************************
class Goomba: public Character
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Character::IsA(type); }

public:
	Goomba();
};

//**********************************************************************************************************************
class Enemy: public Character
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Character::IsA(type); }
	virtual uu::network::DataContainer* CreateContainer() const;

public:
	Enemy(const char* name);
};

//**********************************************************************************************************************
class DarkWarrior: public Enemy
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Enemy::IsA(type); }

public:
	DarkWarrior();
};
