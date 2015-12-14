//**********************************************************************************************************************
#include "endianness.h"
//#include <assert.h>

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace system
{
	static Endianness _endianness = Endianness::eUndefined;

	//**********************************************************************************************************************
	void DetermineEndianness()
	{
		//TO DO: determine the platform Endianness and set the right value to the _endianness varaible
		static const unsigned short e = 0x0102;
		if ( (*(char*)&e) == 0x01 )
			_endianness = Endianness::eBigEndian;
		else if ( (*(char*)&e) == 0x02 )
			_endianness = Endianness::eLittleEndian;
	}

	//**********************************************************************************************************************
	Endianness GetEndianness()
	{
		return _endianness ;
	}

	//**********************************************************************************************************************
	bool IsBigEndian()
	{
		return (_endianness == Endianness::eBigEndian);
	}

	//**********************************************************************************************************************
	bool IsLittleEndian()
	{
		return (_endianness == Endianness::eLittleEndian);
	}

	//**********************************************************************************************************************
	void _Swap_Bytes(void const* ptr_in, void* ptr_out, size_t size_in_bytes)
	{
		//swap with pointers
		byte* ptr_read = (byte*)ptr_in;
		byte* ptr_write = (byte*)ptr_out;

		//assert(ptr_read != ptr_write);

		if (size_in_bytes == 2)
		{
			ptr_write[0] = ptr_read[1];
			ptr_write[1] = ptr_read[0];
		}
		else if (size_in_bytes == 4)
		{
			ptr_write[0] = ptr_read[3];
			ptr_write[1] = ptr_read[2];
			ptr_write[2] = ptr_read[1];
			ptr_write[3] = ptr_read[0];
		}
		else if (size_in_bytes == 8)
		{
			ptr_write[0] = ptr_read[7];
			ptr_write[1] = ptr_read[6];
			ptr_write[2] = ptr_read[5];
			ptr_write[3] = ptr_read[4];
			ptr_write[4] = ptr_read[3];
			ptr_write[5] = ptr_read[2];
			ptr_write[6] = ptr_read[1];
			ptr_write[7] = ptr_read[0];
		}
	}

#ifndef _USE_TEMPLATE_SWAP_FUNC_

	//**********************************************************************************************************************
	u16 BigToLittleEndian(u16 be_value)
	{
		//TO DO : convert be_value from BigEndian to LittleEndian
		u16 le_value = 0;
		_Swap_Bytes(&be_value, &le_value, sizeof(u16));

		return le_value;
	}

	//**********************************************************************************************************************
	s16 BigToLittleEndian(s16 be_value)
	{
		//TO DO : convert be_value from BigEndian to LittleEndian
		s16 le_value = 0;
		_Swap_Bytes(&be_value, &le_value, sizeof(s16));

		return le_value;
	}

	//**********************************************************************************************************************
	u32 BigToLittleEndian(u32 be_value)
	{
		//TO DO : convert be_value from BigEndian to LittleEndian
		u32 le_value = 0;
		_Swap_Bytes(&be_value, &le_value, sizeof(u32));

		return le_value;
	}

	//**********************************************************************************************************************
	s32 BigToLittleEndian(s32 be_value)
	{
		//TO DO : convert be_value from BigEndian to LittleEndian
		s32 le_value = 0;
		_Swap_Bytes(&be_value, &le_value, sizeof(s32));

		return le_value;
	}

	//**********************************************************************************************************************
	u64 BigToLittleEndian(u64 be_value)
	{
		//TO DO : convert be_value from BigEndian to LittleEndian
		u64 le_value = 0;
		_Swap_Bytes(&be_value, &le_value, sizeof(u64));

		return le_value;
	}

	//**********************************************************************************************************************
	s64 BigToLittleEndian(s64 be_value)
	{
		//TO DO : convert be_value from BigEndian to LittleEndian
		s64 le_value = 0;
		_Swap_Bytes(&be_value, &le_value, sizeof(s64));

		return le_value;
	}

	//**********************************************************************************************************************
	u16 LittleToBigEndian(u16 le_value)
	{
		//TO DO : convert le_value from LittleEndian to BigEndian
		u16 be_value = 0;
		_Swap_Bytes(&le_value, &be_value, sizeof(u16));

		return be_value;
	}

	//**********************************************************************************************************************
	s16 LittleToBigEndian(s16 le_value)
	{
		//TO DO : convert le_value from LittleEndian to BigEndian
		s16 be_value = 0;
		_Swap_Bytes(&le_value, &be_value, sizeof(s16));

		return be_value;
	}

	//**********************************************************************************************************************
	u32 LittleToBigEndian(u32 le_value)
	{
		//TO DO : convert le_value from LittleEndian to BigEndian
		u32 be_value = 0;
		_Swap_Bytes(&le_value, &be_value, sizeof(u32));

		return be_value;
	}

	//**********************************************************************************************************************
	s32 LittleToBigEndian(s32 le_value)
	{
		//TO DO : convert le_value from LittleEndian to BigEndian
		s32 be_value = 0;
		_Swap_Bytes(&le_value, &be_value, sizeof(s32));

		return be_value;
	}

	//**********************************************************************************************************************
	u64 LittleToBigEndian(u64 le_value)
	{
		//TO DO : convert le_value from LittleEndian to BigEndian
		u64 be_value = 0;
		_Swap_Bytes(&le_value, &be_value, sizeof(u64));

		return be_value;
	}

	//**********************************************************************************************************************
	s64 LittleToBigEndian(s64 le_value)
	{
		//TO DO : convert le_value from LittleEndian to BigEndian
		s64 be_value = 0;
		_Swap_Bytes(&le_value, &be_value, sizeof(s64));

		return be_value;
	}
#endif // _USE_TEMPLATE_SWAP_FUNC_

} //namespace system

} //namespace univubi