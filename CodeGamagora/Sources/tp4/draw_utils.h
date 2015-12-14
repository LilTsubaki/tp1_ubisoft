//**********************************************************************************************************************
#pragma once
//**********************************************************************************************************************

#include "SFML/Graphics.hpp"

//**********************************************************************************************************************
void DrawEllipse(sf::RenderTarget& target, sf::Vector2f const& center, sf::Vector2f const& radius, sf::Color border_color, float thickness = 1, sf::Color fill_color = sf::Color::Transparent);
void DrawLine(sf::RenderTarget& target, sf::Vector2f const& start, sf::Vector2f const& end, sf::Color color);
void DrawRect(sf::RenderTarget& target, sf::FloatRect const& rect, sf::Color border_color, float thickness = 1, sf::Color fill_color = sf::Color::Transparent);
void DrawGauge(sf::RenderTarget& target, float fill_percent, sf::FloatRect const& rect, sf::Color background_color, sf::Color fill_color);
