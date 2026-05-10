#include "../headers/Vehicle.h"
#include "../headers/Soldier.h"

Vehicle::Vehicle() 
	: velocityX(0.0f), velocityY(0.0f), bombAmmo(10), driver(nullptr), isDestroyed(false)
{
	active = true;
	hp = 3; // 3-hit model as per rubric
	maxHp = 3;
}

void Vehicle::takeDamage(int dmg)
{
	hp -= dmg;
	if (hp == 1) {
		// Warning logic for driver
	}
	if (hp <= 0) die();
}

void Vehicle::die()
{
	isDestroyed = true;
	ejectDriver();
	active = false;
}

void Vehicle::enter(Soldier* s)
{
	if (!s) return;
	driver = s;
	driver->setActive(false); // Hide soldier while driving
}

void Vehicle::ejectDriver()
{
	if (driver) {
		driver->setActive(true);
		driver->setPosition(x, y - 50.0f); // Eject slightly above
		driver = nullptr;
	}
}

void Vehicle::update(float dt)
{
	if (!active || isDestroyed) return;

	if (driver) {
		// Input handling logic...
	}

	x += velocityX * dt;
	y += velocityY * dt;

	animator.update(dt);
	animator.applyToSprite(sprite);
}

void Vehicle::draw(sf::RenderWindow& w, float cx, float cy)
{
	if (!active) return;
	sprite.setPosition(x - cx, y - cy);
	w.draw(sprite);
}

void Vehicle::loadTexture(const char* path)
{
	if (texture.loadFromFile(path)) {
		sprite.setTexture(texture);
	}
}
