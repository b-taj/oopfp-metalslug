#pragma once
#include "EnemyAIState.h"

/**
 * PatrolState.h -- Default behavior for idle enemies.
 * Moves back and forth until the player enters detection range.
 */

class PatrolState : public EnemyAIState
{
public:
	PatrolState(float range = 200.0f);
	void	onEnter(class Enemy* e) override;
	void	update(class Enemy* e, float dt, class Soldier* player) override;
	void	onExit(class Enemy* e) override;
	void	setLevelRef(const char** grid, int h, int w, int cell);
	EnemyAIMode getMode() const override { return EnemyAIMode::PATROL; }

private:
	float	patrolTimer;
	float	patrolDir;
	float	patrolRange;
	float	spawnX;
	bool	initialized;

	const char** tileGrid;
	int gridH, gridW, cellSz;
};
