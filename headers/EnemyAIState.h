#pragma once
#include "Enums.h"

/**
 * EnemyAIState.h -- Abstract base for the Enemy AI State Pattern.
 * Decouples behavioral logic from the Enemy class.
 */

class EnemyAIState
{
public:
	virtual ~EnemyAIState() = default;

	virtual void	onEnter(class Enemy* e) = 0;
	virtual void	update(class Enemy* e, float dt, class Soldier* player) = 0;
	virtual void	onExit(class Enemy* e) = 0;
	virtual EnemyAIMode getMode() const = 0;
};
