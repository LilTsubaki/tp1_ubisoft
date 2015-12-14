//**********************************************************************************************************************
#include "Serializer.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
//**********************************************************************************************************************
Serializer::Serializer(void* bytes, size_t size, Endianness endianness):
	_offset(0),
	_capacity(size),
	_buffer((uu::byte*)bytes),
	_endianness(endianness)
{
}

//**********************************************************************************************************************
void Serializer::Reset()
{
	_offset = 0;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
Writer::Writer(void* bytes, size_t size, Endianness endianness): Serializer(bytes, size, endianness)
{
}

//**********************************************************************************************************************
void Writer::_WriteBytes(const uu::byte* bytes, size_t size)
{
	if (size == 2)
	{
		if (_endianness != uu::system::GetEndianness())
		{
			_buffer[_offset++] = bytes[1];
			_buffer[_offset++] = bytes[0];
		}
		else
		{
			_buffer[_offset++] = bytes[0];
			_buffer[_offset++] = bytes[1];
		}
	}
	else if (size == 4)
	{
		if (_endianness != uu::system::GetEndianness())
		{
			_buffer[_offset++] = bytes[3];
			_buffer[_offset++] = bytes[2];
			_buffer[_offset++] = bytes[1];
			_buffer[_offset++] = bytes[0];
		}
		else
		{
			_buffer[_offset++] = bytes[0];
			_buffer[_offset++] = bytes[1];
			_buffer[_offset++] = bytes[2];
			_buffer[_offset++] = bytes[3];
		}
	}
	else if (size == 8)
	{
		if (_endianness != uu::system::GetEndianness())
		{
			_buffer[_offset++] = bytes[7];
			_buffer[_offset++] = bytes[6];
			_buffer[_offset++] = bytes[5];
			_buffer[_offset++] = bytes[4];
			_buffer[_offset++] = bytes[3];
			_buffer[_offset++] = bytes[2];
			_buffer[_offset++] = bytes[1];
			_buffer[_offset++] = bytes[0];
		}
		else
		{
			_buffer[_offset++] = bytes[0];
			_buffer[_offset++] = bytes[1];
			_buffer[_offset++] = bytes[2];
			_buffer[_offset++] = bytes[3];
			_buffer[_offset++] = bytes[4];
			_buffer[_offset++] = bytes[5];
			_buffer[_offset++] = bytes[6];
			_buffer[_offset++] = bytes[7];
		}
	}
}

//**********************************************************************************************************************
bool Writer::WriteBool(bool value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::byte)) > _capacity)
		return false;

	if (value == true)
		_buffer[_offset++] = 1;
	else
		_buffer[_offset++] = 0;

	return true;
}

//**********************************************************************************************************************
bool Writer::WriteByte(uu::byte value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::byte)) > _capacity)
		return false;

	_buffer[_offset++] = value;

	return true;
}

//**********************************************************************************************************************
bool Writer::WriteInt16(uu::s16 value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::s16)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_WriteBytes(bytes, sizeof(uu::s16));

	return true;
}

//**********************************************************************************************************************
bool Writer::WriteUInt16(uu::u16 value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::u16)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_WriteBytes(bytes, sizeof(uu::u16));

	return true;
}

//**********************************************************************************************************************
bool Writer::WriteInt32(uu::s32 value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::s32)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_WriteBytes(bytes, sizeof(uu::s32));

	return true;
}

//**********************************************************************************************************************
bool Writer::WriteUInt32(uu::u32 value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::u32)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_WriteBytes(bytes, sizeof(uu::u32));

	return true;
}

//**********************************************************************************************************************
bool Writer::WriteInt64(uu::s64 value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::s64)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_WriteBytes(bytes, sizeof(uu::s64));

	return true;
}

//**********************************************************************************************************************
bool Writer::WriteUInt64(uu::u64 value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::u64)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_WriteBytes(bytes, sizeof(uu::u64));

	return true;
}

//**********************************************************************************************************************
bool Writer::WriteFloat(uu::f32 value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::f32)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_WriteBytes(bytes, sizeof(uu::f32));

	return true;
}

//**********************************************************************************************************************
bool Writer::WriteDouble(uu::f64 value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::f64)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_WriteBytes(bytes, sizeof(uu::f64));

	return true;
}

//**********************************************************************************************************************
bool Writer::WriteString(std::string const& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::u16) + value.length()) > _capacity)
		return false;

	uu::u16 size = (uu::u16)value.length();

	if (WriteUInt16(size) == false)
		return false;

	const uu::byte* bytes = (const uu::byte*)value.c_str();

	memcpy(&_buffer[_offset], bytes, size);
	_offset += size;

	return true;
}

//**********************************************************************************************************************
bool Writer::WriteIPEndPoint(uu::network::IPEndPoint const& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::u32) + sizeof(uu::u16)) > _capacity)
		return false;

	if (WriteUInt32(value.GetAddress()) == false)
		return false;

	if (WriteUInt16(value.GetPort()) == false)
		return false;

	return true;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
