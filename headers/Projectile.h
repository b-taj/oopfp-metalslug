#pragma once
#include <SFML/Graphics.hpp>
#include "Enums.h"

// Projectile.h -- A single in-flight projectile produced by Weapon::fire()
// or spawned by a Grenade::explode(). Removed from the active list when isAlive=false.

class Projectile
{
public:
	// Advance position; deactivate if boundary is reached.
	void			update(float dt);

	void			draw(sf::RenderWindow& window);

	// Called by collision detection when this projectile hits a target.
	void			onHit();
	void			loadTexture(const char* filename);
	void			setTextureRect(int left, int top, int width, int height);
	void			setPosition(float nx, float ny);

	// ── data ─────────────────────────────────────────────────────────────
	float			x;
	float			y;
	float			velX;			// pixels per second
	float			velY;
	int			damage;
	ProjectileType	type;
	bool			isAlive;

private:
	sf::Sprite		sprite;
	sf::Texture		texture;
};
