#include "../headers/Weapon.h"

Weapon::Weapon() 
	: ammo(0), infiniteAmmo(false), fireRate(1.0f), fireTimer(1.0f), damage(0) 
{}

void Weapon::update(float dt)
{
	// Accumulate time towards the next shot
	if (fireTimer < 1.0f / fireRate) {
		fireTimer += dt;
	}
}

bool Weapon::canFire() const
{
	return (fireTimer >= 1.0f / fireRate) && (infiniteAmmo || ammo > 0);
}

void Weapon::reload(int amount) { ammo += amount; }
int Weapon::getAmmo() const { return ammo; }
int Weapon::getDamage() const { return damage; }
