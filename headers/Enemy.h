#pragma once
#include "DamagableEntity.h"
#include "EnemyAIState.h"
#include "Projectile.h"
#include "SpriteAnimator.h"
#include "SoundManager.h"
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

	virtual void	onDeath() = 0; 
	
	void			update(float dt) override; 
	virtual void	update(float dt, class Soldier* player); 
	
	void			draw(sf::RenderWindow& window, float camOX, float camOY) override;
	void			takeDamage(int dmg) override;
	void			die() override;

	void			setAIState(EnemyAIState* next);
	void			activateGrudge();
	
	virtual class Projectile* fireWeapon(class Soldier* target);

	void			loadTexture(const char* path);
	void			setSoundManager(class SoundManager* sm);
	int				getScoreValue() const;

	void			setVelocityX(float vx);
	void			setVelocityY(float vy);
	void			setFacingRight(bool right);
	virtual void	shootPlayer(class Soldier* player);

protected:
	Enemy();

	SpriteAnimator	animator;
	EnemyAIState*	currentAI;		
	Weapon*			weapon;			
	
	sf::Texture		texture;
	sf::Sprite		sprite;

	float			aggroRange;
	int				scoreValue;
	bool			isGrudge;
	float			grudgePowerMult;

	float			velocityX;
	float			velocityY;
	bool			facingRight;

	float			hitFlashTimer;
	class SoundManager* soundManager;	
};
