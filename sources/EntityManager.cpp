#include "../headers/EntityManager.h"
#include <SFML/Graphics/Rect.hpp>

EntityManager::EntityManager()
	: enemyCount(0), projectileCount(0), collectibleCount(0)
{
	for (int i = 0; i < MAX_ENEMIES; ++i) enemies[i] = nullptr;
	for (int i = 0; i < MAX_PROJECTILES; ++i) projectiles[i] = nullptr;
	for (int i = 0; i < MAX_COLLECTIBLES; ++i) collectibles[i] = nullptr;
}

EntityManager::~EntityManager()
{
	clear();
}

void EntityManager::addEnemy(Enemy* e)
{
	if (enemyCount < MAX_ENEMIES && e) {
		enemies[enemyCount++] = e;
	}
}

void EntityManager::addProjectile(Projectile* p)
{
	if (projectileCount < MAX_PROJECTILES && p) {
		projectiles[projectileCount++] = p;
	}
}

void EntityManager::addCollectible(Collectible* c)
{
	if (collectibleCount < MAX_COLLECTIBLES && c) {
		collectibles[collectibleCount++] = c;
	}
}

void EntityManager::update(float dt, Soldier* player, bool interactPressed, float camOffsetX)
{
	// 1. Update all active entities
	for (int i = 0; i < enemyCount; ++i) {
		if (enemies[i]->isActive()) {
			enemies[i]->update(dt, player);

			// GRUDGE DETECTION: If enemy is scrolled past without being killed
			if (enemies[i]->getX() < camOffsetX - 100.0f) {
				enemies[i]->activateGrudge();
			}
		}
	}

	for (int i = 0; i < projectileCount; ++i) {
		if (projectiles[i]->isActive()) {
			projectiles[i]->update(dt);
		}
	}

	for (int i = 0; i < collectibleCount; ++i) {
		if (collectibles[i]->isActive()) {
			collectibles[i]->update(dt);
		}
	}

	for (int i = 0; i < interactableCount; ++i) {
		if (interactables[i]->isActive()) {
			interactables[i]->update(dt);
		}
	}

	// 2. Collisions & Interactions
	checkCollisions(player);
	checkCollectiblePickup(player);
	checkInteractions(player, interactPressed);
}

void EntityManager::checkInteractions(Soldier* player, bool pressed)
{
	for (int i = 0; i < interactableCount; ++i) {
		if (interactables[i]->isActive()) {
			interactables[i]->checkInteraction(player, pressed);
		}
	}
}

void EntityManager::checkCollisions(Soldier* player)
{
	sf::FloatRect playerBounds = player->getBounds();

	for (int i = 0; i < projectileCount; ++i) {
		if (!projectiles[i]->isActive()) continue;
		sf::FloatRect pBounds = projectiles[i]->getBounds();

		if (projectiles[i]->isEnemyProjectile) {
			// Enemy Projectile vs Player
			if (pBounds.intersects(playerBounds)) {
				player->takeDamage(projectiles[i]->damage);
				projectiles[i]->setActive(false);
			}
		} else {
			// Player Projectile vs Enemies
			for (int j = 0; j < enemyCount; ++j) {
				if (!enemies[j]->isActive()) continue;
				if (pBounds.intersects(enemies[j]->getBounds())) {
					enemies[j]->takeDamage(projectiles[i]->damage);
					projectiles[i]->setActive(false);
					break;
				}
			}
		}
	}

	// Enemy vs Player (Contact Damage)
	for (int i = 0; i < enemyCount; ++i) {
		if (enemies[i]->isActive() && enemies[i]->getBounds().intersects(playerBounds)) {
			player->takeDamage(5); // Fixed contact damage
		}
	}
}

void EntityManager::checkCollectiblePickup(Soldier* player)
{
	sf::FloatRect playerBounds = player->getBounds();
	for (int i = 0; i < collectibleCount; ++i) {
		if (collectibles[i]->isActive() && collectibles[i]->getBounds().intersects(playerBounds)) {
			collectibles[i]->onPickup(player);
			collectibles[i]->setActive(false);
		}
	}
}

void EntityManager::cleanupInactive()
{
	// Swap-with-last pattern
	for (int i = 0; i < enemyCount; ) {
		if (!enemies[i]->isActive()) {
			delete enemies[i];
			enemies[i] = enemies[--enemyCount];
		} else {
			i++;
		}
	}

	for (int i = 0; i < projectileCount; ) {
		if (!projectiles[i]->isActive()) {
			delete projectiles[i];
			projectiles[i] = projectiles[--projectileCount];
		} else {
			i++;
		}
	}

	for (int i = 0; i < collectibleCount; ) {
		if (!collectibles[i]->isActive()) {
			delete collectibles[i];
			collectibles[i] = collectibles[--collectibleCount];
		} else {
			i++;
		}
	}
}

void EntityManager::clear()
{
	for (int i = 0; i < enemyCount; ++i) delete enemies[i];
	enemyCount = 0;

	for (int i = 0; i < projectileCount; ++i) delete projectiles[i];
	projectileCount = 0;

	for (int i = 0; i < collectibleCount; ++i) delete collectibles[i];
	collectibleCount = 0;
}

Enemy** EntityManager::getEnemies() { return enemies; }
int EntityManager::getEnemyCount() const { return enemyCount; }
Projectile** EntityManager::getProjectiles() { return projectiles; }
int EntityManager::getProjectileCount() const { return projectileCount; }
