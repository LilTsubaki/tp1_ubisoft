//**********************************************************************************************************************
#include "Animable.h"
#include "Maths.h"
#include "Vector2.h"

#include "SFML/Graphics.hpp"
#include "Log.h"
#include "ObjectOrientation.h"
#include "Game.h"

//**********************************************************************************************************************
uu::StringId Animable::type = uu::StringId("Animable");

//**********************************************************************************************************************
Animable::Animable(const char* name):
	Entity(),
	_sprite(name)
{
}

//**********************************************************************************************************************
Animable::~Animable()
{
}

//**********************************************************************************************************************
bool Animable::Initialize()
{
	_orientation.x = _orientation.y = 1.f;

	_position.x  = 50.f + fmodf((float)rand(), (Configuration::GetInstance()._area_width - 100.f));
	_position.y  = 50.f + fmodf((float)rand(), (Configuration::GetInstance()._area_height - 100.f));

	_sprite.Init();

	return true;
}

//**********************************************************************************************************************
bool Animable::Update(time_t _time_now)
{
	_sprite.SetPosition(_position);
	_sprite.Refresh();

	return true;
}

//**********************************************************************************************************************
bool Animable::Terminate()
{
	_sprite.Term();

	return true;
}

//**********************************************************************************************************************
void Animable::Draw(sf::RenderTarget& target, bool bDebugDraw)
{
	_sprite.Draw(target);
}

//**********************************************************************************************************************
bool Animable::PickTest(float x, float y) const
{
	sf::FloatRect rect = _sprite.GetGlobalBounds();

	return rect.contains(x, y);
}

//**********************************************************************************************************************
bool Animable::_SetAnimation(const char* name, bool loop)
{
	ObjectOrientation result = GetObjectOrientation(_orientation);

	std::string animation = name;
	animation += "_";
	animation += strObjectOrientation[(int)result];

	if (_sprite.SetAnimation(animation.c_str(), loop) == nullptr)
	{
		return (_sprite.SetAnimation(name, loop) != nullptr);
	}

	return true;
}

//**********************************************************************************************************************
void Animable::GetPositionEllipse(sf::Vector2f& center, sf::Vector2f& radius) const
{
	sf::FloatRect rect = _sprite.GetGlobalBounds();

	center.x = rect.left + (rect.width * 0.5f);
	center.y = rect.top + rect.height;
	center.y -= 2.f;

	radius.x = 1.f;
	radius.y = 0.33f;
}

//**********************************************************************************************************************
void Animable::GetPositionEllipse(uu::Ellipse2f& ellipse) const
{
	sf::FloatRect rect = _sprite.GetGlobalBounds();

	ellipse._center.x = rect.left + (rect.width * 0.5f);
	ellipse._center.y = rect.top + rect.height;
	ellipse._center.y -= 2.f;

	ellipse._radius.x = 1.f;
	ellipse._radius.y = 0.33f;
}

//**********************************************************************************************************************
bool Animable::IsNear(sf::Vector2f const& point, float distance) const
{
	uu::Ellipse2f ellipse;
	GetPositionEllipse(ellipse);

	ellipse._radius *= distance;

	return (ellipse.IsPointInside(point));
}