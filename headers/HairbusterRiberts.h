#pragma once
#include "Boss.h"
#include "Weapon.h"

class HairbusterRiberts : public Boss
{
public:
	HairbusterRiberts(class EntityManager* em);
	~HairbusterRiberts();

	void	spawnMinions() override;
	void	retreat() override;
	void	onDeath() override;
	void	shootPlayer(class Soldier* player) override;
	void	update(float dt, class Soldier* player) override;

private:
	Weapon* rocketBays[4];
};
