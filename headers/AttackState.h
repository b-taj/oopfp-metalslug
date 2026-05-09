#pragma once
#include "EnemyAIState.h"

/**
 * AttackState.h -- Combat behavior.
 * Pursues and fires at the player.
 */

class AttackState : public EnemyAIState
{
public:
	AttackState();
	void	onEnter(class Enemy* e) override;
	void	update(class Enemy* e, float dt, class Soldier* player) override;
	void	onExit(class Enemy* e) override;
	EnemyAIMode getMode() const override { return EnemyAIMode::ATTACK; }

private:
	float	fireTimer;
};
