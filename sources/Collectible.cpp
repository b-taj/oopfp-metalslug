#include "../headers/Collectible.h"

void Collectible::update(float dt)
{
	(void)dt;
}

void Collectible::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}