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

	// FIX 3: Projectiles persist until they leave level bounds
	if (x < -100.0f || x > LEVEL_WIDTH * CELL_SIZE + 100.0f ||
		y < -200.0f || y > LEVEL_HEIGHT * CELL_SIZE + 100.0f)
	{
		active = false;
	}
}
