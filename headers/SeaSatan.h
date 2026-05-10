#pragma once
#include "Boss.h"
#include "Weapon.h"

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
	Weapon* leftGun;
	Weapon* rightGun;
	Weapon* rocketLauncher;
};