Reader::Reader(void* bytes, size_t size, Endianness endianness): Serializer(bytes, size, endianness)
{
}

//**********************************************************************************************************************
void Reader::_ReadBytes(uu::byte* bytes, size_t size)
{
	if (size == 2)
	{
		if (_endianness != uu::system::GetEndianness())
		{
			bytes[1] = _buffer[_offset++];
			bytes[0] = _buffer[_offset++];
		}
		else
		{
			bytes[0] = _buffer[_offset++];
			bytes[1] = _buffer[_offset++];
		}
	}
	else if (size == 4)
	{
		if (_endianness != uu::system::GetEndianness())
		{
			bytes[3] = _buffer[_offset++];
			bytes[2] = _buffer[_offset++];
			bytes[1] = _buffer[_offset++];
			bytes[0] = _buffer[_offset++];
		}
		else
		{
			bytes[0] = _buffer[_offset++];
			bytes[1] = _buffer[_offset++];
			bytes[2] = _buffer[_offset++];
			bytes[3] = _buffer[_offset++];
		}
	}
	else if (size == 8)
	{
		if (_endianness != uu::system::GetEndianness())
		{
			bytes[7] = _buffer[_offset++];
			bytes[6] = _buffer[_offset++];
			bytes[5] = _buffer[_offset++];
			bytes[4] = _buffer[_offset++];
			bytes[3] = _buffer[_offset++];
			bytes[2] = _buffer[_offset++];
			bytes[1] = _buffer[_offset++];
			bytes[0] = _buffer[_offset++];
		}
		else
		{
			bytes[0] = _buffer[_offset++];
			bytes[1] = _buffer[_offset++];
			bytes[2] = _buffer[_offset++];
			bytes[3] = _buffer[_offset++];
			bytes[4] = _buffer[_offset++];
			bytes[5] = _buffer[_offset++];
			bytes[6] = _buffer[_offset++];
			bytes[7] = _buffer[_offset++];
		}
	}
}

//**********************************************************************************************************************
bool Reader::ReadBool(bool& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::byte)) > _capacity)
		return false;

	uu::byte val = _buffer[_offset++];
	if (val == 0)
		value = false;
	else
		value = true;

	return true;
}

//**********************************************************************************************************************
bool Reader::ReadByte(uu::byte& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::byte)) > _capacity)
		return false;

	value = _buffer[_offset++];

	return true;
}

//**********************************************************************************************************************
bool Reader::ReadInt16(uu::s16& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::s16)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_ReadBytes(bytes, sizeof(uu::s16));

	return true;
}

//**********************************************************************************************************************
bool Reader::ReadUInt16(uu::u16& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::u16)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_ReadBytes(bytes, sizeof(uu::u16));

	return true;
}

//**********************************************************************************************************************
bool Reader::ReadInt32(uu::s32& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::s32)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_ReadBytes(bytes, sizeof(uu::s32));

	return true;
}

//**********************************************************************************************************************
bool Reader::ReadUInt32(uu::u32& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::u32)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_ReadBytes(bytes, sizeof(uu::u32));

	return true;
}

//**********************************************************************************************************************
bool Reader::ReadInt64(uu::s64& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::s64)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_ReadBytes(bytes, sizeof(uu::s64));

	return true;
}

//**********************************************************************************************************************
bool Reader::ReadUInt64(uu::u64& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::u64)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_ReadBytes(bytes, sizeof(uu::u64));

	return true;
}

//**********************************************************************************************************************
bool Reader::ReadFloat(uu::f32& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::f32)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_ReadBytes(bytes, sizeof(uu::f32));

	return true;
}

//**********************************************************************************************************************
bool Reader::ReadDouble(uu::f64& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::f64)) > _capacity)
		return false;

	uu::byte* bytes = (uu::byte*)&value;

	_ReadBytes(bytes, sizeof(uu::f64));

	return true;
}

//**********************************************************************************************************************
bool Reader::ReadString(std::string& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::u16)) > _capacity)
		return false;

	uu::u16 size = 0;

	if (ReadUInt16(size) == false)
		return false;

	if ((_offset + size) > _capacity)
		return false;

	value.clear();
	for (uu::u16 i = 0; i < size; ++i)
	{
		value += _buffer[_offset++];
	}

	return true;
}

//**********************************************************************************************************************
bool Reader::ReadIPEndPoint(uu::network::IPEndPoint& value)
{
	if (_buffer == nullptr || _capacity == 0)
		return false;

	if ((_offset + sizeof(uu::u32) + sizeof(uu::u16)) > _capacity)
		return false;

	uu::u32 ip = 0;
	uu::u16 port = 0;

	if (ReadUInt32(ip) == false)
		return false;
	if (ReadUInt16(port) == false)
		return false;

	value.SetAddress(ip);
	value.SetPortA(port);

	return true;
}

} //namespace univubi