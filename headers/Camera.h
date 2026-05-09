#pragma once
#include <SFML/Graphics.hpp>

/**
 * Camera.h -- 2-D scrolling camera.
 * Tracks the player with lerp smoothing and supports screen shake effects.
 */

class Camera
{
public:
	Camera();

	void	update(float playerX, float playerY, float dt);
	void	shake(float magnitude, float duration);

	float	getOffsetX() const;
	float	getOffsetY() const;

private:
	float	offsetX;
	float	offsetY;

	float	shakeTimer;
	float	shakeMagnitude;
};
