#pragma once
#include "DamagableEntity.h"
#include "TransformationState.h"
#include "Weapon.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

/**
 * Soldier.h -- Base class for all humanoid units (Players and NPCs).
 * Inherits from DamagableEntity. Owns its transformation state and weapons.
 */

class Soldier : public DamagableEntity
{
public:
	virtual ~Soldier();

	virtual void	activateSpecialPower() = 0;
	virtual float	getFireRate() const = 0;
	virtual const char* getName() const = 0;

	void			update(float dt) override;
	void			draw(sf::RenderWindow& window, float camOffsetX, float camOffsetY) override;

	void			takeDamage(int dmg) override;
	void			die() override;

	void			setTransformationState(TransformationState* next);
	void			forceKnifeOnly(bool f);
	void			resolveGround(char** grid, int h, int w, int cell);
	
	class Projectile* shoot(float angle);
	
	void			loadTexture(const char* path);
	void			setSpriteScale(float sx, float sy);
	void			setSpeed(float spd, float jumpForce);

	int				getLives() const;
	int				getGrenadeCount() const;

	// Movement utility for states
	void			setSpeedMultiplier(float m);

protected:
	Soldier();

	sf::Texture		texture;
	sf::Sprite		sprite;

	float			speedX;			// Base move speed
	float			speedY;			// Jump force
	float			velocityX;
	float			velocityY;
	float			speedMult;		// Applied by states

	bool			onGround;
	bool			facingRight;
	bool			knifeOnly;

	int				lives;
	int				grenadeCount;

	TransformationState* currentTransform;	// OWNED
	Weapon*			currentWeapon;			// OWNED (Composition)
	Weapon*			pistol;					// OWNED (Fallback)
};
