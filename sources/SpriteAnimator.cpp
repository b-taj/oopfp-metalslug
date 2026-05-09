#include "../headers/SpriteAnimator.h"
#include <cstring>

SpriteAnimator::SpriteAnimator() 
	: animCount(0), currentAnim(-1), currentFrame(0), frameTimer(0.0f), finished(false)
{
}

void SpriteAnimator::addAnimation(const char* name, Frame* frames, int count, bool loop)
{
	if (animCount >= 16) return;

	Animation& anim = animations[animCount++];
	std::strncpy(anim.name, name, 31);
	anim.name[31] = '\0';
	anim.frameCount = (count > 32) ? 32 : count;
	anim.loop = loop;

	for (int i = 0; i < anim.frameCount; ++i) {
		anim.frames[i] = frames[i];
	}
}

void SpriteAnimator::play(const char* name)
{
	// Find animation by name
	int foundIdx = -1;
	for (int i = 0; i < animCount; ++i) {
		if (std::strcmp(animations[i].name, name) == 0) {
			foundIdx = i;
			break;
		}
	}

	if (foundIdx != -1 && foundIdx != currentAnim) {
		currentAnim = foundIdx;
		currentFrame = 0;
		frameTimer = 0.0f;
		finished = false;
	}
}

void SpriteAnimator::update(float dt)
{
	if (currentAnim == -1 || finished) return;

	const Animation& anim = animations[currentAnim];
	frameTimer += dt;

	if (frameTimer >= anim.frames[currentFrame].duration) {
		frameTimer = 0.0f;
		currentFrame++;

		if (currentFrame >= anim.frameCount) {
			if (anim.loop) {
				currentFrame = 0;
			} else {
				currentFrame = anim.frameCount - 1;
				finished = true;
			}
		}
	}
}

void SpriteAnimator::applyToSprite(sf::Sprite& sprite) const
{
	if (currentAnim != -1) {
		sprite.setTextureRect(animations[currentAnim].frames[currentFrame].rect);
	}
}

bool SpriteAnimator::isFinished() const { return finished; }

bool SpriteAnimator::getCurrentAnimationLoops() const
{
	if (currentAnim < 0) return true;
	return animations[currentAnim].loop;
}

const char* SpriteAnimator::getCurrentAnimation() const
...
	return (currentAnim != -1) ? animations[currentAnim].name : "none";
}
