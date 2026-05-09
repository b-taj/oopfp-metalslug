#pragma once
#include <SFML/Graphics.hpp>

// Grenade.h -- Abstract throwable explosive. Sub-classes override explode()
// for their unique detonation effect. Physics (arc, gravity) live in the base.

class Grenade
{
public:
	virtual ~Grenade() = default;

	// Detonation effect defined per sub-class (damage, radius, fire pool).
	virtual void	explode() = 0;

	// Step ballistic arc physics; detect ground or enemy contact.
	void		update(float dt);

	void		draw(sf::RenderWindow& window);

	// Compute and apply initial velocity from origin at given angle.
	void		launch(float ox, float oy, float angle);

protected:
	float		x;
	float		y;
	float		velocityX;
	float		velocityY;
	int			damage;
	float		blastRadius;		// radius in blocks
	bool		hasExploded;
	sf::Sprite	sprite;
	sf::Texture	texture;
};

// Default grenade; 5 HP damage; 3-block blast radius.
class HandGrenade : public Grenade
{
public:
	void	explode() override;
};

// Eri's default grenade; explodes on impact; leaves a 3-block fire pool for 10 s.
class FireBombGrenade : public Grenade
{
public:
	void	explode() override;

	// Tick fire pool duration; deactivate when timer reaches 0.
	void	updateFirePool(float dt);

private:
	float	firePoolTimer;		// 10.0 s remaining
	float	firePoolRadius;		// 3 blocks
};
