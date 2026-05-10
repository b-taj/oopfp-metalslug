#include "../headers/Camera.h"
#include "../headers/Constants.h"
#include <cmath>
#include <cstdlib>

Camera::Camera() : offsetX(0.0f), offsetY(0.0f), shakeTimer(0.0f), shakeMagnitude(0.0f)
{
}

void Camera::update(float playerX, float playerY, float dt)
{
	float targetX = playerX - SCREEN_W * 0.5f;
	float targetY = playerY - SCREEN_H * 0.5f;
	float speed   = 8.0f;

	offsetX += (targetX - offsetX) * speed * dt;
	offsetY += (targetY - offsetY) * speed * dt;

	float maxOffsetX = (float)(LEVEL_WIDTH  * CELL_SIZE) - SCREEN_W;
	float maxOffsetY = (float)(LEVEL_HEIGHT * CELL_SIZE) - SCREEN_H;

	if (offsetX < 0.0f) offsetX = 0.0f;
	if (offsetY < 0.0f) offsetY = 0.0f;
	if (offsetX > maxOffsetX) offsetX = maxOffsetX;
	if (offsetY > maxOffsetY) offsetY = maxOffsetY;

	if (maxOffsetX < 0.0f) offsetX = 0.0f;
	if (maxOffsetY < 0.0f) offsetY = 0.0f;

	// SCREEN SHAKE
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
