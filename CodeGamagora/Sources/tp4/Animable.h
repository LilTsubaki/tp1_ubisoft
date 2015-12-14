//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "Entity.h"
#include "AnimatedSprite.h"


//**********************************************************************************************************************
/*
_SetAnimation()
	common: "attack" "death" "go" "idle"
	darkwarrior: "super_attack"
	elf: "muz"
	gnome: "repairs"
	medusa: no "attack"

	coin, bomb, box1, box2, box3, flower, live, star, up: only "idle"
	goomba: only "go" "idle"
*/

//**********************************************************************************************************************
class Animable: public Entity
{
public:
	static uu::StringId type;
	virtual bool IsA(uu::StringId const& type) const { return (this->type == type); }
	virtual bool PickTest(float x, float y) const;

public:
	Animable(const char* name);
	virtual ~Animable();

	//Entity overrides
	virtual bool Initialize();
	virtual bool Update(time_t _time_now);
	virtual bool Terminate();
	virtual void Draw(sf::RenderTarget& target, bool bDebugDraw = false);
	virtual uu::StringId const& GetType() const { return type; }

	virtual void SetPosition(float x, float y) { _position.x = x; _position.y = y; }
	virtual void SetPosition(sf::Vector2f const& position) { _position = position; }
	virtual void GetPosition(sf::Vector2f& position) const { position = _position; }

	virtual bool IsNear(sf::Vector2f const& point, float distance) const;
	void GetPositionEllipse(sf::Vector2f& center, sf::Vector2f& radius) const;
	void GetPositionEllipse(uu::Ellipse2f& ellipse) const;

//public :

//    // To broadcast from Entity MASTER a DataContainer to all replicas of the entity
//     virtual void BroadcastFromMasterToReplicas(uu::network::DataContainer& datacontainer)
//     {
//          assert( IsMaster() );
//          Game::GetInstance().SendDataContainer( datacontainer, _owner );
//     }
// 
//     // To Send a DataContainer to the Entity Master
//     virtual void SendFromReplicaToMaster(uu::network::DataContainer& datacontainer)
//     {
//          assert( !IsMaster() );
//          Game::GetInstance().SendDataContainerToSessionClients( datacontainer );
//     }

protected:
	AnimatedSprite _sprite;
	sf::Vector2f _position;
	sf::Vector2f _orientation;
	sf::Vector2f _target;

protected:

	bool _SetAnimation(const char* name, bool loop);

};
