#pragma once
#include "Constants.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Collectible.h"
#include "Soldier.h"

/**
 * EntityManager.h -- Central hub for entity lifecycle and collision logic.
 * Manages fixed-size pools for enemies, projectiles, and collectibles.
 */

#include "InteractableObject.h"

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	void		addEnemy(Enemy* e);
	void		addProjectile(Projectile* p);
	void		addCollectible(Collectible* c);
	void		addInteractable(InteractableObject* i);

	void		update(float dt, Soldier* player, bool interactPressed);
	void		cleanupInactive();
	void		clear();

	Enemy**			getEnemies();
	int				getEnemyCount() const;
	Projectile**	getProjectiles();
	int				getProjectileCount() const;

private:
	void		checkCollisions(Soldier* player);
	void		checkCollectiblePickup(Soldier* player);
	void		checkInteractions(Soldier* player, bool pressed);

	Enemy*			enemies[MAX_ENEMIES];
	int				enemyCount;

	Projectile*		projectiles[MAX_PROJECTILES];
	int				projectileCount;

	Collectible*	collectibles[MAX_COLLECTIBLES];
	int				collectibleCount;

	InteractableObject* interactables[MAX_COLLECTIBLES];
	int				interactableCount;
};
