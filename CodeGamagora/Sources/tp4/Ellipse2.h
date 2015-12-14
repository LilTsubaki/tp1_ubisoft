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
class Ellipse2
{
public:
	Ellipse2();
	Ellipse2(sf::Vector2<T> const& center, sf::Vector2<T> const& radius);
	Ellipse2(sf::Rect<T> const& rect);
	Ellipse2(Ellipse2<T> const& other);
	Ellipse2<T>& operator=(Ellipse2<T> const& other);
	Ellipse2<T>& operator=(sf::Rect<T> const& rect);

	bool IsPointInside(sf::Vector2<T> const& point) const;

	sf::Vector2<T> _center;
	sf::Vector2<T> _radius;
};

//**********************************************************************************************************************
template<typename T>
Ellipse2<T>::Ellipse2()
{
	_center.x = _center.y = 0.f;
	_radius.x = _radius.y = 0.f;
}

//**********************************************************************************************************************
template<typename T>
Ellipse2<T>::Ellipse2(sf::Vector2<T> const& center, sf::Vector2<T> const& radius):
	_center(center),
	_radius(radius)
{
}

//**********************************************************************************************************************
template<typename T>
Ellipse2<T>::Ellipse2(sf::Rect<T> const& rect)
{
	_radius.x = rect.width * 0.5f;
	_radius.y = rect.height * 0.5f;

	_center.x = rect.left + _radius.x;
	_center.y = rect.top + _radius.y;
}

//**********************************************************************************************************************
template<typename T>
Ellipse2<T>::Ellipse2(Ellipse2<T> const& other):
	_center(other._center),
	_radius(other._radius)
{
}

//**********************************************************************************************************************
template<typename T>
Ellipse2<T>& Ellipse2<T>::operator=(Ellipse2<T> const& other)
{
	_center = other._center;
	_radius = other._radius;

	return *this;
}

//**********************************************************************************************************************
template<typename T>
Ellipse2<T>& Ellipse2<T>::operator=(sf::Rect<T> const& rect)
{
	_radius.x = rect.width * 0.5f;
	_radius.y = rect.height * 0.5f;

	_center.x = rect.left + _radius.x;
	_center.y = rect.top + _radius.y;

	return *this;
}

//**********************************************************************************************************************
template<typename T>
bool Ellipse2<T>::IsPointInside(sf::Vector2<T> const& point) const
{
	sf::FloatRect rect;

	rect.width = _radius.x * 2.f;
	rect.height = _radius.y * 2.f;

	rect.left = _center.x - _radius.x;
	rect.top = _center.y - _radius.y;

	return rect.contains(point.x, point.y);
}

//**********************************************************************************************************************

} //namespace maths

} //namespace univubi
