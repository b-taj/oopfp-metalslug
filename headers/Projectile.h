#pragma once
#include "Entity.h"
#include "Enums.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

/**
 * Projectile.h -- Abstract base for all in-flight combat objects.
 * Inherits from Entity.
 */

class Projectile : public Entity
{
public:
	virtual ~Projectile() = default;

	virtual void	update(float dt) = 0;
	void			draw(sf::RenderWindow& window, float camOX, float camOY) override;

	int				getDamage() const;
	bool			isFromPlayer() const;
	void			loadTexture(const char* path);

	// Public access for collision checks
	bool			isEnemyProjectile; // Mapping fromPlayer to this for existing logic
	int				damage;

protected:
	Projectile();

	float			velocityX;
	float			velocityY;
	bool			fromPlayer;
	ProjectileType	projType;

	sf::Sprite		sprite;
	sf::Texture		texture;
};
