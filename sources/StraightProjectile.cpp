#include "../headers/StraightProjectile.h"
#include "../headers/Constants.h"

StraightProjectile::StraightProjectile(float sx, float sy, float vx, float vy, int dmg, bool playerOwned)
{
	x = sx; y = sy;
	velocityX = vx; velocityY = vy;
	damage = dmg;
	fromPlayer = playerOwned;
	isEnemyProjectile = !playerOwned;
	projType = ProjectileType::STRAIGHT;
}

void StraightProjectile::update(float dt)
{
	x += velocityX * dt;
	y += velocityY * dt;

	// Bounds check
	if (x < -100 || x > LEVEL_WIDTH * CELL_SIZE + 100 || y < -500 || y > SCREEN_H + 500) {
		active = false;
	}
}
