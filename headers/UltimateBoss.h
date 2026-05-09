#pragma once
#include "Boss.h"
#include "IronNokana.h"
#include "HairbusterRiberts.h"
#include "SeaSatan.h"

class UltimateBoss : public Boss
{
public:
	UltimateBoss(class EntityManager* em);
	~UltimateBoss();

	void	spawnMinions() override;
	void	retreat() override;
	void	onDeath() override;
	void	shootPlayer(class Soldier* player) override;
	void	update(float dt, class Soldier* player) override;
	void	takeDamage(int dmg) override;

private:
	void	transitionPhase();
	Boss*	getCurrentBoss();

	IronNokana*			groundBoss;
	HairbusterRiberts*	aerialBoss;
	SeaSatan*			aquaticBoss;

	BossPhase			currentPhase;
	float				phaseTimer;
	int					groundHp;
	int					aerialHp;
	int					aquaticHp;
};
