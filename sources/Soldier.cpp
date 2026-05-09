#include "../headers/Soldier.h"
#include "../headers/NormalState.h"
#include "../headers/Projectile.h"
#include "../headers/Constants.h"
#include <iostream>

Soldier::Soldier()
	: speedX(200.0f), speedY(JUMP_FORCE), velocityX(0.0f), velocityY(0.0f), speedMult(1.0f),
	  onGround(false), facingRight(true), knifeOnly(false), lives(2), grenadeCount(10),
	  currentWeapon(nullptr)
{
	active = true;
	width = 50.0f;
	height = 50.0f;
	hp = 100;
	maxHp = 100;
	damageLevel = 0;

	pistol = new Pistol(); // Default weapon
	currentWeapon = pistol;
	
	currentTransform = new NormalState();
	// Note: onEnter(this) called during first setTransformationState or initialization
}

Soldier::~Soldier()
{
	if (currentTransform) delete currentTransform;
	if (pistol) delete pistol;
	if (currentWeapon != pistol) delete currentWeapon;
}

void Soldier::update(float dt)
{
	if (!active) return;

	// Call current state logic (Zombie/Mummy timers, etc.)
	if (currentTransform) currentTransform->update(this, dt);

	// Basic Physics
	if (!onGround) {
		velocityY += GRAVITY * dt;
	}

	x += velocityX * dt;
	y += velocityY * dt;

	// Reset horiz velocity for next frame (input will re-set it)
	velocityX = 0.0f;
}

void Soldier::draw(sf::RenderWindow& window, float camOffsetX, float camOffsetY)
{
	if (!active) return;
	sprite.setPosition(x - camOffsetX, y - camOffsetY);
	
	// Handle mirroring
	if (facingRight) sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
	else sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);

	window.draw(sprite);
}

void Soldier::takeDamage(int dmg)
{
	hp -= dmg;
	if (hp <= 0) die();
	else {
		float ratio = (float)hp / maxHp;
		if (ratio < 0.3f) damageLevel = 2;
		else if (ratio < 0.6f) damageLevel = 1;
	}
}

void Soldier::die()
{
	hp = 0;
	lives--;
	if (lives < 0) {
		active = false;
	} else {
		// Respawn logic or death animation trigger
		hp = maxHp;
		damageLevel = 0;
	}
}

void Soldier::setTransformationState(TransformationState* next)
{
	if (!next) return;
	if (currentTransform) {
		currentTransform->onExit(this);
		delete currentTransform;
	}
	currentTransform = next;
	currentTransform->onEnter(this);
}

void Soldier::forceKnifeOnly(bool f) { knifeOnly = f; }

void Soldier::setSpeedMultiplier(float m) { speedMult = m; }

void Soldier::loadTexture(const char* path) { texture.loadFromFile(path); sprite.setTexture(texture); }

void Soldier::setSpriteScale(float sx, float sy) { sprite.setScale(sx, sy); }

void Soldier::setSpeed(float spd, float jumpForce) { speedX = spd; speedY = jumpForce; }

int Soldier::getLives() const { return lives; }
int Soldier::getGrenadeCount() const { return grenadeCount; }
int Soldier::getHp() const { return hp; }

void Soldier::resolveGround(char** grid, int h, int w, int cell)
{
	onGround = false;
	int tileY = static_cast<int>(y + height) / cell;
	int tileX = static_cast<int>(x + width/2.0f) / cell;

	if (tileY >= 0 && tileY < h && tileX >= 0 && tileX < w) {
		if (grid[tileY][tileX] != ' ') {
			y = (float)(tileY * cell - height);
			velocityY = 0;
			onGround = true;
		}
	}
}

Projectile* Soldier::shoot(float angle)
{
	if (knifeOnly) return nullptr; // Melee weapons fire returns nullptr

	if (currentWeapon && currentWeapon->canFire()) {
		return currentWeapon->fire(x, y, angle);
	}

	if (pistol && pistol->canFire()) {
		return pistol->fire(x, y, angle);
	}

	return nullptr;
}
