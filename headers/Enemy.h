#pragma once
#include <SFML/Graphics.hpp>
#include "Weapon.h"

class Character;

// Enemy.h -- Abstract base for all hostile units. Sub-classes define movement
// patterns, weapons, and special mechanics (shields, resurrection, etc.).
// The Grudge system allows surviving enemies to return as enhanced variants.

class Enemy
{
public:
	virtual ~Enemy() = default;

	virtual void	update(float dt) = 0;
	virtual void	takeDamage(int dmg) = 0;
	virtual void	attack(Character* target) = 0;
	virtual void	die() = 0;

	// Flag this enemy as a grudge variant and apply power multipliers.
	void			activateGrudge();

	int				getScore();
	void			draw(sf::RenderWindow& window);
	float			getX() { return x; }
	float			getY() { return y; }
	void			loadTexture(const char* filename);
	void			setPosition(float nx, float ny);
	bool			getIsAlive() { return isAlive; }

protected:
	int			hp;
	float		x;
	float		y;
	float		speed;
	int			scoreValue;		// base score awarded on kill
	sf::Sprite	sprite;
	sf::Texture	texture;
	bool		isAlive;
	bool		isGrudge;		// true when returning as enhanced variant
	float		grudgePowerMult;	// damage/speed multiplier in grudge form
	bool		facingRight;
};

// 2 HP; pistol; batches of 2-4; score: 50.
class RebelSoldier : public Enemy
{
public:
	RebelSoldier(float sx, float sy);
	void	update(float dt) override;
	void	takeDamage(int dmg) override;
	void	attack(Character* target) override;
	void	die() override;

private:
	Weapon*	pistol;
	float	fireTimer;
};

// 5 HP; riot shield blocks frontal bullets; hit from above/behind or with explosives.
// Batches of 1-2; score: 75.
class ShieldedSoldier : public Enemy
{
public:
	void	update(float dt) override;
	void	takeDamage(int dmg) override;
	void	takeDamageFromAbove(int dmg);	// bypasses the shield
	void	attack(Character* target) override;
	void	die() override;

private:
	bool	shieldIntact;
	int		shieldHp;
};

// 2 HP; slow-moving; fires rockets in a steep arc. Batches of 1-2; score: 100.
class BazookaSoldier : public Enemy
{
public:
	void	update(float dt) override;
	void	takeDamage(int dmg) override;
	void	attack(Character* target) override;
	void	die() override;

private:
	Weapon*	rocketLauncher;
	float	reloadTimer;
};

// 2 HP; lobs grenades in projectile motion toward player. Batches of 1-2; score: 100.
class GrenadeSoldier : public Enemy
{
public:
	void	update(float dt) override;
	void	takeDamage(int dmg) override;
	void	attack(Character* target) override;
	void	die() override;

private:
	float	throwTimer;
	float	grenadeAngle;
};

// Descends from top of screen; wraps one of the four infantry types (inherited batch sizes).
class Paratrooper : public Enemy
{
public:
	void	update(float dt) override;
	void	takeDamage(int dmg) override;
	void	attack(Character* target) override;
	void	die() override;

private:
	Enemy*	innerEnemy;		// wrapped infantry unit (owned composition)
	bool	isDescending;
	bool	parachuteOpen;

	void	land();			// hand control to innerEnemy once grounded
};

// 5 HP; transforms player to MUMMY on contact; crumbles then resurrects after 3 s.
// Can only be permanently killed by fire weapons. Spawns alone; score: 150.
class MummyWarrior : public Enemy
{
public:
	void	update(float dt) override;
	void	takeDamage(int dmg) override;	// crumbles unless dmg is fire-type
	void	attack(Character* target) override;	// sets target state to MUMMY
	void	die() override;				// reachable only via fire damage

private:
	bool	isCrumbled;
	float	resurrectionTimer;		// 3.0 s
	bool	onlyDiesFromFire;

	void	crumble();
	void	resurrect();
};

// 5 HP; walks toward player; transforms to UNDEAD on contact; also has pistol.
// Batches of 3-5; score: 100.
class Zombie : public Enemy
{
public:
	void	update(float dt) override;
	void	takeDamage(int dmg) override;
	void	attack(Character* target) override;	// sets target state to UNDEAD
	void	die() override;

private:
	Weapon*	pistol;
	bool	transformOnContact;
};

// 3 HP total; 2-phase. Phase 1: pod flies above, fires energy beams.
// Phase 2 (pod destroyed): on foot with pistol.
class Martian : public Enemy
{
public:
	void	update(float dt) override;
	void	takeDamage(int dmg) override;
	void	attack(Character* target) override;
	void	die() override;

private:
	int			phase;			// 1=airborne pod | 2=on foot
	sf::Sprite	podSprite;
	int			podHp;
	float		energyBeamTimer;

	void	transitionToPhase2();
};
