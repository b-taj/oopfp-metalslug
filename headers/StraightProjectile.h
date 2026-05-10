#pragma once
#include "Projectile.h"

/**
 * StraightProjectile.h -- Standard bullet behavior.
 */

class StraightProjectile : public Projectile
{
public:
	StraightProjectile(float sx, float sy, float vx, float vy, int dmg, bool playerOwned);
	void	update(float dt) override;
};
