#pragma once
#include "EnemyAIState.h"

/**
 * DeadAIState.h -- Null behavior for defeated enemies.
 */

class DeadAIState : public EnemyAIState
{
public:
	void	onEnter(class Enemy* e) override {}
	void	update(class Enemy* e, float dt, class Soldier* player) override {}
	void	onExit(class Enemy* e) override {}
	EnemyAIMode getMode() const override { return EnemyAIMode::DEAD; }
};
