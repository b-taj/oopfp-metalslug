#include "../headers/Character.h"
#include <cmath>

Character::Character()
	: hp(100), maxHp(100), x(0), y(0), velocityX(0), velocityY(0), speed(100),
	lives(2), grenadeCount(10), currentWeapon(&pistol), currentVehicle(nullptr),
	state(CharacterState::NORMAL), stateTimer(0), damageLevel(0), facingRight(true), isJumping(false),
	acceleration(2000.0f), maxSpeed(500.0f),
	gravity(2500.0f), terminalVelocity(1000.0f), jumpStrength(-1200.0f), onGround(false),
	aimAngle(0.0f), walkTimer(0.0f)
{
}

Character::Character(float startX, float startY)
	: hp(100), maxHp(100), x(startX), y(startY), velocityX(0), velocityY(0), speed(100),
	lives(2), grenadeCount(10), currentWeapon(&pistol), currentVehicle(nullptr),
	state(CharacterState::NORMAL), stateTimer(0), damageLevel(0), facingRight(true), isJumping(false),
	acceleration(2000.0f), maxSpeed(500.0f),
	gravity(2500.0f), terminalVelocity(1000.0f), jumpStrength(-1200.0f), onGround(false),
	aimAngle(0.0f), walkTimer(0.0f)
{
}

void Character::updateMovement(float dt)
{
	float moveInputX = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { moveInputX = -1.0f; facingRight = false; }
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { moveInputX = 1.0f; facingRight = true; }

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && onGround) { velocityY = jumpStrength; onGround = false; }

	float targetVelX = moveInputX * maxSpeed;
	float diffX = targetVelX - velocityX;
	velocityX += diffX * 10.0f * dt;
	if (std::abs(velocityX) < 10.0f && moveInputX == 0) velocityX = 0.0f;

	if (!onGround) {
		velocityY += gravity * dt;
		if (velocityY > terminalVelocity) velocityY = terminalVelocity;
	}
	x += velocityX * dt;
	y += velocityY * dt;

	float bob = 0.0f;
	if (std::abs(velocityX) > 10.0f && onGround) {
		walkTimer += dt * 15.0f;
		bob = std::sin(walkTimer) * 10.0f;
	} else { walkTimer = 0.0f; }

	sprite.setOrigin(261.0f, 235.0f);
	if (facingRight) sprite.setScale(0.2f, 0.2f);
	else sprite.setScale(-0.2f, 0.2f);

	float adx = 0.0f, ady = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ady -= 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ady += 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) adx -= 1.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) adx += 1.0f;
	if (adx == 0.0f && ady == 0.0f) adx = facingRight ? 1.0f : -1.0f;

	float targetAngle = std::atan2(ady, adx) * 180.0f / 3.14159265f;
	float angleDiff = targetAngle - aimAngle;
	while (angleDiff > 180.0f) angleDiff -= 360.0f;
	while (angleDiff < -180.0f) angleDiff += 360.0f;
	if (std::abs(angleDiff) < 540.0f * dt) aimAngle = targetAngle;
	else aimAngle += (angleDiff > 0 ? 1 : -1) * 540.0f * dt;

	if (currentWeapon) currentWeapon->update(dt);
	sprite.setPosition(x, y + bob);
}

void Character::resolveGround(char** lvl, int height, int width, int cellSize)
{
	onGround = false;
	if (y > height * cellSize) { y = 0; velocityY = 0; }
	float feetY = y + 47.0f; 
	int tileY = static_cast<int>(feetY) / cellSize;
	int tileXLeft = static_cast<int>(x - 20.0f) / cellSize;
	int tileXRight = static_cast<int>(x + 20.0f) / cellSize;

	if (tileY >= 0 && tileY < height) {
		if (tileXLeft >= 0 && tileXLeft < width && tileXRight >= 0 && tileXRight < width) {
			char leftTile = lvl[tileY][tileXLeft], rightTile = lvl[tileY][tileXRight];
			if ((leftTile == 'B' || leftTile == 'W' || leftTile == '#') && velocityY >= 0) {
				y = static_cast<float>(tileY * cellSize - 47.0f);
				velocityY = 0; onGround = true;
			}
		}
	}
}

