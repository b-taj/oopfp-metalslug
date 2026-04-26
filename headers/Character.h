#pragma once
#include <SFML/Graphics.hpp>
#include "Enums.h"
#include "Weapon.h"
#include "Grenade.h"

class Vehicle;
class Enemy;

// Character.h -- Abstract base for all four playable characters + FusionCompanion.
// Sub-classes override the pure-virtual methods to apply character-specific
// stat modifiers, special powers, and per-frame update logic.
// Switching between characters (key Z) is managed by Game::switchCharacter().

class Character
{
public:
	virtual ~Character() = default;

	// ── pure virtual -- implemented by each character sub-class ──────────
	virtual void	update(float dt) = 0;
	virtual void	activateSpecialPower() = 0;
	virtual float	getFireRate() = 0;		// shots/s including character buffs
	virtual int		getGrenadeDamage() = 0;		// final grenade HP value

	// ── movement & actions ───────────────────────────────────────────────
	void			moveLeft();
	void			moveRight();
	void			jump();
	void			shoot(float angle);		// calls currentWeapon->fire()
	void			throwGrenade(float angle);
	void			meleeAttack();
	void			pickupWeapon(Weapon* weapon);
	void			boardVehicle(Vehicle* vehicle);	// sets currentVehicle (aggregation)
	void			exitVehicle();

	// ── health & state ───────────────────────────────────────────────────
	void			takeDamage();		// advances damageLevel; triggers CharacterState transitions
	void			die();
	void			respawn();			// restore hp; lose one life; reset position
	void			applyState(CharacterState newState);

	// ── utility ──────────────────────────────────────────────────────────
	float			getMeleeRange();
	void			draw(sf::RenderWindow& window);
	sf::FloatRect	getBounds();

protected:
	int				hp;
	int				maxHp;
	float			x;
	float			y;
	float			velocityX;
	float			velocityY;
	float			speed;
	int				lives;				// 2 per character; 0 -> cannot switch back
	int				grenadeCount;			// base = 10; varies per character
	Weapon*			currentWeapon;			// equipped special weapon (nullable)
	Pistol			pistol;				// always available; infinite ammo
	Vehicle*		currentVehicle;			// null when on foot (aggregated, not owned)
	CharacterState	state;
	float			stateTimer;				// counts down UNDEAD/MUMMY duration
	int				damageLevel;			// 0=fine | 1=injured | 2=critical
	sf::Sprite		sprite;
	sf::Texture		texture;
	bool			facingRight;
	bool			isJumping;
};

// Pure infantry fighter. +25% fire rate, piercing melee, -2 grenades.
// Special power: fire simultaneously in both directions for 10 seconds.
class Marco : public Character
{
public:
	void	update(float dt) override;
	void	activateSpecialPower() override;	// enables dualFireActive for 10 s
	float	getFireRate() override;			// base x 1.25
	int		getGrenadeDamage() override;		// base - 2 grenades
	void	meleeAttack();					// overridden: pierces shielded opponents

private:
	bool	dualFireActive;
	float	dualFireTimer;
	bool	canPierceShield;
};

// Mediocre on foot, powerful in vehicles. -20% foot speed/HP.
// Special power: full immunity (self + vehicle) for 20 seconds.
class Tarma : public Character
{
public:
	void	update(float dt) override;
	void	activateSpecialPower() override;	// enables immunityActive for 20 s
	float	getFireRate() override;
	int		getGrenadeDamage() override;
	float	getVehicleFireRate();			// vehicle fire rate x 1.25
	float	getVehicleDurability();			// vehicle max HP x 1.20
	void	onVehicleDestroyed();			// Tarma survives vehicle destruction

private:
	bool	immunityActive;
	float	immunityTimer;
};

// Grenade specialist. Double grenades, +50% blast radius, no melee, -20% fire rate.
// Special power: 2 grenades for cost of 1 for 10 s (second grenade lands 2 blocks further).
class Eri : public Character
{
public:
	void	update(float dt) override;
	void	activateSpecialPower() override;	// enables doubleGrenadeActive for 10 s
	void	throwGrenade(float angle);			// overridden: throws 2 when power is active
	float	getFireRate() override;			// base x 0.80
	int		getGrenadeDamage() override;		// blastRadius x 1.50

private:
	bool	doubleGrenadeActive;
	float	doubleGrenadeTimer;
	float	blastRadius;		// effective radius (base x 1.5)
};

// Weapon master. +50% ammo on pickup, +10% fire rate, -25% melee, -2 grenades.
// Special power: +100% fire rate (SUPERCHARGED) for 10 seconds.
class Fio : public Character
{
public:
	void	update(float dt) override;
	void	activateSpecialPower() override;	// enables superchargeActive for 10 s
	void	pickupWeapon(Weapon* weapon);		// overridden: grants +50% ammo
	float	getFireRate() override;			// base x 1.10 (x 2.10 while supercharged)
	int		getGrenadeDamage() override;		// -2 from base count
	float	getMeleeRange();				// overridden: -25% melee damage

private:
	bool	superchargeActive;
	float	superchargeTimer;
};

// AI-controlled companion; spawned once per level (30 s survival / 3-min cooldown in campaign).
// Stats are averaged from 2-4 fused characters; all buffs and weaknesses are inherited.
class FusionCompanion : public Character
{
public:
	void	update(float dt) override;				// drives followAndAttack() each frame
	void	activateSpecialPower() override;
	float	getFireRate() override;				// averaged across fusedChars
	int		getGrenadeDamage() override;
	void	setFusedCharacters(std::vector<Character*> chars);	// call before spawning

private:
	std::vector<Character*>	fusedChars;
	bool					isImmortal;
	float					activeTimer;		// seconds until despawn
	Enemy*					target;			// current attack target (aggregated)

	void	averageStats();		// recompute speed/hp/fireRate from fusedChars
	void	followAndAttack();		// pursuit-and-fire AI tick
};
