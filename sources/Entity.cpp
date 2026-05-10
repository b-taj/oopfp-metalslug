#include "../headers/Entity.h"

sf::FloatRect Entity::getBounds() const
{
	return sf::FloatRect(x, y, width, height);
}

float Entity::getX() const { return x; }
float Entity::getY() const { return y; }
float Entity::getWidth() const { return width; }
float Entity::getHeight() const { return height; }
bool Entity::isActive() const { return active; }

void Entity::setPosition(float nx, float ny)
{
	x = nx;
	y = ny;
}

void Entity::setActive(bool a)
{
	active = a;
}
