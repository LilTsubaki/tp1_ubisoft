//**********************************************************************************************************************
#include "types.h"

//**********************************************************************************************************************
//**********************************************************************************************************************
void DrawEllipse(sf::RenderTarget& target, sf::Vector2f const& center, sf::Vector2f const& radius, sf::Color border_color, float thickness, sf::Color fill_color)
{
	float x_radius = radius.x;
	float y_radius = radius.y;

	sf::Vector2f position = center - sf::Vector2f(radius.x, radius.y);

	sf::CircleShape ellipse;
	ellipse.setPosition(position);
	ellipse.setFillColor(fill_color);
	ellipse.setOutlineColor(border_color);
	ellipse.setOutlineThickness(thickness);

	if (x_radius > y_radius)
	{
		ellipse.setRadius(x_radius);
		ellipse.setScale(1.0f, y_radius / x_radius);
	}
	else
	{
		ellipse.setRadius(y_radius);
		ellipse.setScale(x_radius / y_radius, 1.0f);
	}

	target.draw(ellipse);
}

//**********************************************************************************************************************
void DrawLine(sf::RenderTarget& target, sf::Vector2f const& start, sf::Vector2f const& end, sf::Color color)
{
	sf::Vertex line[2];
	line[0].position = start;
	line[1].position = end;
	line[0].color = line[1].color = color;		

	target.draw(line, 2, sf::Lines);
}

//**********************************************************************************************************************
void DrawRect(sf::RenderTarget& target, sf::FloatRect const& rect, sf::Color border_color, float thickness, sf::Color fill_color)
{
	sf::RectangleShape rectangle(sf::Vector2f(rect.width, rect.height));
	rectangle.setPosition(sf::Vector2f(rect.left, rect.top));
	rectangle.setFillColor(fill_color);
	rectangle.setOutlineColor(border_color);
	rectangle.setOutlineThickness(thickness);

	target.draw(rectangle);
}

//**********************************************************************************************************************
void DrawGauge(sf::RenderTarget& target, float fill_percent, sf::FloatRect const& rect, sf::Color background_color, sf::Color fill_color)
{
	sf::Vector2f size(rect.width, rect.height);

	sf::RectangleShape rectangle(size);
	rectangle.setPosition(sf::Vector2f(rect.left, rect.top));
	rectangle.setFillColor(background_color);

	target.draw(rectangle);

	sf::FloatRect new_rect = rect;

	new_rect.top += 1;
	new_rect.left += 1;
	size.x = (new_rect.width - 2) * fill_percent / 100.f;
	size.y -= 2;

	rectangle.setPosition(sf::Vector2f(new_rect.left, new_rect.top));
	rectangle.setSize(size);
	rectangle.setFillColor(fill_color);
	target.draw(rectangle);
}
