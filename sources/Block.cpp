#include "../headers/Block.h"

void Block::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Block::destroy()
{
}

void Block::flood()
{
}

sf::FloatRect Block::getBounds()
{
	return sprite.getGlobalBounds();
}