#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

/**
 * SpriteAnimator.h -- Manages frame-by-frame animation state.
 */

class SpriteAnimator
{
public:
	struct Frame
	{
		sf::IntRect rect;
		float duration; 
	};

	struct Animation
	{
		char name[32];
		Frame frames[32];
		int frameCount;
		bool loop;
	};

	SpriteAnimator();

	void addAnimation(const char* name, Frame* frames, int count, bool loop);
	void play(const char* name);
	void update(float dt);
	void applyToSprite(sf::Sprite& sprite) const;

	void setScale(float sx, float sy) { scaleX = sx; scaleY = sy; }
	float getScaleX() const { return scaleX; }
	float getScaleY() const { return scaleY; }

	bool isFinished() const;
	bool getCurrentAnimationLoops() const;
	const char* getCurrentAnimation() const;

private:
	Animation animations[16];
	int animCount;
	int currentAnim; 
	int currentFrame;
	float frameTimer;
	bool finished;
	float scaleX, scaleY;
};
