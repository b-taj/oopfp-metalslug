#include "../headers/PatrolState.h"
#include "../headers/AttackState.h"
#include "../headers/Enemy.h"
#include "../headers/Soldier.h"
#include <cmath>

PatrolState::PatrolState(float range) 
	: patrolTimer(0.0f), patrolDir(1.0f), patrolRange(range), spawnX(0.0f), initialized(false),
	  tileGrid(nullptr), gridH(0), gridW(0), cellSz(1)
{}

void PatrolState::onEnter(class Enemy* e) {
	if (!initialized) {
		spawnX = e->getX();
		initialized = true;
	}
}

void PatrolState::update(class Enemy* e, float dt, class Soldier* player) {
	// Platform Edge Detection
	if (tileGrid) {
		int nextCol = (int)((e->getX() + (patrolDir > 0 ? e->getWidth() : 0) + patrolDir * 10.0f) / cellSz);
		int rowBelow = (int)((e->getY() + e->getHeight() + 2) / cellSz);
		
		if (nextCol >= 0 && nextCol < gridW && rowBelow >= 0 && rowBelow < gridH) {
			if (tileGrid[rowBelow][nextCol] == ' ') { // No floor ahead
				patrolDir *= -1.0f;
			}
		}
	}

	float currentX = e->getX();
	if (currentX > spawnX + patrolRange) patrolDir = -1.0f;
	else if (currentX < spawnX - patrolRange) patrolDir = 1.0f;

	e->setVelocityX(patrolDir * 100.0f);
	
	float dx = player->getX() - e->getX();
	float dy = player->getY() - e->getY();
	if (std::sqrt(dx*dx + dy*dy) < 400.0f) {
		e->setAIState(new AttackState());
	}
}

void PatrolState::onExit(class Enemy* e) { (void)e; }

void PatrolState::setLevelRef(const char** grid, int h, int w, int cell) {
	tileGrid = grid; gridH = h; gridW = w; cellSz = cell;
}
