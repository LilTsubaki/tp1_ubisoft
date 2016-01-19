//**********************************************************************************************************************
#include "Entity.h"
#include "Log.h"
#include "Game.h"
#include <assert.h>

//**********************************************************************************************************************
const char* Entity::ToString() const
{
	sprintf_s((char*)_str_tmp, sizeof(_str_tmp), "this=%p  local=%s  type=%s (0x%08x)  id=%lu  name=%s  owner=%s",
		this,
		IsMaster() ? "true" : "false",
		GetType().Content().c_str(),
		GetType().Id(),
		_id,
		_name.c_str(),
		_peer.ToString());

	return _str_tmp;
}

//**********************************************************************************************************************
void Entity::ReadFromContainer(uu::network::DataContainer const& container)
{
	CreateEntityRequest const& data = dynamic_cast<CreateEntityRequest const&>(container);

	_name = data._name;
	_peer = data._owner;
	_id = data._id;
	SetPosition(data._x, data._y);
}

//**********************************************************************************************************************
void Entity::WriteToContainer(uu::network::DataContainer& container) const
{
	CreateEntityRequest& data = dynamic_cast<CreateEntityRequest&>(container);

	data._type = GetType().Id();
	data._name = _name;
	data._owner = _peer;
	data._id = _id;
	sf::Vector2f position;
	GetPosition(position);
	data._x = position.x;
	data._y = position.y;
}


//**********************************************************************************************************************
bool Entity::SendDataContainerToMaster(uu::network::DataContainer& datacontainer)
{
	assert(IsReplica() == true);

	return Game::GetInstance().SendDataContainer(datacontainer, _peer);
}

//**********************************************************************************************************************
bool Entity::BroadcastDataContainerToReplicas(uu::network::DataContainer& datacontainer)
{
	assert(IsMaster() == true);

	return Game::GetInstance().SendDataContainerToSessionClients(datacontainer);
}

//**********************************************************************************************************************
bool Entity::SendDataContainerToReplica(uu::network::DataContainer& datacontainer, uu::network::IPEndPoint const& peertarget)
{
    assert(IsMaster() == true);

    return Game::GetInstance().SendDataContainer(datacontainer, peertarget);
}

