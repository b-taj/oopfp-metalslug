#include "../headers/Camera.h"

// Implements a horizontal-only dead-zone camera system
// Vertical scrolling is completely disabled (Locked to screen top)
void Camera::update(float playerX, float playerY)
{
    targetX = playerX;
    targetY = playerY;

    // Define Horizontal Dead-Zone Margin (Screen is 1600x900)
    float hMargin = 500.0f; 

    // Horizontal Dead-Zone Logic
    if (playerX < offsetX + hMargin) {
        offsetX = playerX - hMargin;
    }
    else if (playerX > offsetX + (1600.0f - hMargin)) {
        offsetX = playerX - (1600.0f - hMargin);
    }

    // VERTICAL LOCK: Completely disable vertical scrolling
    offsetY = 0.0f;

    // World Bound Clamping (Horizontal only)
    if (offsetX < 0.0f) offsetX = 0.0f;
    if (offsetX > (worldWidth - 1600.0f)) offsetX = (float)(worldWidth - 1600);
}

// Configures the world boundaries in pixels for clamping
void Camera::setBounds(int w, int h)
{
    worldWidth = w;
    worldHeight = h;
}

// Applies the current camera transformation to the render window
void Camera::apply(sf::RenderWindow& window)
{
    (void)window;
}

// Maps world-space coordinates to screen-space coordinates
void Camera::worldToScreen(float worldX, float worldY, float& outX, float& outY)
{
    outX = worldX - offsetX;
    outY = worldY - offsetY;
}

// Accessor for the horizontal screen offset
float Camera::getOffsetX() { return offsetX; }

// Accessor for the vertical screen offset
float Camera::getOffsetY() { return offsetY; }
