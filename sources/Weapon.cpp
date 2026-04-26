#include "../headers/Weapon.h"
#include "../headers/Projectile.h"
#include <cmath>

bool Weapon::canFire()
{
	// Ensure variables are initialized if they were garbage
	if (fireTimer > 1000.0f || fireTimer < -1000.0f) fireTimer = 0;
	return fireTimer <= 0;
}

void Weapon::reload(int amount) { (void)amount; }
int Weapon::getAmmo() { return 999; }
int Weapon::getDamage() { return damage; }

void Weapon::update(float dt)
{
	if (fireTimer > 0)
		fireTimer -= dt;
}

Projectile* createBasicProjectile(float x, float y, float angle, int damage, float speed)
{
	Projectile* p = new Projectile();
	p->x = x;
	p->y = y;
	
	float rad = angle * 3.14159f / 180.0f;
	p->velX = std::cos(rad) * speed;
	p->velY = std::sin(rad) * speed;
	
	p->damage = damage;
	p->isAlive = true;
	p->type = ProjectileType::BULLET;
	
	// Visibility: Use a 32x32 area. 
	p->loadTexture("Sprites/blocks/stone.png");
	p->setTextureRect(0, 0, 32, 32); 
	return p;
}

Projectile* Pistol::fire(float x, float y, float angle)
{
	if (!canFire()) return nullptr;
	fireTimer = 0.2f; 
	return createBasicProjectile(x, y, angle, 10, 1200.0f);
}

Projectile* HeavyMachineGun::fire(float x, float y, float angle)
{
	if (!canFire()) return nullptr;
	fireTimer = 0.1f;
	return createBasicProjectile(x, y, angle, 15, 1500.0f);
}

Projectile* RocketLauncher::fire(float x, float y, float angle)
{
	if (!canFire()) return nullptr;
	fireTimer = 0.5f;
	Projectile* p = createBasicProjectile(x, y, angle, 50, 800.0f);
	p->type = ProjectileType::ROCKET;
	return p;
}

Projectile* FlameShot::fire(float x, float y, float angle)
{
	if (!canFire()) return nullptr;
	fireTimer = 0.05f;
	Projectile* p = createBasicProjectile(x, y, angle, 5, 600.0f);
	p->type = ProjectileType::FLAME_STREAM;
	return p;
}

Projectile* LaserGun::fire(float x, float y, float angle)
{
	if (!canFire()) return nullptr;
	fireTimer = 0.5f;
	Projectile* p = createBasicProjectile(x, y, angle, 100, 2500.0f);
	p->type = ProjectileType::LASER_BEAM;
	return p;
}
