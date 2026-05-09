#pragma once
#include "Weapon.h"
#include <SFML/Graphics/Texture.hpp>

/**
 * ProjectileWeapon.h -- Intermediate class for weapons that spawn physical projectiles.
 * Manages the texture for the bullets it fires.
 */

class ProjectileWeapon : public Weapon
{
public:
	virtual ~ProjectileWeapon() = default;
	void loadBulletTexture(const char* path);

protected:
	sf::Texture bulletTex;
};
