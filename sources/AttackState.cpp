#include "../headers/AttackState.h"
#include "../headers/PatrolState.h"
#include "../headers/Enemy.h"
#include "../headers/Soldier.h"
#include <cmath>

AttackState::AttackState() : fireTimer(0.0f) {}

void AttackState::onEnter(class Enemy* e) { (void)e; }

void AttackState::update(class Enemy* e, float dt, class Soldier* player) {
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
	if (fireTimer >= 1.0f) { // 1 shot per second in attack state
		fireTimer = 0.0f;
		e->shootPlayer(player);
	}

	// De-aggro
	if (dist > 600.0f) {
		e->setAIState(new PatrolState());
	}
}

void AttackState::onExit(class Enemy* e) { (void)e; }
