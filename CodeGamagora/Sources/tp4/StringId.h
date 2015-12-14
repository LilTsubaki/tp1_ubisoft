//**********************************************************************************************************************
#pragma once

#include "types.h"

//**********************************************************************************************************************
namespace uu //univubi
{

//**********************************************************************************************************************
class StringId
{
public:
	StringId();
	StringId(const char* content);
	StringId(StringId const& other);
	StringId(std::string const& content);

	StringId& operator=(StringId const& other);
	StringId& operator=(std::string const& content);
	StringId& operator=(const char* content);

	bool IsValid() const { return (_id != 0 && _content.empty() == false); }

	bool operator==(u32 id) const { return (_id == id); }
	bool operator!=(u32 id) const { return (_id != id); }
	bool operator<(u32 id) const { return (_id < id); }
	bool operator<=(u32 id) const { return (_id <= id); }
	bool operator>(u32 id) const { return (_id > id); }
	bool operator>=(u32 id) const { return (_id >= id); }

	bool operator==(StringId const& other) const { return (_id == other._id); }
	bool operator!=(StringId const& other) const { return (_id != other._id); }
	bool operator<(StringId const& other) const { return (_id < other._id); }
	bool operator<=(StringId const& other) const { return (_id <= other._id); }
	bool operator>(StringId const& other) const { return (_id > other._id); }
	bool operator>=(StringId const& other) const { return (_id >= other._id); }

	u32 Id() const { return _id; }
	std::string const& Content() const { return _content; }

private:
	u32 _id;
	std::string _content;

	void _SetContent(const char* content);

};

} //namespace univubi
