//**********************************************************************************************************************
#pragma once

#include "types.h"

#include "serializer.h"
#include "DataContainer.h"
#include "StringId.h"
#include "IPEndPoint.h"

//**********************************************************************************************************************
/*
DataContainer hierachy:

DataContainer
\____CreateEntityRequest
|	\____CreateCharacterRequest
|	|		\____CreatePlayerRequest
|	|		\____CreateEnemyRequest
|	\____CreateBombRequest
*/

//**********************************************************************************************************************
class CreateEntityRequest: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _type;
	std::string _name;
	uu::u32 _id;
	uu::network::IPEndPoint _owner;
	float _x;
	float _y;

};

//**********************************************************************************************************************
class CreateCharacterRequest: public CreateEntityRequest
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	float _speed;
	float _power;
	float _live;
	float _view_range;
	float _attack_range;
	float _detect_range;

	uu::u32 _entity_to_follow;
	uu::u32 _entity_to_attack;
};

//**********************************************************************************************************************
class CreatePlayerRequest: public CreateCharacterRequest
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _coins;

};

