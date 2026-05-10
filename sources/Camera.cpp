#include "../headers/Camera.h"
#include "../headers/Constants.h"
#include <cmath>
#include <cstdlib>

Camera::Camera() : offsetX(0.0f), offsetY(0.0f), shakeTimer(0.0f), shakeMagnitude(0.0f)
{
}

void Camera::update(float playerX, float playerY, float dt)
{
	// 1. Tighten margins (16px instead of 64px)
	float margin = 16.0f;
	
	// 2. Calculate current view bounds
	float viewRight = offsetX + SCREEN_W - margin;
	float viewLeft  = offsetX + margin;

	// 3. Determine target horizontal scroll
	// Metal Slug Style: Keep player slightly left of center (40% from left)
	float idealTargetX = playerX - (SCREEN_W * 0.4f);

	// 4. Update Target only if player hits margins
	if (playerX > viewRight) {
		offsetX += (playerX - viewRight) * 5.0f * dt; // Catch up
	} 
	else if (playerX < viewLeft) {
		offsetX -= (viewLeft - playerX) * 5.0f * dt; // Retreat
	}

	// 5. Smoothly transition toward the ideal framing (Lerp)
	float diffX = idealTargetX - offsetX;
	offsetX += diffX * 2.0f * dt; 

	// 6. Minimal/Stable Vertical Movement
	float targetY = playerY - SCREEN_H * 0.6f; // Keep ground in view
	offsetY += (targetY - offsetY) * 2.0f * dt;

	// 7. Maximum Bounds Clamping
	float maxOffsetX = (float)(LEVEL_WIDTH  * CELL_SIZE) - SCREEN_W;
	float maxOffsetY = (float)(LEVEL_HEIGHT * CELL_SIZE) - SCREEN_H;

	if (offsetX < 0.0f) offsetX = 0.0f;
	if (offsetY < 0.0f) offsetY = 0.0f;
	if (offsetX > maxOffsetX) offsetX = maxOffsetX;
	if (offsetY > maxOffsetY) offsetY = maxOffsetY;

	// Guard against negative max (when level is smaller than screen)
	if (maxOffsetX < 0.0f) offsetX = 0.0f;
	if (maxOffsetY < 0.0f) offsetY = 0.0f;

	// 8. SCREEN SHAKE
	if (shakeTimer > 0.0f) {
		float angle = (float)(std::rand() % 628) * 0.01f;
		offsetX += std::cos(angle) * shakeMagnitude;
		offsetY += std::sin(angle) * shakeMagnitude;
		shakeTimer -= dt;
		shakeMagnitude *= 0.85f;
	}
}

void Camera::shake(float magnitude, float duration)
{
	shakeMagnitude = magnitude;
	shakeTimer = duration;
}

float Camera::getOffsetX() const { return offsetX; }
float Camera::getOffsetY() const { return offsetY; }
