//**********************************************************************************************************************
#pragma once
//**********************************************************************************************************************

#include "SFML/Graphics.hpp"

//**********************************************************************************************************************

class Widget
{
	friend class Game;

public:

	Widget();
	virtual ~Widget();

	virtual void Draw(sf::RenderTarget& target) = 0;
	virtual bool PickTest(float x, float y) const = 0;

	void SetId(uu::u32 id) { _id = id; }
	uu::u32 GetId() const { return _id; }

	void SetFont(sf::Font* font);
	void SetPosition(float x, float y) { _position.x = x; _position.y = y; }
	void SetPosition(sf::Vector2f const& position);
	void GetPosition(sf::Vector2f& position) const;

protected:
	uu::u32 _id;
	sf::Font* _font;
	sf::Vector2f _position;
};

//**********************************************************************************************************************
class Label: public Widget
{
public:

	Label();
	virtual ~Label();

	virtual void Draw(sf::RenderTarget& target);
	virtual bool PickTest(float x, float y) const;

	void SetText(const char* format, ...);
	void SetFontSize(sf::Uint32 font_size);
	void SetFontColor(sf::Color const& color);
	void SetBorder(float thickness, sf::Color const& color);
	void SetStyle(sf::Uint32 style);
	void SetStyle(sf::Font* font, sf::Uint32 font_size, sf::Uint32 style, sf::Color const& text_color, float border_thickness = 0.f, sf::Color const& border_color = sf::Color::Transparent);

	void Enable() { _enabled = true; }
	void Disable() { _enabled = false; }

protected:
	bool _enabled;
	sf::Uint32 _style;
	sf::Uint32 _font_size;
	sf::Color _text_color;
	sf::Color _border_color;
	sf::Text _text;
	float _border_thickness;
};

//**********************************************************************************************************************
class Button: public Label
{
public:

	Button();
	virtual ~Button();

	virtual void Draw(sf::RenderTarget& target);

protected:

};
