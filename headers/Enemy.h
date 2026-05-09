#pragma once
#include "DamagableEntity.h"
#include "EnemyAIState.h"
#include "Weapon.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

/**
 * Enemy.h -- Base class for all hostile units.
 * Inherits from DamagableEntity. Uses the AI State Pattern.
 */

class Enemy : public DamagableEntity
{
public:
	virtual ~Enemy();

	virtual void	onDeath() = 0; // Trigger drops, sounds, etc.
	
	void			update(float dt) override; // Empty Entity override
	virtual void	update(float dt, class Soldier* player); // Main AI update
	
	void			draw(sf::RenderWindow& window, float camOX, float camOY) override;
	void			takeDamage(int dmg) override;
	void			die() override;

	void			setAIState(EnemyAIState* next);
	void			activateGrudge();
	
	void			loadTexture(const char* path);
	int				getScoreValue() const;

	// Accessors for states
	void			setVelocityX(float vx);
	void			setVelocityY(float vy);
	void			setFacingRight(bool right);
	virtual void	shootPlayer(class Soldier* player);

protected:
	Enemy();

	EnemyAIState*	currentAI;		// OWNED
	Weapon*			weapon;			// OWNED, nullable
	
	sf::Texture		texture;
	sf::Sprite		sprite;

	float			aggroRange;
	int				scoreValue;
	bool			isGrudge;
	float			grudgePowerMult;

	float			velocityX;
	float			velocityY;
	bool			facingRight;
};
