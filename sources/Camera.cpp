#include "../headers/Camera.h"

void Camera::update(float playerX, float playerY)
{
    // targetX and targetY aren't strictly needed for basic follow, 
    // but we can use them for smooth lerping later.
    targetX = playerX;
    targetY = playerY;

    // Keep player centered after they pass the middle of the screen (800px)
    if (playerX > 800.0f) {
        offsetX = playerX - 800.0f;
    } else {
        offsetX = 0.0f;
    }

    // Vertical scroll (optional, keeping at 0 for now unless player goes very high)
    if (playerY < 300.0f) {
        offsetY = playerY - 300.0f;
    } else {
        offsetY = 0.0f;
    }
}

void Camera::apply(sf::RenderWindow& window)
{
    // Since sf::View is not allowed, this method is a stub.
    // Coordinate translation happens manually via getOffsetX().
    (void)window;
}

void Camera::worldToScreen(float worldX, float worldY, float& outX, float& outY)
{
    outX = worldX - offsetX;
    outY = worldY - offsetY;
}

float Camera::getOffsetX() { return offsetX; }
float Camera::getOffsetY() { return offsetY; }
