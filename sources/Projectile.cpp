#include "../headers/Projectile.h"

Projectile::Projectile()
	: damage(0), velocityX(0.0f), velocityY(0.0f), fromPlayer(true), 
	  projType(ProjectileType::STRAIGHT), isEnemyProjectile(false)
{
	active = true;
	width = 16.0f;
	height = 16.0f;
}

void Projectile::draw(sf::RenderWindow& window, float camOX, float camOY)
{
	if (!active) return;
	sprite.setPosition(x - camOX, y - camOY);
	window.draw(sprite);
}

int Projectile::getDamage() const { return damage; }
bool Projectile::isFromPlayer() const { return fromPlayer; }

void Projectile::loadTexture(const char* path)
{
	if (texture.loadFromFile(path)) {
		sprite.setTexture(texture);
	}
}
