#pragma once
#include "Enums.h"
#include "Projectile.h"

/**
 * Weapon.h -- Abstract base class for all combat equipment.
 * Manages fire rates, ammunition, and cooldown timers.
 */

class Weapon
{
public:
	virtual ~Weapon() = default;

	virtual class Projectile* fire(float x, float y, float angle, class SoundManager* sm = nullptr) = 0;
	virtual WeaponType	getType() const = 0;
...

	void				update(float dt);
	bool				canFire() const;
	bool				isOutOfAmmo() const;
	void				reload(int amount);

	int					getAmmo() const;
	int					getDamage() const;

protected:
	Weapon();

	int					ammo;
	bool				infiniteAmmo;
	float				fireRate;		// Shots per second
	float				fireTimer;		// Accumulated time
	int					damage;
};
