//**********************************************************************************************************************
#include "types.h"
#include "Windows.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace program
{
	//**********************************************************************************************************************
	bool Is32Bits()
	{
		return (sizeof(void*) == 4);
	}

	//**********************************************************************************************************************
	bool Is64Bits()
	{
		return (sizeof(void*) == 8);
	}

} //namespace system

} //namespace univubi


//**********************************************************************************************************************
/*void* operator new(size_t bytes)
{
	void* ptr = malloc(bytes);

	char tmp_str[1024];
	sprintf_s(tmp_str, "[NEW]  ptr=%p  size=%lu bytes\n", ptr, bytes);
	OutputDebugStringA(tmp_str);

	return ptr;
}

//**********************************************************************************************************************
void operator delete(void* ptr)
{
	char tmp_str[1024];
	sprintf_s(tmp_str, "[DEL]  ptr=%p\n", ptr);
	OutputDebugStringA(tmp_str);

	if (ptr != nullptr)
		free(ptr);
}*/

//**********************************************************************************************************************