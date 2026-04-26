#pragma once
#include <SFML/Graphics.hpp>
#include "Enums.h"

class Character;
class Enemy;
class Projectile;

// Vehicle.h -- Abstract player vehicle base + four concrete player vehicles.
//              Also contains EnemyVehicle abstract base + three enemy vehicle sub-classes.

// =========================================================================
//  PLAYER VEHICLES
// =========================================================================

// Abstract base. takeDamage() uses a 3-hit model; warns driver before destruction.
class Vehicle
{
public:
	virtual ~Vehicle() = default;

	virtual void		update(float dt) = 0;
	virtual Projectile*	fire() = 0;		// main weapon varies by vehicle

	void			takeDamage(int dmg);
	void			dropBomb();		// throw a grenade from open hatch
	void			ejectDriver();		// force-remove driver before explosion
	void			enter(Character* c);	// set driver reference (aggregation)
	void			draw(sf::RenderWindow& window);
	sf::FloatRect	getBounds();

protected:
	int			hp;
	int			maxHp;
	float		x;
	float		y;
	float		velX;
	float		velY;
	int			bombAmmo;		// grenades available for dropBomb()
	Character*	driver;			// aggregated -- character exists independently
	sf::Sprite	sprite;
	sf::Texture	texture;
	bool		isDestroyed;
};

// Main tank. Twin Vulcan cannons (unlimited) + limited bombs.
// 360-degree tilt; traverse 1-block height differences; crouch opens hatch.
class MetalSlug : public Vehicle
{
public:
	void		update(float dt) override;
	Projectile*	fire() override;			// fires twin Vulcan tracers
	Projectile*	fireCannon();			// fires a bomb (uses bombAmmo)
	void		crouch();				// opens hatch for grenade throw
	void		suicideAttack(Enemy* target);	// driver ejects; tank rams target

private:
	int		cannonAmmo;
	float	tiltAngle;		// 0-360 degrees
	bool	canCrouch;
};

// Helicopter. Bullets at half the slug's speed. 4 built-in missiles.
class SlugFlyer : public Vehicle
{
public:
	void		update(float dt) override;
	Projectile*	fire() override;
	Projectile*	fireMissile();

private:
	int	missiles;		// 4 total
};

// Submarine. 3 missiles in each of three directions.
class SlugMariner : public Vehicle
{
public:
	void		update(float dt) override;
	Projectile*	fire() override;		// defaults to horizontal salvo
	Projectile*	fireMissile(MissileType type);	// explicit direction

private:
	int	horizontalMissiles;		// 3
	int	verticalMissiles;		// 3
	int	reverseMissiles;		// 3
};

// Transitions automatically between MetalSlug (ground), SlugFlyer (air), SlugMariner (water).
class AmphibiousSlug : public Vehicle
{
public:
	void		update(float dt) override;	// calls switchForm() on terrain change
	Projectile*	fire() override;			// delegates to the active form
	void		onEnterWater();
	void		onExitWater();
	void		onGround();

private:
	MetalSlug*	groundSlug;		// owned sub-vehicle components
	SlugFlyer*	flyer;
	SlugMariner*	mariner;
	VehicleForm	currentForm;

	void	switchForm(VehicleForm form);
};

// =========================================================================
//  ENEMY VEHICLES
// =========================================================================

class EnemyVehicle
{
public:
	virtual ~EnemyVehicle() = default;

	virtual void	update(float dt) = 0;
	virtual void	attack(Character* target) = 0;

	void			takeDamage(int dmg);
	void			die();
	void			draw(sf::RenderWindow& window);
	sf::FloatRect	getBounds();

protected:
	int			hp;
	float		x;
	float		y;
	int			scoreValue;
	sf::Sprite	sprite;
	sf::Texture	texture;
};

// Hovers at medium height; drops grenades when directly above the player. Batches of 2.
class FlyingTara : public EnemyVehicle
{
public:
	void	update(float dt) override;
	void	attack(Character* target) override;

private:
	float	altitude;
	float	dropTimer;
};

// Long-range artillery. 7 HP. Arc missiles; can advance horizontally on flat ground.
class M15ABradley : public EnemyVehicle
{
public:
	void	update(float dt) override;
	void	attack(Character* target) override;
	void	moveToward(Character* target);

private:
	bool	isStationary;
	float	missileTimer;
};

// Aquatic biome sub. 7 HP. Fires rockets toward the player.
class EnemySub : public EnemyVehicle
{
public:
	void	update(float dt) override;
	void	attack(Character* target) override;

private:
	float	rocketTimer;
	bool	isUnderwater;
};
