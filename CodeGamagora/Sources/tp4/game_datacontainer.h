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
\____GotoObjectRequest
\____FollowObjectRequest
\____AttackObjectRequest
\____HitObjectRequest

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
	uu::u32 _bomb_id;
	uu::u32 _enemy_id;
	uu::u32 _coins;

};

//**********************************************************************************************************************
class CreateEnemyRequest: public CreateCharacterRequest
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:

};

//**********************************************************************************************************************
class CreateBombRequest: public CreateEntityRequest
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	time_t _explosion_time;
	float _explosion_radius;
	float _current_radius;
	float _power;

};

//**********************************************************************************************************************
class GotoObjectRequest: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _id;
	float _x;
	float _y;
};

//**********************************************************************************************************************
class FollowObjectRequest: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _id;
	uu::u32 _id_to_follow;
};

//**********************************************************************************************************************
class AttackObjectRequest: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _id_attacker;
	uu::u32 _id_to_attack;
};

//**********************************************************************************************************************
class HitObjectRequest: public uu::network::DataContainer
{
public:
	static uu::StringId dataContainerId;

public:
	//uu::network::DataContainer overrides
	virtual uu::StringId const& GetDataContainerId() const { return dataContainerId; }
	virtual bool ReadFromNetworkData(uu::Reader& reader, uu::network::IPEndPoint const& from_addr);
	virtual bool WriteToNetworkData(uu::Writer& writer);

public:
	uu::u32 _id_attacker;
	uu::u32 _id_to_hit;
	float _hit_value;
};
