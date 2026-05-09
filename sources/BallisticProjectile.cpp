#include "../headers/BallisticProjectile.h"
#include "../headers/EntityManager.h"
#include "../headers/Constants.h"
#include <cmath>

BallisticProjectile::BallisticProjectile(float sx, float sy, float vx, float vy, int dmg, bool playerOwned, EntityManager* em)
	: gravity(GRAVITY), explosive(false), blastRadius(0.0f), burning(false), burnTimer(0.0f), entityManager(em)
{
	x = sx; y = sy;
	velocityX = vx; velocityY = vy;
	damage = dmg;
	fromPlayer = playerOwned;
	isEnemyProjectile = !playerOwned;
	projType = ProjectileType::BALLISTIC;
}

void BallisticProjectile::update(float dt)
{
	velocityY += gravity * dt;
	x += velocityX * dt;
	y += velocityY * dt;

	// Check for ground impact (Simple height check for now, can be refined with grid collision)
	if (y > SCREEN_H - 100.0f) { 
		if (explosive) explode();
		active = false;
	}

	// Bounds check
	if (x < -100 || x > LEVEL_WIDTH * CELL_SIZE + 100) active = false;
}

void BallisticProjectile::explode()
{
	if (!entityManager) return;

	Enemy** enemies = entityManager->getEnemies();
	for (int i = 0; i < entityManager->getEnemyCount(); ++i) {
		float dx = enemies[i]->getX() - x;
		float dy = enemies[i]->getY() - y;
		float dist = std::sqrt(dx*dx + dy*dy);

		if (dist <= blastRadius) {
			enemies[i]->takeDamage(damage * 2); // Double damage at epicenter
		}
	}
}

void BallisticProjectile::setExplosive(bool e, float radius) { explosive = e; blastRadius = radius; }
void BallisticProjectile::setBurning(bool b, float time) { burning = b; burnTimer = time; }
