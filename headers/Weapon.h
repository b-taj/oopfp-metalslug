#pragma once
#include <SFML/Graphics.hpp>

class Projectile;

// Weapon.h -- Abstract firearm base. Sub-classes implement fire() which
// allocates and returns a Projectile aimed at the given angle.
// Only the SFML objects listed in allowed_SFML.pdf are used.

class Weapon
{
public:
	virtual ~Weapon() = default;

	// Core factory method: returns a newly created Projectile.
	virtual Projectile*	fire(float x, float y, float angle) = 0;

	// True when fireTimer has elapsed and ammo > 0 (or infiniteAmmo is set).
	bool	canFire();

	// Add amount to ammo pool (capped per weapon type).
	void	reload(int amount);

	int		getAmmo();
	int		getDamage();

	// Tick internal fireTimer toward 0 each frame.
	void	update(float dt);

protected:
	int		ammo;
	bool	infiniteAmmo;		// true for Pistol
	float	fireRate;		// shots per second
	int		damage;			// base HP removed per hit
	float	fireTimer;		// seconds until next shot is allowed
};

// Default sidearm; always available; infinite ammo; 3 HP / shot; 4 shots/s.
class Pistol : public Weapon
{
public:
	Projectile*	fire(float x, float y, float angle) override;
};

// Hold trigger to fire continuously; 2x pistol rate; 3 HP / bullet.
class HeavyMachineGun : public Weapon
{
public:
	Projectile*	fire(float x, float y, float angle) override;
};

// Single rocket per trigger; 2-second reload; 5 HP in a 3-block blast radius.
class RocketLauncher : public Weapon
{
public:
	Projectile*	fire(float x, float y, float angle) override;

private:
	float	reloadCooldown;		// 2.0 s
	float	blastRadius;		// 3 blocks
};

// Sustained flame stream up to 5 blocks; 2 HP/s burn; instantly kills mummies.
class FlameShot : public Weapon
{
public:
	Projectile*	fire(float x, float y, float angle) override;

private:
	float	range;			// 5 blocks
	float	burnDmgPerSec;		// 2.0 HP/s
};

// Fires a beam to the screen edge; instantly kills all enemies in its path.
class LaserGun : public Weapon
{
public:
	Projectile*	fire(float x, float y, float angle) override;
};
