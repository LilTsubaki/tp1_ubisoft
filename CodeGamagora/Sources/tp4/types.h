//**********************************************************************************************************************
#pragma once
//**********************************************************************************************************************
// Disable warning messages
#pragma warning( disable : 4700 4100 4127)

#include <string>
#include <vector>
#include <list>
#include <map>
#include "Vector2.h"
#include "Ellipse2.h"

//**********************************************************************************************************************
//extern void* operator new(size_t bytes);
//extern void* operator new[](size_t bytes);
//extern void operator delete(void* ptr);
//extern void operator delete[](void* ptr);

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
	typedef unsigned char             byte;

	typedef unsigned char             u8;
	typedef signed char               s8;
	typedef unsigned short int        u16;
	typedef signed short int          s16;
	typedef unsigned long int         u32;
	typedef signed long int           s32;
	typedef unsigned long long int    u64;
	typedef signed long long int      s64;
	typedef float                     f32;
	typedef double                    f64;

	typedef sf::Vector2<float>        Vector2f;
	typedef sf::Vector2<double>       Vector2d;
	typedef uu::maths::Ellipse2<float>    Ellipse2f;
	typedef uu::maths::Ellipse2<double>   Ellipse2d;

//**********************************************************************************************************************
namespace program
{
	//**********************************************************************************************************************
	bool Is32Bits();
	bool Is64Bits();

} //namespace system

} //namespace univubi