Projectile* Character::shoot(float angle)
{
	if (currentWeapon) {
		float rad = aimAngle * 3.14159265f / 180.0f;
		float fireX = x + std::cos(rad) * 40.0f;
		float fireY = y + std::sin(rad) * 40.0f;
		
		// Fire rate modifier is handled in getFireRate override
		return currentWeapon->fire(fireX, fireY, aimAngle);
	}
	return nullptr;
}

void Character::jump() { if (onGround) { velocityY = jumpStrength; onGround = false; } }
void Character::boardVehicle(Vehicle* vehicle) { (void)vehicle; }
void Character::exitVehicle() {}
void Character::takeDamage() { hp -= 20; if(hp < 0) hp = 0; }
void Character::die() { hp = 0; }
void Character::respawn() { hp = maxHp; }
void Character::applyState(CharacterState newState) { (void)newState; }
void Character::draw(sf::RenderWindow& window) { window.draw(sprite); }
void Character::setPosition(float newX, float newY) { x = newX; y = newY; sprite.setPosition(x, y); }
void Character::setSpeed(float accel, float maxSpd) { acceleration = accel; maxSpeed = maxSpd; }
void Character::setSpriteScale(float scaleX, float scaleY) { sprite.setScale(scaleX, scaleY); }

void Character::loadTexture(const char* filename)
{
	(void)filename; 
	texture.loadFromFile("Sprites/Character.png");
	sprite.setTexture(texture);
}

// ==================== PLAYER MARCO ====================
PlayerMarco::PlayerMarco() : Character() { 
	grenadeCount = 8; 
	canPierceShield = true; 
}
PlayerMarco::PlayerMarco(float startX, float startY) : Character(startX, startY) { 
	grenadeCount = 8; 
	canPierceShield = true;
}
void PlayerMarco::update(float dt) { updateMovement(dt); }
void PlayerMarco::activateSpecialPower() {}
float PlayerMarco::getFireRate() { return 1.25f; } // +25%
int PlayerMarco::getGrenadeDamage() { return 20; }
void PlayerMarco::meleeAttack() {}
void PlayerMarco::throwGrenade(float angle) { (void)angle; }
void PlayerMarco::pickupWeapon(Weapon* weapon) { (void)weapon; }
float PlayerMarco::getMeleeRange() { return 100.0f; }

// ==================== PLAYER TARMA ====================
PlayerTarma::PlayerTarma() : Character() { 
	hp = 80; maxHp = 80; // -20% HP on foot
	maxSpeed = 400.0f;   // -20% Speed (Base was 500)
}
PlayerTarma::PlayerTarma(float startX, float startY) : Character(startX, startY) { 
	hp = 80; maxHp = 80; 
	maxSpeed = 400.0f;
}
void PlayerTarma::update(float dt) { updateMovement(dt); }
void PlayerTarma::activateSpecialPower() {}
float PlayerTarma::getFireRate() { return 1.0f; }
int PlayerTarma::getGrenadeDamage() { return 20; }
float PlayerTarma::getVehicleFireRate() { return 1.25f; }
float PlayerTarma::getVehicleDurability() { return 1.20f; }
void PlayerTarma::onVehicleDestroyed() {}

// ==================== PLAYER ERI ====================
PlayerEri::PlayerEri() : Character() { 
	grenadeCount = 20; // Double grenades
}
PlayerEri::PlayerEri(float startX, float startY) : Character(startX, startY) { 
	grenadeCount = 20; 
}
void PlayerEri::update(float dt) { updateMovement(dt); }
void PlayerEri::activateSpecialPower() {}
void PlayerEri::throwGrenade(float angle) { (void)angle; }
float PlayerEri::getFireRate() { return 0.80f; } // -20%
int PlayerEri::getGrenadeDamage() { return 30; } // Larger blast radius simulation

// ==================== PLAYER FIO ====================
PlayerFio::PlayerFio() : Character() { 
	grenadeCount = 8; // -2 grenades
}
PlayerFio::PlayerFio(float startX, float startY) : Character(startX, startY) { 
	grenadeCount = 8; 
}
void PlayerFio::update(float dt) { updateMovement(dt); }
void PlayerFio::activateSpecialPower() {}
void PlayerFio::pickupWeapon(Weapon* weapon) { (void)weapon; }
float PlayerFio::getFireRate() { return 1.10f; } // +10%
int PlayerFio::getGrenadeDamage() { return 20; }
float PlayerFio::getMeleeRange() { return 75.0f; } // Lower melee dmg handled via range
