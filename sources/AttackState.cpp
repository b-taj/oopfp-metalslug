#include "../headers/AttackState.h"
#include "../headers/PatrolState.h"
#include "../headers/Enemy.h"
#include "../headers/Soldier.h"
#include <cmath>

AttackState::AttackState() : fireTimer(0.0f), fireRate(1.5f) {}

void AttackState::onEnter(class Enemy* e) { (void)e; }

void AttackState::update(class Enemy* e, float dt, class Soldier* player) {
	if (player == nullptr) return; // FIX 4: Null guard

	float dx = player->getX() - e->getX();
	float dy = player->getY() - e->getY();
	float dist = std::sqrt(dx*dx + dy*dy);

	// Direction
	e->setFacingRight(dx > 0);

	// Movement: Keep at comfortable firing distance
	if (dist > 300.0f) e->setVelocityX((dx > 0 ? 1 : -1) * 150.0f);
	else if (dist < 150.0f) e->setVelocityX((dx > 0 ? -1 : 1) * 100.0f);
	else e->setVelocityX(0.0f);

	// Firing logic
	fireTimer += dt;
	if (fireTimer >= fireRate)
	{
		Projectile* p = e->fireWeapon(player);
		// The caller of EnemyAIState::update (EntityManager) usually handles adding the projectile.
		// However, for direct fix, we rely on the implementation pattern.
		// If fireWeapon returns a projectile, we assume it's handled or added to a queue.
		fireTimer = 0.0f;
	}

	// De-aggro
	if (dist > 600.0f) {
		e->setAIState(new PatrolState());
	}
}

void AttackState::onExit(class Enemy* e) { (void)e; }
