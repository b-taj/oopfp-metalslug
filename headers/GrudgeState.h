#pragma once
#include "EnemyAIState.h"

/**
 * GrudgeState.h -- Relentless pursuit behavior.
 * Triggered when a player tries to bypass the enemy.
 */

class GrudgeState : public EnemyAIState
{
public:
	GrudgeState(float power = 1.5f);
	void	onEnter(class Enemy* e) override;
	void	update(class Enemy* e, float dt, class Soldier* player) override;
	void	onExit(class Enemy* e) override;
	EnemyAIMode getMode() const override { return EnemyAIMode::GRUDGE; }

private:
	float	powerMult;
};
