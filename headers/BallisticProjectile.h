#pragma once
#include "Projectile.h"
#include "SoundManager.h"

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
	void	setLevelRef(const char** grid, int h, int w, int cell);
	void	setSoundManager(class SoundManager* sm);

private:
	float	gravity;
	bool	explosive;
	float	blastRadius;
	bool	burning;
	float	burnTimer;
	class EntityManager* entityManager;
	class SoundManager* soundManager;	// AGGREGATED

	const char** tileGrid;
	int gridH, gridW, cellSz;
};
