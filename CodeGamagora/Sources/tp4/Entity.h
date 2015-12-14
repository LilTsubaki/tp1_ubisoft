//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "DataContainer.h"
#include "game_datacontainer.h"


//**********************************************************************************************************************

//**********************************************************************************************************************
class Entity
{
	friend class Game;

public:

	Entity(): _id(0), _bMaster(false), _bRemove(false)
	{
	}

	virtual ~Entity()
	{
	}

	virtual bool Initialize() = 0;
	virtual bool Update(time_t _time_now) = 0;
	virtual bool Terminate() = 0;

	void SetOwner(uu::network::IPEndPoint const& owner) { _peer = owner; }
	uu::network::IPEndPoint const& GetOwner() const { return _peer; }

	bool IsMaster() const { return _bMaster; }
	bool IsReplica() const { return (_bMaster == false); }

	uu::u32 GetId() const { return _id; }

	virtual void SetPosition(float x, float y) = 0;
	virtual void SetPosition(sf::Vector2f const& position) = 0;
	virtual void GetPosition(sf::Vector2f& position) const = 0;

	virtual uu::StringId const& GetType() const = 0;
	virtual bool IsA(uu::StringId const& type) const = 0;

	virtual void Draw(sf::RenderTarget& target, bool bDebugDraw = false) = 0;
	virtual bool PickTest(float x, float y) const = 0;

	virtual uu::network::DataContainer* CreateContainer() const = 0;

	const char* ToString() const;

	void MarkForDelete() { _bRemove = true; };

	bool SendDataContainerToMaster(uu::network::DataContainer& datacontainer);
	bool BroadcastDataContainerToReplicas(uu::network::DataContainer& datacontainer);
    bool SendDataContainerToReplica(uu::network::DataContainer& datacontainer, uu::network::IPEndPoint const& peertarget);

protected:
	std::string _name;
	uu::u32 _id;
	uu::network::IPEndPoint _peer;

	bool _bMaster;
	bool _bRemove;

	char _str_tmp[2048];

	virtual void ReadFromContainer(uu::network::DataContainer const& container);
	virtual void WriteToContainer(uu::network::DataContainer& container) const;
};
