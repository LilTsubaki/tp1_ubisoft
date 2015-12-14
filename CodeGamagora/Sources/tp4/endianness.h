//**********************************************************************************************************************
#pragma once
//**********************************************************************************************************************
// Disable warning messages
#pragma warning( disable : 4700 4100 4127)

//**********************************************************************************************************************
#include "types.h"

#define _USE_TEMPLATE_SWAP_FUNC_

//**********************************************************************************************************************
namespace uu //univubi
{
	//**********************************************************************************************************************
	enum Endianness
	{
		eUndefined = 0,
		eBigEndian = 1,
		eLittleEndian = 2,
		eMiddleEndian = 3,
		eIntelEndian = eBigEndian,
		eMotorolaEndian = eLittleEndian,
		eNetworkEndian = eBigEndian,
	};

	//**********************************************************************************************************************
	namespace system
	{
		//**********************************************************************************************************************
		void DetermineEndianness();

		uu::Endianness GetEndianness();
		bool IsBigEndian();
		bool IsLittleEndian();

		//**********************************************************************************************************************
		void _Swap_Bytes(void const* ptr_in, void* ptr_out, size_t size_in_bytes);

		//**********************************************************************************************************************
#ifdef _USE_TEMPLATE_SWAP_FUNC_

		template<typename T>
		inline T BigToLittleEndian(T be_value)
		{
			T le_value = 0;
			_Swap_Bytes(&be_value, &le_value, sizeof(T));

			return le_value;
		}

		//**********************************************************************************************************************
		template<typename T>
		inline T LittleToBigEndian(T le_value)
		{
			T be_value = 0;
			_Swap_Bytes(&le_value, &be_value, sizeof(T));
			return be_value;
		}

#else // _USE_TEMPLATE_SWAP_FUNC_

		//**********************************************************************************************************************
		u16 BigToLittleEndian(u16 be_value);
		s16 BigToLittleEndian(s16 be_value);
		u32 BigToLittleEndian(u32 be_value);
		s32 BigToLittleEndian(s32 be_value);
		u64 BigToLittleEndian(u64 be_value);
		s64 BigToLittleEndian(s64 be_value);

		u16 LittleToBigEndian(u16 le_value);
		s16 LittleToBigEndian(s16 le_value);
		u32 LittleToBigEndian(u32 le_value);
		s32 LittleToBigEndian(s32 le_value);
		u64 LittleToBigEndian(u64 le_value);
		s64 LittleToBigEndian(s64 le_value);
#endif // _USE_TEMPLATE_SWAP_FUNC_

	} //namespace system

} //namespace univubi