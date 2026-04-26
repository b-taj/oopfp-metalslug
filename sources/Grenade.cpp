#include "../headers/Grenade.h"

void Grenade::update(float dt)
{
	(void)dt;
}

void Grenade::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Grenade::launch(float ox, float oy, float angle)
{
	(void)ox; (void)oy; (void)angle;
}

void HandGrenade::explode()
{
}

void FireBombGrenade::explode()
{
}

void FireBombGrenade::updateFirePool(float dt)
{
	(void)dt;
}