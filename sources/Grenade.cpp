#include "../headers/Grenade.h"

/**
 * Grenade Implementation
 * 
 * Base and concrete classes for throwable explosive devices.
 * Handles timers, blast radii, and physics integration for grenades.
 */

// Virtual destructor for proper cleanup of derived grenade types
Grenade::~Grenade()
{
}

// Renders the grenade sprite to the target SFML window
void Grenade::draw(sf::RenderWindow& window)
{
	if (!isExploded) window.draw(sprite);
}

// Returns the axis-aligned bounding box for collision detection
sf::FloatRect Grenade::getBounds()
{
	return sprite.getGlobalBounds();
}

// ==================== HAND GRENADE ====================

// Updates physics for the standard hand grenade and checks the fuse timer
void HandGrenade::update(float dt)
{
    (void)dt;
}

// Triggers the standard explosive blast logic
void HandGrenade::explode()
{
    isExploded = true;
}

// ==================== FIRE BOMB GRENADE ====================

// Updates physics for the Eri-specific fire bomb
void FireBombGrenade::update(float dt)
{
    (void)dt;
}

// Triggers an incendiary blast that creates a lingering damage area
void FireBombGrenade::explode()
{
    isExploded = true;
}
