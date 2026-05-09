#pragma once
#include "Projectile.h"

/**
 * BallisticProjectile.h -- Arcing projectiles (Rockets, Grenades).
 * Handles explosions and area-of-effect damage.
 */

class BallisticProjectile : public Projectile
{
public:
	BallisticProjectile(float sx, float sy, float vx, float vy, int dmg, bool playerOwned, class EntityManager* em);
	
	void	update(float dt) override;
	void	explode();

	void	setExplosive(bool e, float radius);
	void	setBurning(bool b, float time);

private:
	float	gravity;
	bool	explosive;
	float	blastRadius;
	bool	burning;
	float	burnTimer;
	class EntityManager* entityManager;
};
