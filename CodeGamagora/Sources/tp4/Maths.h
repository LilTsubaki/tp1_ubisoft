//**********************************************************************************************************************
#pragma once

//**********************************************************************************************************************
#include "types.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace maths
{
	static const float Pi = 3.141592653589793f;	//180°
	static const float TwoPi = 2 * Pi;			//360°

	// 1 radian = 57.295779513 degrees
	// 1 degree = 0.0174532925 radians
	static const float RadInDeg = 57.295779513f;
	static const float DegInRad = 0.0174532925f;

#define Radians2Degrees(_rad) ((_rad) * uu::maths::RadInDeg)
#define Degrees2Radians(_deg) ((_deg) * uu::maths::DegInRad)

} //namespace maths

} //namespace univubi
