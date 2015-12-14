#include "types.h"
#include "Time.h"
#include "GameObjects.h"
#include "draw_utils.h"
#include "Game.h"
#include "game_datacontainer.h"

//**********************************************************************************************************************
uu::StringId Player::type = uu::StringId("Player");
uu::StringId Enemy::type = uu::StringId("Enemy");
uu::StringId Item::type = uu::StringId("Item");

uu::StringId DarkWarrior::type = uu::StringId("DarkWarrior");
uu::StringId MageBlue::type = uu::StringId("MageBlue");
uu::StringId Paladin::type = uu::StringId("Paladin");

uu::StringId Bomb::type = uu::StringId("Bomb");
uu::StringId Box1::type = uu::StringId("Box1");
uu::StringId Box2::type = uu::StringId("Box2");
uu::StringId Box3::type = uu::StringId("Box3");
uu::StringId Coin::type = uu::StringId("Coin");
uu::StringId Flower::type = uu::StringId("Flower");
uu::StringId Goomba::type = uu::StringId("Goomba");
uu::StringId Live::type = uu::StringId("Live");
uu::StringId Star::type = uu::StringId("Star");
uu::StringId Up::type = uu::StringId("Up");

static const uu::StringId Player_types[2] = { MageBlue::type, Paladin::type };
static const uu::StringId Enemy_types[1] = { DarkWarrior::type };
static const uu::StringId Item_types[9] = { Bomb::type, Box1::type, Box2::type, Box3::type, Coin::type, Flower::type, Live::type, Star::type, Up::type };

static const uu::StringId All_Known_types[2+1+9] = {	MageBlue::type, Paladin::type,
														DarkWarrior::type,
														Bomb::type, Box1::type, Box2::type, Box3::type, Coin::type, Flower::type, Live::type, Star::type, Up::type };

uu::StringId unknown_type = uu::StringId("unknown_type");


//**********************************************************************************************************************
uu::StringId const& GetKnownType(uu::u32 id)
{
	auto max = sizeof(All_Known_types) / sizeof(uu::StringId);

	for (auto i=0; i<max; i++)
	{
		if (All_Known_types[i] == id)
			return All_Known_types[i];
	}

	return unknown_type;
}

//**********************************************************************************************************************
uu::StringId const& GetRandomPlayerType()
{
	auto max = sizeof(Player_types) / sizeof(uu::StringId);
	auto index = rand() % max;

	return Player_types[index];
}

//**********************************************************************************************************************
uu::StringId const& GetRandomEnemyType()
{
	auto max = sizeof(Enemy_types) / sizeof(uu::StringId);
	auto index = rand() % max;

	return Enemy_types[index];
}

//**********************************************************************************************************************
uu::StringId const& GetRandomItemType()
{
	auto max = sizeof(Item_types) / sizeof(uu::StringId);
	auto index = rand() % max;

	return Item_types[index];
}


//**********************************************************************************************************************
Box1::Box1() : Item("Box1")
{
}

//**********************************************************************************************************************
Box2::Box2() : Item("Box2")
{
}

//**********************************************************************************************************************
Box3::Box3() : Item("Box3")
{
}

//**********************************************************************************************************************
Coin::Coin() : Item("Coin")
{
}

void Coin::Collect()
{
    MarkForDelete();
    _sprite.SetVisible(false);
}

//**********************************************************************************************************************
Flower::Flower() : Item("Flower")
{
}

//**********************************************************************************************************************
Live::Live() : Item("Live")
{
}

//**********************************************************************************************************************
Star::Star() : Item("Star")
{
}

//**********************************************************************************************************************
Up::Up() : Item("Up")
{
}

