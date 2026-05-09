#pragma once
#include "Boss.h"
#include "Weapon.h"

/**
 * IronNokana.h -- Ground boss for Phase 1.
 */

class IronNokana : public Boss
{
public:
	IronNokana(class EntityManager* em);
	~IronNokana();

	void	spawnMinions() override;
	void	retreat() override;
	void	onDeath() override;
	void	shootPlayer(class Soldier* player) override;
	void	update(float dt, class Soldier* player) override;

private:
	Weapon*	missileTurret;
	Weapon*	flameShot;
};
