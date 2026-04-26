#pragma once
#include <SFML/Graphics.hpp>

// Camera.h -- 2-D scrolling camera. Tracks the active player and clamps
// the viewport to world boundaries. Supports both horizontal and vertical scroll.

class Camera
{
public:
	// Smoothly move viewport toward (playerX, playerY) this frame.
	void	update(float playerX, float playerY);

	// Apply current offset to the SFML view before drawing.
	void	apply(sf::RenderWindow& window);

	// Convert world coordinates to screen coordinates using current offset.
	void	worldToScreen(float x, float y, float& outX, float& outY);

	float	getOffsetX();
	float	getOffsetY();

private:
	float	offsetX;		// current horizontal scroll
	float	offsetY;		// current vertical scroll
	float	targetX;		// desired target (lerped toward)
	float	targetY;
	int		worldWidth;
	int		worldHeight;
};
