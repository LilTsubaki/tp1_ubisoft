//**********************************************************************************************************************
#include "types.h"
#include "tinyxml2.h"

#include "AnimatedSprite.h"
#include "ResourceManager.h"
#include "Game.h"
#include "Log.h"

//**********************************************************************************************************************
//**********************************************************************************************************************
Sprite::Sprite(const char* name):
	name(name),
	texture(nullptr),
	_scale(1.f, 1.f),
	_bVisible(true)
{
}

//**********************************************************************************************************************
Sprite::~Sprite()
{
}

//**********************************************************************************************************************
void Sprite::SetVisible(bool visible)
{
	_bVisible = visible;
}

//**********************************************************************************************************************
void Sprite::SetScale(sf::Vector2f const& scale)
{
	_scale = scale;
}

//**********************************************************************************************************************
void Sprite::SetPosition(sf::Vector2f const& position)
{
	sprite.setPosition(position);
}

//**********************************************************************************************************************
sf::FloatRect Sprite::GetGlobalBounds() const
{
	return sprite.getGlobalBounds();
}

//**********************************************************************************************************************
bool Sprite::Init()
{
	return true;
}

//**********************************************************************************************************************
bool Sprite::Refresh()
{
	return true;
}

//**********************************************************************************************************************
bool Sprite::Term()
{
	return true;
}

//**********************************************************************************************************************
AnimatedSprite::AnimationFrames::AnimationFrames():
	current_frame(nullptr),
	current_frame_index(0),
	duration(200),
	bLoop(false),
	bAnimationEnded(false)
{
}

//**********************************************************************************************************************
AnimatedSprite::AnimationFrames::~AnimationFrames()
{
	frames.clear();
}

//**********************************************************************************************************************
SubTexture* AnimatedSprite::AnimationFrames::AddFrame(SubTexture* frame)
{
	frames.push_back(frame);

	return frame;
}

//**********************************************************************************************************************
void AnimatedSprite::AnimationFrames::_SetCurrentFrame(AnimatedSprite& sprite)
{
	bAnimationEnded = false;

	if (current_frame_index >= frames.size())
	{
		bAnimationEnded = true;

		if (bLoop == false)
			current_frame_index = (unsigned int)(frames.size()-1);
		else
			current_frame_index %= frames.size();
	}

	current_frame = frames[current_frame_index];

	sprite.sprite.setTexture((const sf::Texture&)*(current_frame->texture));

	sprite.sprite.setOrigin(current_frame->origin);
	sprite.sprite.setTextureRect(current_frame->rect);
	sprite.sprite.setOrigin((float)(current_frame->rect.width * .5f), (float)current_frame->rect.height);
	sprite.sprite.setScale(sprite._scale);
}

//**********************************************************************************************************************
bool AnimatedSprite::AnimationFrames::Init(AnimatedSprite& sprite, bool loop)
{
	bLoop = loop;
	current_frame_index = 0;
	_SetCurrentFrame(sprite);

	return true;
}

//**********************************************************************************************************************
bool AnimatedSprite::AnimationFrames::Refresh(AnimatedSprite& sprite)
{
	sf::Time elapsedTime = clock.getElapsedTime();

	if (elapsedTime.asMilliseconds() >= duration)
	{
		current_frame_index++;
		_SetCurrentFrame(sprite);

		clock.restart();
	}

	return true;
}

//**********************************************************************************************************************
AnimatedSprite::AnimatedSprite(const char* name):
	Sprite(name),
	_current_animation(nullptr)
{
}

//**********************************************************************************************************************
AnimatedSprite::~AnimatedSprite()
{
	for (auto it = _animations.begin(); it != _animations.end(); it++)
	{
		delete(it->second);
	}

	_animations.clear();
}

//**********************************************************************************************************************
bool AnimatedSprite::IsAnimationEnded() const
{
	if (_current_animation == nullptr)
		return false;

	return _current_animation->bAnimationEnded;
}

//**********************************************************************************************************************
bool AnimatedSprite::Init()
{
	return _InitAllAnimationsFromSpriteSheet();
}

//**********************************************************************************************************************
bool AnimatedSprite::_InitAnimationFromSpriteSheet(const char* file)
{
	SpriteSheet* spritesheet = ResourceManager::GetInstance().LoadSpriteSheet(file);
	if (spritesheet == nullptr)
		return false;

	char animName[256];
	for (auto it = spritesheet->sprites.begin(); it != spritesheet->sprites.end(); it++)
	{
		SubTexture& frame = *(*it);

		strcpy_s(animName, frame.name.c_str());
		char* pt = (strchr(animName, '/'));
		if (pt)
			*pt = 0;

		AnimationFrames* animation = AddAnimation(animName);
		if (animation != nullptr)
		{
			animation->AddFrame(&frame);
		}
	}

	return true;
}

//**********************************************************************************************************************
bool AnimatedSprite::_InitAllAnimationsFromSpriteSheet()
{
	return _InitAnimationFromSpriteSheet(name.c_str());
}

//**********************************************************************************************************************
bool AnimatedSprite::Refresh()
{
	if (_current_animation != nullptr)
	{
		_current_animation->Refresh(*this);
	}

	return true;
}

//**********************************************************************************************************************
void AnimatedSprite::Draw(sf::RenderTarget& target)
{
	if (IsVisible() == false)
		return;

	target.draw(sprite);
}

//**********************************************************************************************************************
AnimatedSprite::AnimationFrames* AnimatedSprite::SetAnimation(const char* name, bool loop)
{
	auto it = _animations.find(std::string(name));
	if (it == _animations.end())
	{
		printf("Error: %s.SetAnimation(\"%s\")\n", this->name.c_str(), name);

		return nullptr;
	}

	if (_current_animation != it->second)
	{
		_current_animation = it->second;
		_current_animation->Init(*this, loop);
	}

	return _current_animation;
}

//**********************************************************************************************************************
AnimatedSprite::AnimationFrames* AnimatedSprite::AddAnimation(const char* name)
{
	AnimationFrames* animation;

	auto it = _animations.find(std::string(name));
	if (it == _animations.end())
	{
		//printf("%s.AddAnimation(\"%s\")\n", this->name.c_str(), name);

		animation = new AnimationFrames();

		_animations[std::string(name)] = animation;
	}
	else
	{
		animation = it->second;
	}

	return animation;
}

//**********************************************************************************************************************
