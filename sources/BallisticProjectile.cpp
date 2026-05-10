#include "../headers/BallisticProjectile.h"
#include "../headers/EntityManager.h"
#include "../headers/Constants.h"
#include <cmath>

BallisticProjectile::BallisticProjectile(float sx, float sy, float vx, float vy, int dmg, bool playerOwned, EntityManager* em)
	: gravity(900.0f), explosive(false), blastRadius(0.0f), burning(false), burnTimer(0.0f), entityManager(em),
	  tileGrid(nullptr), gridH(0), gridW(0), cellSz(1), soundManager(nullptr)
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

	// Grid-based ground collision
	if (tileGrid) {
		int col = (int)(x / cellSz);
		int row = (int)((y + height) / cellSz);
		if (col >= 0 && col < gridW && row >= 0 && row < gridH) {
			char tile = tileGrid[row][col];
			if (tile != '\0' && tile != ' ') { // Any solid tile
				if (explosive) explode();
				active = false;
				return;
			}
		}
	}

	// Fallback/Bounds check
	if (x < -100 || x > LEVEL_WIDTH * CELL_SIZE + 100 || y > SCREEN_H + 500) active = false;
}

void BallisticProjectile::setLevelRef(const char** grid, int h, int w, int cell)
{
	tileGrid = grid;
	gridH = h;
	gridW = w;
	cellSz = cell;
}

void BallisticProjectile::setSoundManager(SoundManager* sm)
{
	soundManager = sm;
}

void BallisticProjectile::explode()
{
	if (soundManager) soundManager->play("explosion");
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
