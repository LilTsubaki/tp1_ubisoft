//**********************************************************************************************************************
#pragma once
//**********************************************************************************************************************

#include "SFML/Graphics.hpp"
#include "SpriteSheet.h"

//**********************************************************************************************************************
//**********************************************************************************************************************

class Sprite
{
public:
	Sprite(const char* name);
	virtual ~Sprite();

	virtual bool Init();
	virtual bool Refresh();
	virtual bool Term();

	void SetVisible(bool visible);
	bool IsVisible() const { return _bVisible; }
	virtual bool IsAnimated() const { return false; }

	void SetScale(sf::Vector2f const& scale);
	void SetPosition(sf::Vector2f const& position);
	sf::FloatRect GetGlobalBounds() const;

	std::string name;
	sf::Texture* texture;
	sf::Sprite sprite;

protected:
	sf::Vector2f _scale;
	bool _bVisible;
};

//**********************************************************************************************************************
class AnimatedSprite: public Sprite
{
public:

	class AnimationFrames
	{
	public:
		AnimationFrames();
		virtual ~AnimationFrames();

		SubTexture* AddFrame(SubTexture* frame);

		typedef std::vector<SubTexture*> frames_t;
		std::vector<SubTexture*> frames;

		bool Init(AnimatedSprite& sprite, bool loop);
		bool Refresh(AnimatedSprite& sprite);

		SubTexture* current_frame;
		unsigned int current_frame_index;
		sf::Int32 duration;
		sf::Clock clock;
		bool bLoop;
		bool bAnimationEnded;

protected:
		void _SetCurrentFrame(AnimatedSprite& sprite);
	};

	AnimatedSprite(const char* name);
	virtual ~AnimatedSprite();

	bool Init();
	bool Refresh();
	void Draw(sf::RenderTarget& target);
	bool IsAnimated() const { return _animations.size() > 0; }

	bool IsAnimationEnded() const;

	AnimationFrames* AddAnimation(const char* name);
	AnimationFrames* SetAnimation(const char* name, bool loop);

protected:
	typedef std::map<std::string, AnimationFrames*> animations_t;
	std::map<std::string, AnimationFrames*> _animations;

protected:
	bool _InitAllAnimationsFromSpriteSheet();
	bool _InitAnimationFromSpriteSheet(const char* file);

	AnimationFrames* _current_animation;
};

//**********************************************************************************************************************
