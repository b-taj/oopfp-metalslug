#include "../headers/Boss.h"

void Boss::update(float dt)
{
	(void)dt;
}

void Boss::takeDamage(int dmg)
{
	(void)dmg;
}

void Boss::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}