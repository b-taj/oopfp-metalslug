#pragma once
#include "Enemy.h"

/**
 * Boss.h -- Abstract base class for all phase bosses.
 * Inherits from Enemy and manages phase transitions and minion spawning.
 */

class Boss : public Enemy
{
public:
	virtual ~Boss() = default;

	virtual void	spawnMinions() = 0;
	virtual void	retreat() = 0;

	bool			isAtHalfHp() const;

protected:
	Boss(class EntityManager* em);

	char			minionType[32];
	float			minionTimer;
	class EntityManager* entityMgr; // AGGREGATED
};
