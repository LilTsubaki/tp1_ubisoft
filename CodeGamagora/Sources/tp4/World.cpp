//**********************************************************************************************************************
#include "Game.h"
#include "World.h"

//**********************************************************************************************************************
//**********************************************************************************************************************

//**********************************************************************************************************************
World::World(): Singleton<World>(),
	_ground(nullptr)
{
}

//**********************************************************************************************************************
World::~World()
{
	if (_ground != nullptr)
	{
		delete _ground;
		_ground = nullptr;
	}
}

//**********************************************************************************************************************
bool World::Initialize()
{
	sf::IntRect area_size;

	area_size.width = Configuration::GetInstance()._area_width;
	area_size.height = Configuration::GetInstance()._area_height;

	// draw ground into rendertexture
	sf::Sprite tmp_sprite;

	_ground = new sf::RenderTexture();
	if (_ground != nullptr)
	{
		_ground->create(area_size.width, area_size.height);

		sf::Texture* background_texture = ResourceManager::GetInstance().LoadTexture("data/background.png");
		if (background_texture != nullptr)
		{
			background_texture->setRepeated(true);
			tmp_sprite.setTexture(*background_texture);
			tmp_sprite.setTextureRect(area_size);
			_ground->draw(tmp_sprite);
		}

		// render ground texture
		_ground->display();
		_background.setTexture(_ground->getTexture());
	}

	return true;
}

//**********************************************************************************************************************
bool World::Update()
{
	return true;
}

//**********************************************************************************************************************
void World::Draw(sf::RenderTarget& target)
{
	target.draw(_background);
}

//**********************************************************************************************************************
bool World::Terminate()
{
	if (_ground != nullptr)
	{
		delete _ground;
		_ground = nullptr;
	}

	return true;
}
