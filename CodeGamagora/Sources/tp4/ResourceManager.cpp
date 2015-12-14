//**********************************************************************************************************************
#include "ResourceManager.h"
#include "StringId.h"

#include "SFML/Graphics.hpp"

//**********************************************************************************************************************
//**********************************************************************************************************************
ResourceManager::ResourceManager() : Singleton<ResourceManager>()
{
}

//**********************************************************************************************************************
ResourceManager::~ResourceManager()
{
	{
		for (auto it = _textures.begin(); it != _textures.end(); it++)
		{
			delete (it->second);
		}

		_textures.clear();
	}

	{
		for (auto it = _fonts.begin(); it != _fonts.end(); it++)
		{
			delete (it->second);
		}
		
		_fonts.clear();
	}

	{
		for (auto it = _spritesheets.begin(); it != _spritesheets.end(); it++)
		{
			delete (it->second);
		}

		_spritesheets.clear();
	}

	{
		for (auto it = _xmlfiles.begin(); it != _xmlfiles.end(); it++)
		{
			delete (it->second);
		}

		_xmlfiles.clear();
	}
}

//**********************************************************************************************************************
sf::Texture* ResourceManager::LoadTexture(const char* file)
{
	if (file == nullptr)
		return false;

	unsigned long id = uu::StringId(file).Id();
	auto it = _textures.find(id);

	if (it == _textures.end())
	{
		sf::Texture *res = new sf::Texture();

		if (res->loadFromFile(file) == false)
		{
			delete(res);
			return nullptr;
		}

		_textures[id] = res;

		return res;
	}

	return it->second;
}

//**********************************************************************************************************************
sf::Font* ResourceManager::LoadFont(const char* file)
{
	if (file == nullptr)
		return false;

	unsigned long id = uu::StringId(file).Id();
	auto it = _fonts.find(id);

	if (it == _fonts.end())
	{
		sf::Font *resource = new sf::Font();

		if (resource->loadFromFile(file) == false)
		{
			delete(resource);
			return nullptr;
		}

		_fonts[id] = resource;

		return resource;
	}

	return it->second;
}

//**********************************************************************************************************************
tinyxml2::XMLDocument* ResourceManager::LoadXmlFile(const char* file)
{
	if (file == nullptr)
		return false;

	unsigned long id = uu::StringId(file).Id();
	auto it = _xmlfiles.find(id);

	if (it == _xmlfiles.end())
	{

		tinyxml2::XMLDocument *resource = new tinyxml2::XMLDocument();

		if (resource->LoadFile(file) != tinyxml2::XML_SUCCESS)
		{
			delete(resource);
			return nullptr;
		}

		_xmlfiles[id] = resource;

		return resource;
	}

	return it->second;
}

//**********************************************************************************************************************
SpriteSheet* ResourceManager::LoadSpriteSheet(const char* name)
{
	if (name == nullptr)
		return false;

	unsigned long id = uu::StringId(name).Id();
	auto it = _spritesheets.find(id);

	if (it == _spritesheets.end())
	{
		std::string file_path = "data/";
		file_path += name;
		file_path += ".xml";

		tinyxml2::XMLDocument* xml = LoadXmlFile(file_path.c_str());
		if (xml == nullptr)
		{
			return nullptr;
		}

		SpriteSheet *resource = new SpriteSheet();

		if (resource->LoadFromXmlFile(*xml) == false)
		{
			delete(resource);
			return nullptr;
		}

		_spritesheets[id] = resource;

		return resource;
	}

	return it->second;
}
