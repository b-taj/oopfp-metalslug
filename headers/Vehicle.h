#pragma once
#include "DamagableEntity.h"
#include "Enums.h"
#include "SpriteAnimator.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

/**
 * Vehicle.h -- Base class for all controllable vehicles.
 * Aggregates a Soldier as a driver and manages health/ammo.
 */

class Vehicle : public DamagableEntity
{
public:
	virtual ~Vehicle() = default;

	virtual class Projectile* fire() = 0;
	virtual VehicleForm getForm() const = 0;

	void	takeDamage(int dmg) override;
	void	die() override;

	void	enter(class Soldier* s);
	void	ejectDriver();

	void	update(float dt) override;
	void	draw(sf::RenderWindow& w, float cx, float cy) override;
	void	loadTexture(const char* path);

protected:
	Vehicle();

	SpriteAnimator	animator;
	sf::Texture		texture;
	sf::Sprite		sprite;

	float			velocityX;
	float			velocityY;
	int				bombAmmo;
	class Soldier*	driver; // AGGREGATED
	bool			isDestroyed;
};
