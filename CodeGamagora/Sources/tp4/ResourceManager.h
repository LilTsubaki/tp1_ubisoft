//**********************************************************************************************************************
#pragma once

#include "types.h"
#include "Singleton.h"
#include "tinyxml2.h"
#include "SpriteSheet.h"

//**********************************************************************************************************************
namespace sf
{
	class Texture;
	class Font;
}

//**********************************************************************************************************************
class ResourceManager: public Singleton<ResourceManager>
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	sf::Texture* LoadTexture(const char* file);
	sf::Font* LoadFont(const char* file);
	tinyxml2::XMLDocument* LoadXmlFile(const char* file);

	SpriteSheet* LoadSpriteSheet(const char* name);

	typedef std::map<unsigned long, sf::Texture*> textures_t;
	typedef std::map<unsigned long, sf::Font*> fonts_t;
	typedef std::map<unsigned long, SpriteSheet*> spritesheets_t;
	typedef std::map<unsigned long, tinyxml2::XMLDocument*> xmlfiles_t;

protected:

	textures_t _textures;
	fonts_t _fonts;
	spritesheets_t _spritesheets;
	xmlfiles_t _xmlfiles;

};
