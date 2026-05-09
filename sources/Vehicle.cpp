#include "../headers/Vehicle.h"

void Vehicle::takeDamage(int dmg)
{
	(void)dmg;
}

void Vehicle::dropBomb()
{
}

void Vehicle::ejectDriver()
{
}

void Vehicle::enter(Character* c)
{
	(void)c;
}

void Vehicle::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

sf::FloatRect Vehicle::getBounds()
{
	return sprite.getGlobalBounds();
}