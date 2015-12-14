//**********************************************************************************************************************
#pragma once

#include "Item.h"
#include "draw_ui.h"

//**********************************************************************************************************************
class Bomb: public Item
{
public:
	static uu::StringId type;
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Item::IsA(type); }
	virtual uu::network::DataContainer* CreateContainer() const;

public:
	enum State
	{
		none,
		ticks,
		explode,
		dead,
	};

public:
	Bomb();
	//Entity overrides
	virtual bool Update(time_t time_now);
	virtual void Draw(sf::RenderTarget& target, bool bDebugDraw = false);

	State GetState() const { return _state; }
	bool IsDead() const { return (_state == none || _state == dead); } 

	bool IsInExplosionRange(sf::Vector2f const& point) const;

public:
	Label _label;
	State _state;
	time_t _explosion_time;
	float _explosion_radius;
	float _current_radius;
	float _power;

protected:
	bool _SetState(State new_state);

	void _RefreshTicks(time_t time_now);
	void _RefreshExplode(time_t time_now);
	void _RefreshDead(time_t time_now);

	//Entity overrides
	virtual void ReadFromContainer(uu::network::DataContainer const& container);
	virtual void WriteToContainer(uu::network::DataContainer& container) const;
};
