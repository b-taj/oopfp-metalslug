#pragma once
#include "Boss.h"
#include "ConcreteWeapons.h"

/**
 * SeaSatan.h -- Aquatic boss for Phase 3.
 */

class SeaSatan : public Boss
{
public:
	SeaSatan(class EntityManager* em);
	~SeaSatan();

	void	spawnMinions() override;
	void	retreat() override;
	void	onDeath() override;
	void	shootPlayer(class Soldier* player) override;
	void	update(float dt, class Soldier* player) override;

private:
	HeavyMachineGun* leftGun;
	HeavyMachineGun* rightGun;
	RocketLauncher*	 rocketLauncher;
};
