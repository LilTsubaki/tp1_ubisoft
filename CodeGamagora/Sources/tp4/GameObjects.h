//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "StringId.h"
#include "Character.h"
#include "player.h"

//**********************************************************************************************************************
/*

GameObjects hierachy:

Entity
\____Animable
	\____Character
	|	\____Player
	|	|	\____Gnome
	|	|	\____MageBlue
	|	|	\____MageRed
	|	|	\____Paladin
	|	|	\____Ranger
	|	|	\____Warrior
	|	\____Enemy
	|	|	\____DarkWarrior
	|	|	\____Goblin
	|	|	\____GoblinShaman
	|	|	\____GoblinWarrior
	|	|	\____Harpy
	|	|	\____MageBlack
	|	|	\____Medusa
	|	|	\____Rat
	|	|	\____Skeleton
	|	|	\____Spider
	|	|	\____Vampire
	|	|	\____Zombie
	|	|	\____Goomba
	|	\____GoblinArcher
	|	\____Elf
	\____Item
		\____Bomb
		\____Box1
		\____Box2
		\____Box3
		\____Coin
		\____Flower
		\____Live
		\____Star
		\____Up
*/

//**********************************************************************************************************************
extern uu::StringId const& GetRandomPlayerType();
extern uu::StringId const& GetRandomEnemyType();
extern uu::StringId const& GetRandomItemType();
extern uu::StringId const& GetKnownType(uu::u32 id);

//**********************************************************************************************************************
class Box1: public Item
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Item::IsA(type); }

public:
	Box1();
};

//**********************************************************************************************************************
class Box2: public Item
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Item::IsA(type); }

public:
	Box2();
};

//**********************************************************************************************************************
class Box3: public Item
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Item::IsA(type); }

public:
	Box3();
};

//**********************************************************************************************************************
class Coin: public Item
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Item::IsA(type); }
	virtual bool PickTest(float x, float y) const { return false; }

	void Collect();

public:
	Coin();
};

//**********************************************************************************************************************
class Flower: public Item
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Item::IsA(type); }

public:
	Flower();
};

//**********************************************************************************************************************
class Live: public Item
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Item::IsA(type); }

public:
	Live();
};

//**********************************************************************************************************************
class Star: public Item
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Item::IsA(type); }

public:
	Star();
};

//**********************************************************************************************************************
class Up: public Item
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Item::IsA(type); }

public:
	Up();
};

