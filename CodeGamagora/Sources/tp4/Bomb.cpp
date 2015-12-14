#include "bomb.h"
#include "draw_utils.h"
#include "Game.h"
#include "Time.h"

//**********************************************************************************************************************
Bomb::Bomb() : Item("Bomb"),
	_state(none),
	_explosion_time(0),
	_explosion_radius(0.f),
	_current_radius(0.f),
	_power(300.f)
{
	_current_radius = 10.f;
	_explosion_radius = 150.f;
	_explosion_time = uu::Time::GetSynchTime() + 4000; // TEMP : fix that for network latency handling

	_label.SetFontSize(14);
	_label.SetStyle(sf::Text::Bold);

	_SetState(ticks);
}

uu::network::DataContainer* Bomb::CreateContainer() const
{
	return NULL;
}

void Bomb::ReadFromContainer(uu::network::DataContainer const& container)
{
	Item::ReadFromContainer(container);
	
}

void Bomb::WriteToContainer(uu::network::DataContainer& container) const
{
	Item::WriteToContainer(container);
	
}

bool Bomb::_SetState(State new_state)
{
	if (_state != new_state)
	{
		if (_state != dead)
		{
			switch (new_state)
			{
			case none:
			break;
			case ticks:
				_SetAnimation("idle", true);
			break;
			case explode:
				_sprite.SetVisible(false);
			break;
			case dead:
				_sprite.SetVisible(false);
			break;
			}

			_state = new_state;
		}
		else
		{
			if (new_state == none)
			{
				_sprite.SetVisible(false);
				_state = new_state;
			}
		}
	}

	return (_state == new_state);
}

bool Bomb::Update(time_t time_now)
{
	switch (_state)
	{
	case ticks:		_RefreshTicks(time_now);	break;
	case explode:	_RefreshExplode(time_now);	break;
	case dead:		_RefreshDead(time_now);		break;
	default:									break;
	}

	return Animable::Update(time_now);
}

void Bomb::_RefreshTicks(time_t time_now)
{
	float timeToExplosion = (_explosion_time - time_now) / 1000.f;

	if (timeToExplosion > 0.f)
	{
		_label.SetText("%.02f", timeToExplosion);
	}
	else
	{
		_SetState(explode);
	}
}

void Bomb::_RefreshExplode(time_t time_now)
{
	_current_radius += 1.5f;
	if (_current_radius > _explosion_radius)
	{
		_SetState(dead);
	}
	else
	{
		std::vector<Entity*> list;
		Game::GetInstance().GetEntitiesList(list, Character::type, GetId());
		if (list.empty() == false)
		{
			sf::Vector2f position;
			for (auto it = list.begin(); it != list.end(); it++)
			{
				Character& ch = dynamic_cast<Character&>(*(*it));
				ch.GetPosition(position);
				if (IsInExplosionRange(position) == true)
				{
					if (ch.IsDead() == false)
					{
						ch.Hit(GetId(), _power);
					}
				}
			}

			list.clear();
		}
	}
}

void Bomb::_RefreshDead(time_t time_now)
{
	MarkForDelete();
}

void Bomb::Draw(sf::RenderTarget& target, bool bDebugDraw)
{
	sf::FloatRect rect = _sprite.GetGlobalBounds();

	if (_state == ticks)
	{
		// draw the sprite
		Animable::Draw(target, bDebugDraw);

		_label.SetPosition(rect.left + 3, rect.top + 10);
		_label.Draw(target);
	}
	else if (_state == explode)
	{
		// draw the explosion ellipse
		sf::Vector2f center, radius;
		GetPositionEllipse(center, radius);
		radius *= _current_radius;

		sf::Uint8 alpha = (sf::Uint8)(_current_radius * 255.f / _explosion_radius);
		sf::Color color = sf::Color(255, 255, 0, 255-alpha);

		DrawEllipse(target, center, radius, color, 0, color);
	}
}

bool Bomb::IsInExplosionRange(sf::Vector2f const& point) const
{
	if (IsDead() == true)
		return false;

	uu::Ellipse2f ellipse;
	GetPositionEllipse(ellipse);

	ellipse._radius *= _current_radius;

	return (ellipse.IsPointInside(point));
}
