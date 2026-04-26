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
	virtual ~Character() = default;

	virtual void	update(float dt) = 0;
	virtual void	activateSpecialPower() = 0;
	virtual float	getFireRate() = 0;
	virtual int		getGrenadeDamage() = 0;

	void			setPosition(float newX, float newY);
	void			setSpeed(float accel, float maxSpd);
	void			setSpriteScale(float scaleX, float scaleY);
	void			loadTexture(const char* filename);

	virtual void	throwGrenade(float angle) {}
	virtual void	meleeAttack() {}
	virtual void	pickupWeapon(Weapon* weapon) {}
	virtual float	getMeleeRange() { return 100.0f; }

	void			moveLeft();
	void			moveRight();
	void			moveUp();
	void			moveDown();
	void			updateMovement(float dt);
	void			resolveGround(char** lvl, int height, int width, int cellSize);
	void			jump();
	Projectile*		shoot(float angle);
	void			boardVehicle(Vehicle* vehicle);
	void			exitVehicle();

	void			takeDamage();
	void			die();
	void			respawn();
	void			applyState(CharacterState newState);

	void			draw(sf::RenderWindow& window);
	float			getX() { return x; }
	float			getY() { return y; }
	int				getHp() { return hp; }

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
	sf::Texture		texture;
	bool			facingRight;
	bool			isJumping;
	float			aimAngle;

	// Animation Timer for procedural bobbing
	float           walkTimer;
};

class PlayerMarco : public Character
{
public:
	PlayerMarco();
	PlayerMarco(float startX, float startY);
	void	update(float dt) override;
	void	activateSpecialPower() override;
	float	getFireRate() override;
	int		getGrenadeDamage() override;
	void	meleeAttack() override;
	void	throwGrenade(float angle) override;
	void	pickupWeapon(Weapon* weapon) override;
	float	getMeleeRange() override;

private:
	bool	dualFireActive;
	float	dualFireTimer;
	bool	canPierceShield;
};

class PlayerTarma : public Character
{
public:
	PlayerTarma();
	PlayerTarma(float startX, float startY);
	void	update(float dt) override;
	void	activateSpecialPower() override;
	float	getFireRate() override;
	int		getGrenadeDamage() override;
	float	getVehicleFireRate();
	float	getVehicleDurability();
	void	onVehicleDestroyed();

private:
	bool	immunityActive;
	float	immunityTimer;
};

class PlayerEri : public Character
{
public:
	PlayerEri();
	PlayerEri(float startX, float startY);
	void	update(float dt) override;
	void	activateSpecialPower() override;
	void	throwGrenade(float angle) override;
	float	getFireRate() override;
	int		getGrenadeDamage() override;

private:
	bool	doubleGrenadeActive;
	float	doubleGrenadeTimer;
	float	blastRadius;
};

class PlayerFio : public Character
{
public:
	PlayerFio();
	PlayerFio(float startX, float startY);
	void	update(float dt) override;
	void	activateSpecialPower() override;
	void	pickupWeapon(Weapon* weapon) override;
	float	getFireRate() override;
	int		getGrenadeDamage() override;
	float	getMeleeRange() override;

private:
	bool	superchargeActive;
	float	superchargeTimer;
};
