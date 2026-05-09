#pragma once
#include <SFML/Graphics.hpp>

class Projectile;

class Weapon
{
public:
	virtual ~Weapon() = default;

	virtual Projectile*	fire(float x, float y, float angle) = 0;

	bool	canFire();
	void			reload(int amount);
	int				getAmmo();
	int				getDamage();
	void			update(float dt);

protected:
	int		ammo;
	bool	infiniteAmmo;
	float	fireRate;
	int		damage;
	float	fireTimer;
};

class Pistol : public Weapon
{
public:
	Projectile*	fire(float x, float y, float angle) override;
};

class HeavyMachineGun : public Weapon
{
public:
	Projectile*	fire(float x, float y, float angle) override;
};

class RocketLauncher : public Weapon
{
public:
	Projectile*	fire(float x, float y, float angle) override;

private:
	float	reloadCooldown;
	float	blastRadius;
};

class FlameShot : public Weapon
{
public:
	Projectile*	fire(float x, float y, float angle) override;

private:
	float	range;
	float	burnDmgPerSec;
};

class LaserGun : public Weapon
{
public:
	Projectile*	fire(float x, float y, float angle) override;
};