//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "endianness.h"
#include "IPEndPoint.h"
#include "Vector2.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
class Serializer
{
public:

	Serializer(void* bytes, size_t size, Endianness endianness);

	size_t GetCapacity() const { return _capacity; }
	size_t GetLength() const { return _offset; }
	size_t GetSize() const { return _offset; }
	size_t GetSizeLeft() const { return (_capacity - _offset); }
	uu::byte* GetDataOffset() const { return &_buffer[_offset]; }

	virtual uu::byte* GetData() const = 0 ;

	void Reset();

protected:
	Endianness _endianness;
	size_t _offset;
	size_t _capacity;
	uu::byte* _buffer;

	Serializer();
	Serializer(Serializer const&);
};

//**********************************************************************************************************************
class Writer : public Serializer
{
public:
	Writer(void* bytes, size_t size, Endianness endianness);

	virtual uu::byte* GetData() const { return _buffer; }

	bool WriteBool(bool value);
	bool WriteByte(uu::byte value);
	bool WriteInt16(uu::s16 value);
	bool WriteUInt16(uu::u16 value);
	bool WriteInt32(uu::s32 value);
	bool WriteUInt32(uu::u32 value);
	bool WriteInt64(uu::s64 value);
	bool WriteUInt64(uu::u64 value);
	bool WriteFloat(uu::f32 value);
	bool WriteDouble(uu::f64 value);
	bool WriteString(std::string const& value);
	bool WriteIPEndPoint(uu::network::IPEndPoint const& value);

protected:
	void Writer::_WriteBytes(const uu::byte* bytes, size_t size);

};

//**********************************************************************************************************************
class Reader : public Serializer
{
public:
	Reader(void* bytes, size_t size, Endianness endianness);

	virtual uu::byte* GetData() const { return (uu::byte*)_buffer; }

	bool ReadBool(bool& value);
	bool ReadByte(uu::byte& value);
	bool ReadInt16(uu::s16& value);
	bool ReadUInt16(uu::u16& value);
	bool ReadInt32(uu::s32& value);
	bool ReadUInt32(uu::u32& value);
	bool ReadInt64(uu::s64& value);
	bool ReadUInt64(uu::u64& value);
	bool ReadFloat(uu::f32& value);
	bool ReadDouble(uu::f64& value);
	bool ReadString(std::string& value);
	bool ReadIPEndPoint(uu::network::IPEndPoint& value);

protected:
	void Reader::_ReadBytes(uu::byte* bytes, size_t size);

};

} //namespace univubi