//**********************************************************************************************************************
#include "types.h"
#include "tinyxml2.h"

#include "SpriteSheet.h"
#include "ResourceManager.h"
#include "Game.h"

//**********************************************************************************************************************
//**********************************************************************************************************************
SubTexture::SubTexture() : 
	texture(nullptr)
{
}

//**********************************************************************************************************************
SubTexture::SubTexture(const char* name, sf::IntRect& rect, sf::Vector2f& origin, sf::IntRect& original, sf::Texture* texture) :
	name(name),
	rect(rect), 
	origin(origin), 
	original(original), 
	texture(texture)
{
}
//**********************************************************************************************************************
SpriteSheet::SpriteSheet() :
	texture(nullptr)
{
}

//**********************************************************************************************************************
SpriteSheet::~SpriteSheet()
{
	for(auto it = sprites.begin(); it != sprites.end(); ++it)
	{
		delete(*it);
	}

	sprites.clear();
}

//**********************************************************************************************************************
bool SpriteSheet::LoadFromXmlFile(tinyxml2::XMLDocument& document )
{
	ResourceManager& resourceManager = ResourceManager::GetInstance();

	tinyxml2::XMLElement* atlas = document.FirstChildElement("TextureAtlas");
	while (atlas != nullptr)
	{
		name = atlas->Attribute("imagePath");
		if (name.empty() == false)
		{
			std::string file_path = "data/";
			file_path += name;

			texture = resourceManager.LoadTexture(file_path.c_str());
			if (texture != nullptr)
			{
				const char* subName;
				sf::IntRect rect;
				sf::Vector2f origin;
				sf::IntRect original;

				tinyxml2::XMLElement* element = atlas->FirstChildElement("SubTexture");
				while (element != nullptr)
				{
					subName = element->Attribute("name");

					rect.top = rect.left = rect.width = rect.height = 0;
					element->QueryAttribute("x", &rect.left);
					element->QueryAttribute("y", &rect.top);
					element->QueryAttribute("width", &rect.width);
					element->QueryAttribute("height", &rect.height);

					origin.x = origin.y = 0.0f;
					original = rect;
					element->QueryAttribute("frameX", &origin.x);
					element->QueryAttribute("frameY", &origin.y);
					element->QueryAttribute("frameWidth", &original.width);
					element->QueryAttribute("frameHeight", &original.height);

					SubTexture* subTexture = new SubTexture(subName, rect, origin, original, texture);
					sprites.push_back(subTexture);

					element = element->NextSiblingElement("SubTexture");
				}
			}
		}

		atlas = atlas->NextSiblingElement("TextureAtlas");
	}

	return true;
}

//**********************************************************************************************************************
