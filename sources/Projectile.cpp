#include "../headers/Projectile.h"

Projectile::Projectile()
	: damage(0), velocityX(0.0f), velocityY(0.0f), fromPlayer(true), 
	  projType(ProjectileType::STRAIGHT), isEnemyProjectile(false)
{
	active = true;
	width = 6.0f;
	height = 3.0f;
    
    rectShape.setSize(sf::Vector2f(width, height));
    rectShape.setFillColor(sf::Color::Red);
}

void Projectile::draw(sf::RenderWindow& window, float camOX, float camOY)
{
	if (!active) return;
	rectShape.setPosition(x - camOX, y - camOY);
	window.draw(rectShape);
}

int Projectile::getDamage() const { return damage; }
bool Projectile::isFromPlayer() const { return fromPlayer; }

void Projectile::loadTexture(const char* path)
{
    (void)path; // Placeholders ignore textures
}
