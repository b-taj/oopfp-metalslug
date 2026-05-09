#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "Enums.h"
#include "Weapon.h"
#include "Grenade.h"

class Vehicle;
class Weapon;
class Grenade;
class Enemy;

class Character
{
public:
	Character();
	Character(float startX, float startY);
	virtual ~Character();

	virtual void	update(float dt, float mouseX, float mouseY) = 0;
	virtual void	activateSpecialPower() = 0;
	virtual float	getFireRate() = 0;
	virtual int		getGrenadeDamage() = 0;

	void			setPosition(float newX, float newY);
	void			setSpeed(float accel, float maxSpd);
	void			setSpriteScale(float scaleX, float scaleY);
	void			loadTexture(const char* filename);

	virtual void	throwGrenade(float angle);
	virtual void	meleeAttack();
	virtual void	pickupWeapon(Weapon* weapon);
	virtual float	getMeleeRange();

	void			moveLeft();
	void			moveRight();
	void			moveUp();
	void			moveDown();
	void			updateMovement(float dt, float mouseX, float mouseY);
	void			resolveGround(char** lvl, int height, int width, int cellSize);
	void			jump();
	Projectile*		shoot(); 
	void			boardVehicle(Vehicle* vehicle);
	void			exitVehicle();

	void			takeDamage();
	void			takeDamage(int dmg);
	void			die();
	void			respawn();
	void			applyState(CharacterState newState);

	// Combat Actions
	bool			canMelee();
	void			performMelee();
	Projectile*		throwGrenade();

	void			draw(sf::RenderWindow& window, float camX, float camY);
	float			getX();
	float			getY();
	int				getHp();
	int				getGrenades();

protected:
	int				hp;
	int				maxHp;
	float			x;
	float			y;
	float			velocityX;
	float			velocityY;
	float			acceleration;
	float			maxSpeed;
	float			speed;
	float			gravity;
	float			terminalVelocity;
	float			jumpStrength;
	bool			onGround;
	int				lives;
	int				grenadeCount;
	Weapon*			currentWeapon;
	Pistol			pistol;
	Vehicle*		currentVehicle;
	CharacterState	state;
	float			stateTimer;
	int				damageLevel;
	sf::Sprite		sprite;
	sf::Texture	texture;
	bool			facingRight;
	bool			isJumping;
	float			aimAngle;
	float           walkTimer;

	// Combat internals
	float			meleeTimer;
	bool			wantsMelee;
	bool			wantsGrenade;
};

class PlayerMarco : public Character
{
public:
	PlayerMarco();
	PlayerMarco(float startX, float startY);
	void	update(float dt, float mouseX, float mouseY) override;
	void	activateSpecialPower() override;
	float	getFireRate() override;
	int		getGrenadeDamage() override;
	void	meleeAttack() override;
	void	throwGrenade(float angle) override;
	void	pickupWeapon(Weapon* weapon) override;
	float	getMeleeRange() override;

private:
	bool	canPierceShield;
};

class PlayerTarma : public Character
{
public:
	PlayerTarma();
	PlayerTarma(float startX, float startY);
	void	update(float dt, float mouseX, float mouseY) override;
	void	activateSpecialPower() override;
	float	getFireRate() override;
	int		getGrenadeDamage() override;
	float	getVehicleFireRate();
	float	getVehicleDurability();
	void	onVehicleDestroyed();
};

class PlayerEri : public Character
{
public:
	PlayerEri();
	PlayerEri(float startX, float startY);
	void	update(float dt, float mouseX, float mouseY) override;
	void	activateSpecialPower() override;
	void	throwGrenade(float angle) override;
	float	getFireRate() override;
	int		getGrenadeDamage() override;
};

class PlayerFio : public Character
{
public:
	PlayerFio();
	PlayerFio(float startX, float startY);
	void	update(float dt, float mouseX, float mouseY) override;
	void	activateSpecialPower() override;
	void	pickupWeapon(Weapon* weapon) override;
	float	getFireRate() override;
	int		getGrenadeDamage() override;
	float	getMeleeRange() override;
};
