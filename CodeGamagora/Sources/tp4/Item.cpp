#include "Item.h"

//**********************************************************************************************************************
//**********************************************************************************************************************
Item::Item(const char* name) : Animable(name)
{
}

uu::network::DataContainer* Item::CreateContainer() const
{
	return new CreateEntityRequest();
}

void Item::ReadFromContainer(uu::network::DataContainer const& container)
{
	Entity::ReadFromContainer(container);
}

void Item::WriteToContainer(uu::network::DataContainer& container) const
{
	Entity::WriteToContainer(container);
}

//**********************************************************************************************************************
bool Item::Initialize()
{
	Animable::Initialize();

	_SetAnimation("idle", true);

	return true;
}
