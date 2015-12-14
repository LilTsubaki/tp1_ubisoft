//**********************************************************************************************************************
#include "types.h"
#include "draw_ui.h"
#include "draw_utils.h"
#include "ResourceManager.h"
#include <stdarg.h>

//**********************************************************************************************************************
//**********************************************************************************************************************
Widget::Widget():
	_id(0),
	_font(nullptr)
{
	_font = ResourceManager::GetInstance().LoadFont("data/arial.ttf");
}

//**********************************************************************************************************************
Widget::~Widget()
{
}

//**********************************************************************************************************************
void Widget::SetFont(sf::Font* font)
{
	_font = font;
}

//**********************************************************************************************************************
//**********************************************************************************************************************
Label::Label(): Widget(),
	_enabled(false),
	_style(sf::Text::Style::Regular),
	_font_size(10),
	_text_color(sf::Color::White),
	_border_color(sf::Color::Transparent),
	_border_thickness(0)
{
	if (_font != nullptr)
		_text.setFont(*_font);
}

//**********************************************************************************************************************
Label::~Label()
{
}

//**********************************************************************************************************************
void Label::Draw(sf::RenderTarget& target)
{
	_text.setPosition(_position);
	target.draw(_text);
}

//**********************************************************************************************************************
void Label::SetFontSize(sf::Uint32 font_size)
{
	_font_size = font_size;
	_text.setCharacterSize(_font_size);
}

//**********************************************************************************************************************
void Label::SetFontColor(sf::Color const& color)
{
	_text_color = color;
	_text.setColor(_text_color);
}

//**********************************************************************************************************************
void Label::SetBorder(float thickness, sf::Color const& color)
{
	_border_thickness = thickness;
	_border_color = color;
}

//**********************************************************************************************************************
void Label::SetStyle(sf::Uint32 style)
{
	_style = style;
	_text.setStyle(_style);
}

//**********************************************************************************************************************
void Label::SetStyle(sf::Font* font, sf::Uint32 font_size, sf::Uint32 style, sf::Color const& text_color, float border_thickness, sf::Color const& border_color)
{
	_font = font;
	_font_size = font_size;
	_style = style;
	_text_color = text_color;
	_border_thickness = border_thickness;
	_border_color = border_color;

	if (_font != nullptr)
		_text.setFont(*_font);

	_text.setCharacterSize(_font_size);
	_text.setStyle(_style);
	_text.setColor(_text_color);
}

//**********************************************************************************************************************
bool Label::PickTest(float x, float y) const
{
	if (_enabled == false)
		return false;

	sf::FloatRect rect = _text.getGlobalBounds();

	return rect.contains(x, y);
}

//**********************************************************************************************************************
void Label::SetText(const char* format, ...)
{
	char str_tmp[1024];

	va_list argptr;
	va_start(argptr, format);
	vsprintf_s(str_tmp, sizeof(str_tmp), format, argptr);
	va_end(argptr);

	_text.setString(str_tmp);
}

//**********************************************************************************************************************
//**********************************************************************************************************************
Button::Button(): Label()
{
	_enabled = true;
}

//**********************************************************************************************************************
Button::~Button()
{
}

//**********************************************************************************************************************
void Button::Draw(sf::RenderTarget& target)
{
	_text.setPosition(_position);

	if (_border_thickness > 0 && _border_color != sf::Color::Transparent)
	{
		sf::FloatRect rect = _text.getGlobalBounds();

		rect.top -= 1; rect.left -= 1;
		rect.width += 2; rect.height += 2;

		DrawRect(target, rect, _border_color, _border_thickness, sf::Color::Transparent);
	}

	target.draw(_text);
}

//**********************************************************************************************************************

//**********************************************************************************************************************
