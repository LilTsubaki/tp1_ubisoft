//**********************************************************************************************************************
#pragma once

//**********************************************************************************************************************
#include "types.h"
#include "SFML/Graphics.hpp"
#include "Maths.h"

//**********************************************************************************************************************
namespace uu //univubi
{
//**********************************************************************************************************************
namespace maths
{
//**********************************************************************************************************************
template<typename T>
double Vector2Length2(sf::Vector2<T>& vector)
{
	double length2 = (vector.x * vector.x) + (vector.y * vector.y);
	return length2;
}

//**********************************************************************************************************************
template<typename T>
double Vector2Length(sf::Vector2<T>& vector)
{
	double length = 0;
	double length2 = (vector.x * vector.x) + (vector.y * vector.y);
	if (length2 != 0)
	{
		length = sqrt(length2);
	}
	return length;
}

//**********************************************************************************************************************
template<typename T>
void Vector2Normalize(sf::Vector2<T>& vector)
{
	double length = Vector2Length(vector);
	if (length != 0)
	{
		vector.x = (T)(vector.x / length);
		vector.y = (T)(vector.y / length);
	}
}

//**********************************************************************************************************************
template<typename T>
T	Vector2Dot(sf::Vector2<T> const& v1, sf::Vector2<T> const& v2)
{
	sf::Vector2<T> a = v1;
	sf::Vector2<T> b = v2;

	Vector2Normalize(a);
	Vector2Normalize(b);

	T dot = (a.x * b.x) + (a.y * b.y);

	return dot;
	/*
	Dot(v1,v2)=(dx1*dx2)+(dy1*dy2)
	If the Dot Product is +1, the unit vectors are both pointing in the same direction.
	If the Dot Product is zero, the unit vectors are perpendicular (at right-angles to each other).
	If the Dot Product is -1, the unit vectors are pointing in opposite directions.
	*/
}

//**********************************************************************************************************************
template<typename T>
bool Vector2AreOrthogonal(sf::Vector2<T> const& v1, sf::Vector2<T> const& v2)
{
	T dot = (v1.x * v2.x) + (v1.y * v2.y);

	return (dot == 0);
}

//**********************************************************************************************************************
template<typename T>
bool Vector2AreColinear(sf::Vector2<T> const& v1, sf::Vector2<T> const& v2)
{
	T dot = (v1.x * v2.x) + (v1.y * v2.y);

	return (dot == 1 || dot == -1);
}

//**********************************************************************************************************************
template<typename T>
sf::Vector2<T> Vector2Cross(sf::Vector2<T> const& v1, sf::Vector2<T> const& v2)
{
	sf::Vector2<T> result;
	result.x = (v1.x * v2.y) - (v1.y * v2.x);
	result.y = result.x;
	return result;
}

//**********************************************************************************************************************
template<typename T>
T Vector2Distance2(sf::Vector2<T> const& v1, sf::Vector2<T> const& v2)
{
	T diff_x = v2.x - v1.x;
	T diff_y = v2.y - v1.y;

	T distance2 = (diff_x * diff_x) + (diff_y * diff_y);

	return distance2;
}

//**********************************************************************************************************************
template<typename T>
double Vector2Distance(sf::Vector2<T> const& v1, sf::Vector2<T> const& v2)
{
	double distance = 0;
	T distance2 = Vector2Distance2(v1, v2);
	if (distance2 != 0)
	{
		distance = sqrt(distance2);
	}
	return distance;
}

} //namespace maths

} //namespace univubi
