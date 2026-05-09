#include "../headers/PatrolState.h"
#include "../headers/AttackState.h"
#include "../headers/Enemy.h"
#include "../headers/Soldier.h"
#include <cmath>

PatrolState::PatrolState(float range) 
	: patrolTimer(0.0f), patrolDir(1.0f), patrolRange(range), spawnX(0.0f), initialized(false) 
{}

void PatrolState::onEnter(class Enemy* e) {
	if (!initialized) {
		spawnX = e->getX();
		initialized = true;
	}
}

void PatrolState::update(class Enemy* e, float dt, class Soldier* player) {
	// Movement logic
	float currentX = e->getX();
	if (currentX > spawnX + patrolRange) patrolDir = -1.0f;
	else if (currentX < spawnX - patrolRange) patrolDir = 1.0f;

	e->setVelocityX(patrolDir * 100.0f); // Move at patrol speed
	
	// Aggro check
	float dx = player->getX() - e->getX();
	float dy = player->getY() - e->getY();
	if (std::sqrt(dx*dx + dy*dy) < 400.0f) {
		e->setAIState(new AttackState());
	}
}

void PatrolState::onExit(class Enemy* e) { (void)e; }
