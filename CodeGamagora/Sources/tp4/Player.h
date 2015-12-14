//**********************************************************************************************************************
#pragma once

#include "GameObjects.h"
#include "types.h"
#include "StringId.h"
#include "Character.h"
#include "Bomb.h"





class Player: public Character
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Character::IsA(type); }
	virtual bool Update(time_t time_now);
	virtual uu::network::DataContainer* CreateContainer() const;

public:
	Player(const char* name);

	void    InvokeLocalCreature();
	Bomb*   SpawnLocalBomb();

protected:
	uu::u32 _bomb_id;
	uu::u32 _enemy_id;
	uu::u32 _coins;

	//Entity overrides
	virtual void ReadFromContainer(uu::network::DataContainer const& container);
	virtual void WriteToContainer(uu::network::DataContainer& container) const;
};

//**********************************************************************************************************************
class MageBlue: public Player
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Player::IsA(type); }

public:
	MageBlue();
};

//**********************************************************************************************************************
class Paladin: public Player
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Player::IsA(type); }

public:
	Paladin();
};


