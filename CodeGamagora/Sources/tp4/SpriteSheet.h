//**********************************************************************************************************************
#pragma once
//**********************************************************************************************************************

#include "SFML/Graphics.hpp"
#include "tinyxml2.h"

//**********************************************************************************************************************
//**********************************************************************************************************************
class SubTexture
{
public:
	SubTexture();
	SubTexture(const char* name, sf::IntRect& rect, sf::Vector2f& origin, sf::IntRect& original, sf::Texture* texture);

	std::string name;		// name of the sprite
	sf::IntRect rect;		// sprite pos in texture
	sf::Vector2f origin;	// sprite's corner offset
	sf::IntRect original;	// sprite's original size
	sf::Texture* texture;
};

//**********************************************************************************************************************
class SpriteSheet
{
public:
	SpriteSheet();
	~SpriteSheet();

	bool LoadFromXmlFile(tinyxml2::XMLDocument& document);

	std::string name;
	sf::Texture* texture;

	typedef std::vector<SubTexture*> textures_t;
	textures_t sprites;
};

//**********************************************************************************************************************
