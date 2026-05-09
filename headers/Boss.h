#pragma once
#include "Enemy.h"

// Boss.h -- Abstract boss extension of Enemy. Adds minion spawning and retreat.
// UltimateBoss (Phase 4) owns and delegates to all three phase bosses;
// attack() is dispatched polymorphically through a Boss* pointer.

class Boss : public Enemy
{
public:
	virtual ~Boss() = default;

	// Spawn a batch of 2-4 minions; called after each batch is cleared.
	virtual void	spawnMinions() = 0;

	// Move boss off-screen when HP drops to 50% (phases 1-3 only).
	virtual void	retreat() = 0;

	// Returns true when current hp <= maxHp / 2.
	bool	isAtHalfHp();

protected:
	int			maxHp;			// 30 for all standard boss phases
	std::string	minionType;		// enemy class name spawned as minions
	float		minionTimer;		// seconds between minion batch spawns
};

// Ground boss. 30 HP. Missile turret on top + flamethrower on bottom + fire-bomb grenades.
// Spawns only on flat plains terrain.
class IronNokana : public Boss
{
public:
	void	update(float dt) override;
	void	takeDamage(int dmg) override;
	void	attack(Character* target) override;
	void	die() override;
	void	spawnMinions() override;
	void	retreat() override;

private:
	Weapon*		missileTurret;
	FlameShot	flamethrower;
	float		attackTimer;
};

// Aerial boss. 30 HP. Multiple rocket bays + fire-bomb grenades. Spawns at high altitude.
class HairbusterRiberts : public Boss
{
public:
	void	update(float dt) override;
	void	takeDamage(int dmg) override;
	void	attack(Character* target) override;
	void	die() override;
	void	spawnMinions() override;
	void	retreat() override;

private:
	std::vector<RocketLauncher*>	rocketBays;
	float					altitudePref;
};

// Aquatic boss. 30 HP. Twin HMG cannons (one each side) + rocket launcher.
class SeaSatan : public Boss
{
public:
	void	update(float dt) override;
	void	takeDamage(int dmg) override;
	void	attack(Character* target) override;
	void	die() override;
	void	spawnMinions() override;
	void	retreat() override;

private:
	HeavyMachineGun	leftGun;
	HeavyMachineGun	rightGun;
	RocketLauncher	rocketLauncher;
};

// Phase 4 fused boss. Cycles GROUND -> AERIAL -> AQUATIC every 10-15 s.
// Each phase has its own HP pool. attack() dispatches polymorphically via getCurrentBoss().
class UltimateBoss : public Boss
{
public:
	void	update(float dt) override;
	void	takeDamage(int dmg) override;	// routes damage to the active phase HP pool
	void	attack(Character* target) override;	// polymorphic via getCurrentBoss()
	void	die() override;
	void	spawnMinions() override;
	void	retreat() override;

private:
	IronNokana*			groundBoss;
	HairbusterRiberts*	aerialBoss;
	SeaSatan*			aquaticBoss;
	BossPhase			currentPhase;
	float				phaseTimer;		// counts down to next transition (10-15 s)
	int					groundHp;
	int					aerialHp;
	int					aquaticHp;

	void	transitionPhase();		// advance currentPhase in cycle
	Boss*	getCurrentBoss();		// pointer to the active sub-boss
};
