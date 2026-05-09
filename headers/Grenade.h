#pragma once
#include <SFML/Graphics.hpp>
#include "Enums.h"

// Grenade.h -- Player explosive weapon. Sub-classes for HandGrenade and FireBombGrenade.
// explode() creates a blast radius that destroys destructible blocks and damages enemies.

class Grenade
{
public:
	virtual ~Grenade();

	virtual void	update(float dt) = 0;
	virtual void	explode() = 0;

	void			draw(sf::RenderWindow& window);
	sf::FloatRect	getBounds();

protected:
	float		x;
	float		y;
	float		velX;
	float		velY;
	int			damage;
	float		blastRadius;
	float		fuseTimer;		// seconds until automatic explode()
	sf::Sprite	sprite;
	sf::Texture	texture;
	bool		isExploded;
};

class HandGrenade : public Grenade
{
public:
	void	update(float dt) override;
	void	explode() override;
};

class FireBombGrenade : public Grenade
{
public:
	void	update(float dt) override;
	void	explode() override;
};
