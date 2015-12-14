#pragma once

//**********************************************************************************************************************
namespace sf
{
class RenderWindow;
class RenderTexture;
}

//**********************************************************************************************************************
#include "types.h"
#include "Singleton.h"

//**********************************************************************************************************************
class World: Singleton<World>
{
public:
	World();
	virtual ~World();

	bool Initialize();
	bool Update();
	bool Terminate();
	void Draw(sf::RenderTarget& target);

protected:
	sf::RenderTexture* _ground;
	sf::Sprite _background;
};
