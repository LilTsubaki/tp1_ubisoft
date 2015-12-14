//**********************************************************************************************************************
#pragma once

#include "Animable.h"


//**********************************************************************************************************************
class Item: public Animable
{
public:
	static uu::StringId type;
	//Entity overrides
	virtual uu::StringId const& GetType() const { return type; }
	virtual bool IsA(uu::StringId const& type) const { if (this->type == type) return true; return Animable::IsA(type); }
	virtual uu::network::DataContainer* CreateContainer() const;

public:
	Item(const char* name);

	//Entity overrides
	virtual bool Initialize();

protected:
	//Entity overrides
	virtual void ReadFromContainer(uu::network::DataContainer const& container);
	virtual void WriteToContainer(uu::network::DataContainer& container) const;
};