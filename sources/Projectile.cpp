#include "../headers/Projectile.h"
#include <cmath>

void Projectile::update(float dt)
{
	if (!isAlive) return;

	x += velX * dt;
	y += velY * dt;

	sprite.setPosition(x, y);

	// Deactivate only when truly outside the scrolling world bounds
	// Level is 110 tiles * 64px = 7040px wide. 
	if (x < -500 || x > 8000 || y < -500 || y > 2000)
	{
		isAlive = false;
	}
}

void Projectile::draw(sf::RenderWindow& window)
{
	if (isAlive)
	{
		window.draw(sprite);
	}
}

void Projectile::onHit()
{
	isAlive = false;
}

void Projectile::loadTexture(const char* filename)
{
	if (!texture.loadFromFile(filename)) {
		// Fallback: SFML draws a white box if no texture is found
	}
	sprite.setTexture(texture);
}

void Projectile::setTextureRect(int left, int top, int width, int height)
{
	sprite.setTextureRect(sf::IntRect(left, top, width, height));
}

void Projectile::setPosition(float nx, float ny)
{
	x = nx;
	y = ny;
	sprite.setPosition(x, y);
}
