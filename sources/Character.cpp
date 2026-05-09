#include "../headers/Character.h"
#include "../headers/Projectile.h"
#include <cmath>
#include <iostream>

/**
 * Character Base Class Implementation
 * 
 * Handles core physics, movement, and common combat mechanics (pistol, melee, grenades).
 */

// Default constructor initializing base stats and state
Character::Character()
	: hp(100), maxHp(100), x(0), y(0), velocityX(0), velocityY(0), speed(100),
	lives(2), grenadeCount(10), currentWeapon(&pistol), currentVehicle(nullptr),
	state(CharacterState::NORMAL), stateTimer(0), damageLevel(0), facingRight(true), isJumping(false),
	acceleration(2000.0f), maxSpeed(500.0f),
	gravity(2500.0f), terminalVelocity(1000.0f), jumpStrength(-1200.0f), onGround(false),
	aimAngle(0.0f), walkTimer(0.0f), meleeTimer(0.0f), wantsMelee(false), wantsGrenade(false)
{
}

// Parameterized constructor for specific spawn points
Character::Character(float startX, float startY)
	: hp(100), maxHp(100), x(startX), y(startY), velocityX(0), velocityY(0), speed(100),
	lives(2), grenadeCount(10), currentWeapon(&pistol), currentVehicle(nullptr),
	state(CharacterState::NORMAL), stateTimer(0), damageLevel(0), facingRight(true), isJumping(false),
	acceleration(2000.0f), maxSpeed(500.0f),
	gravity(2500.0f), terminalVelocity(1000.0f), jumpStrength(-1200.0f), onGround(false),
	aimAngle(0.0f), walkTimer(0.0f), meleeTimer(0.0f), wantsMelee(false), wantsGrenade(false)
{
}

// Virtual destructor for proper cleanup of derived classes
Character::~Character()
{
}

// Default implementation of weapon pickup logic
void Character::pickupWeapon(Weapon* weapon)
{
	currentWeapon = weapon;
}

// Default melee attack logic (to be overridden if needed)
void Character::meleeAttack()
{
}

// Default grenade throwing logic
void Character::throwGrenade(float angle)
{
	(void)angle;
}

// Returns the distance required for a melee strike to connect
float Character::getMeleeRange()
{
	return 100.0f;
}

// Stub for leftward movement (logic handled in updateMovement)
void Character::moveLeft()
{
}

// Stub for rightward movement (logic handled in updateMovement)
void Character::moveRight()
{
}

// Stub for upward movement (logic handled in updateMovement)
void Character::moveUp()
{
}

// Stub for downward movement (logic handled in updateMovement)
void Character::moveDown()
{
}

// Handles input polling and physics integration for character movement
// Updates velocities, applies friction, and calculates aiming angles
void Character::updateMovement(float dt, float mouseX, float mouseY)
{
	// Horizontal Input
	float moveInputX = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) moveInputX = -1.0f;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) moveInputX = 1.0f;

	// Jump Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && onGround) { 
		velocityY = jumpStrength; 
		onGround = false; 
	}

	// Melee & Grenade Input (V for Melee, G for Grenade)
	if (meleeTimer > 0) meleeTimer -= dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::V) && canMelee()) {
		wantsMelee = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
		wantsGrenade = true;
	}

	// Physics: Velocity and Acceleration
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

	// Aiming Logic (Mouse relative to screen center assumed 800,450)
	float dx = mouseX - 800.0f;
	float dy = mouseY - 450.0f;
	float rawAngle = std::atan2(dy, dx) * 180.0f / 3.14159f;

	// Restrict facing direction and aim angle based on mouse
	if (mouseX > 800.0f) {
		facingRight = true;
		if (rawAngle > 45.0f) aimAngle = 45.0f;
		else if (rawAngle < -90.0f) aimAngle = -90.0f;
		else aimAngle = rawAngle;
	} else {
		facingRight = false;
		if (rawAngle > 135.0f || rawAngle < -180.0f) aimAngle = 180.0f;
		else if (rawAngle < 0.0f && rawAngle > -90.0f) aimAngle = -90.0f;
		else aimAngle = rawAngle;
	}

	// Animation: Walking Bob
	if (std::abs(velocityX) > 10.0f && onGround) {
		walkTimer += dt * 15.0f;
	} else { walkTimer = 0.0f; }

	// Sprite Mirroring
	sprite.setOrigin(261.0f, 235.0f);
	if (facingRight) sprite.setScale(0.2f, 0.2f);
	else sprite.setScale(-0.2f, 0.2f);

	// Weapon Cooldown update
	if (currentWeapon) currentWeapon->update(dt);
}

// Helper to determine if a grid character represents a solid collision tile
static bool isSolidTile(char tile)
{
	return (tile == 'B' || tile == 'W' || tile == '#' || 
			tile == 'M' || tile == 'P' || tile == 'D' || tile == 'A' || tile == 'g');
}

// Performs collision detection against the level grid
// Snaps character to ground and resets vertical velocity
void Character::resolveGround(char** lvl, int height, int width, int cellSize)
{
	onGround = false;
	if (y > 2500.0f) { y = 0; velocityY = 0; } // Fall-pit reset
	
	// Feet position relative to origin
	float feetY = y + 47.0f; 
	int tileY = static_cast<int>(feetY) / cellSize;
	
	// Sample points at the base
	int tileXLeft = static_cast<int>(x - 10.0f) / cellSize;
	int tileXRight = static_cast<int>(x + 10.0f) / cellSize;

	if (tileY >= 0 && tileY < height) {
		char leftTile = (tileXLeft >= 0 && tileXLeft < width) ? lvl[tileY][tileXLeft] : ' ';
		char rightTile = (tileXRight >= 0 && tileXRight < width) ? lvl[tileY][tileXRight] : ' ';

		if ((isSolidTile(leftTile) || isSolidTile(rightTile)) && velocityY >= 0) {
			y = static_cast<float>(tileY * cellSize - 47.0f);
			velocityY = 0; 
			onGround = true;
		}
	}
}

// Logic for firing the current weapon
// Handles automatic reversion to pistol when special ammo is depleted
Projectile* Character::shoot()
{
	// 1. Check if current special weapon is out of ammo
	if (currentWeapon != &pistol && currentWeapon->isOutOfAmmo()) {
		std::cout << "[Combat] Special weapon empty. Reverting to Pistol." << std::endl;
		currentWeapon = &pistol;
	}

	// 2. Fire the active weapon
	if (currentWeapon) {
		float rad = aimAngle * 3.14159f / 180.0f;
		float fireX = x + std::cos(rad) * 40.0f;
		float fireY = y + std::sin(rad) * 40.0f;
		return currentWeapon->fire(fireX, fireY, aimAngle);
	}
	return nullptr;
}

// Checks if the melee attack is ready (off cooldown)
bool Character::canMelee() { return meleeTimer <= 0; }

// Triggers the melee strike logic and starts the cooldown timer
void Character::performMelee() 
{ 
	meleeTimer = 0.5f; // 0.5s Cooldown as per requirements
	wantsMelee = false;
	std::cout << "[Combat] Knife attack triggered (2 HP)!" << std::endl;
}

// Spawns a grenade projectile with an upward arc
// Consumes one unit of grenade ammunition
Projectile* Character::throwGrenade()
{
	if (grenadeCount > 0) {
		grenadeCount--;
		wantsGrenade = false;
		
		Projectile* g = new Projectile();
		g->x = x; g->y = y;
		
		float rad = aimAngle * 3.14159f / 180.0f;
		g->velX = std::cos(rad) * 600.0f;
		g->velY = std::sin(rad) * 600.0f - 200.0f; // Slight upward arc
		
		g->damage = 20; // 20 HP as per requirements
		g->isAlive = true;
		g->isEnemyProjectile = false;
		g->type = ProjectileType::BALLISTIC; // Using Ballistic as placeholder for explosive behavior
		
		g->loadTexture("Sprites/blocks/stone.png"); // Placeholder texture
		g->setTextureRect(0, 0, 24, 24);
		g->setPosition(x, y);
		
		std::cout << "[Combat] Grenade thrown! Remaining: " << grenadeCount << std::endl;
		return g;
	}
	wantsGrenade = false;
	return nullptr;
}

// Renders the character and applies a bobbing animation while walking
void Character::draw(sf::RenderWindow& window, float camX, float camY) 
{ 
	float bob = 0.0f;
	if (std::abs(velocityX) > 10.0f && onGround) bob = std::sin(walkTimer) * 10.0f;

	sprite.setPosition(x - camX, y - camY + bob);
	window.draw(sprite); 
}

// Initiates a jump if the character is currently on solid ground
void Character::jump() { if (onGround) { velocityY = jumpStrength; onGround = false; } }

// Aggregates the character into a vehicle (not fully implemented)
void Character::boardVehicle(Vehicle* vehicle) { (void)vehicle; }

// Removes the character from their current vehicle
void Character::exitVehicle() {}

// Standard damage reduction logic (20 HP per hit)
void Character::takeDamage() { hp -= 20; if(hp < 0) hp = 0; }

// Variable damage reduction logic
void Character::takeDamage(int dmg) { hp -= dmg; if(hp < 0) hp = 0; }

// Sets character health to zero immediately
void Character::die() { hp = 0; }

// Resets character health to full capacity
void Character::respawn() { hp = maxHp; }

// Transitions character into a new behavioral or visual state (Mummy, Zombie, etc.)
void Character::applyState(CharacterState newState) { (void)newState; }

// World-space positioning setter
void Character::setPosition(float newX, float newY) { x = newX; y = newY; }

// Configures the physics parameters for character movement
void Character::setSpeed(float accel, float maxSpd) { acceleration = accel; maxSpeed = maxSpd; }

// Adjusts the visual scale of the character sprite
void Character::setSpriteScale(float scaleX, float scaleY) { sprite.setScale(scaleX, scaleY); }

// Returns current world-X coordinate
float Character::getX() { return x; }

// Returns current world-Y coordinate
float Character::getY() { return y; }

// Returns current health points
int Character::getHp() { return hp; }

// Returns remaining grenade count
int Character::getGrenades() { return grenadeCount; }

// Loads the character texture and assigns it to the sprite
void Character::loadTexture(const char* filename)
{
	(void)filename; 
	texture.loadFromFile("Sprites/Character.png");
	sprite.setTexture(texture);
}

// ==================== MARCO ROSSI ====================
PlayerMarco::PlayerMarco() : Character() { grenadeCount = 10; }
PlayerMarco::PlayerMarco(float startX, float startY) : Character(startX, startY) { grenadeCount = 10; }
void PlayerMarco::update(float dt, float mx, float my) { updateMovement(dt, mx, my); }
void PlayerMarco::activateSpecialPower() {}
float PlayerMarco::getFireRate() { return 1.25f; }
int PlayerMarco::getGrenadeDamage() { return 20; }
void PlayerMarco::meleeAttack() {}
void PlayerMarco::throwGrenade(float angle) { (void)angle; }
void PlayerMarco::pickupWeapon(Weapon* weapon) { currentWeapon = weapon; }
float PlayerMarco::getMeleeRange() { return 50.0f; }

// ==================== TARMA ROVING ====================
PlayerTarma::PlayerTarma() : Character() { hp = 80; maxHp = 80; }
PlayerTarma::PlayerTarma(float startX, float startY) : Character(startX, startY) { hp = 80; maxHp = 80; }
void PlayerTarma::update(float dt, float mx, float my) { updateMovement(dt, mx, my); }
void PlayerTarma::activateSpecialPower() {}
float PlayerTarma::getFireRate() { return 1.0f; }
int PlayerTarma::getGrenadeDamage() { return 20; }
float PlayerTarma::getVehicleFireRate() { return 1.5f; }
float PlayerTarma::getVehicleDurability() { return 1.5f; }
void PlayerTarma::onVehicleDestroyed() {}

// ==================== ERI KASAMOTO ====================
PlayerEri::PlayerEri() : Character() { grenadeCount = 20; }
PlayerEri::PlayerEri(float startX, float startY) : Character(startX, startY) { grenadeCount = 20; }
void PlayerEri::update(float dt, float mx, float my) { updateMovement(dt, mx, my); }
void PlayerEri::activateSpecialPower() {}
void PlayerEri::throwGrenade(float angle) { (void)angle; }
float PlayerEri::getFireRate() { return 0.80f; }
int PlayerEri::getGrenadeDamage() { return 20; }

// ==================== FIO GERMI ====================
PlayerFio::PlayerFio() : Character() { grenadeCount = 10; }
PlayerFio::PlayerFio(float startX, float startY) : Character(startX, startY) { grenadeCount = 10; }
void PlayerFio::update(float dt, float mx, float my) { updateMovement(dt, mx, my); }
void PlayerFio::activateSpecialPower() {}
void PlayerFio::pickupWeapon(Weapon* weapon) { currentWeapon = weapon; }
float PlayerFio::getFireRate() { return 1.10f; }
int PlayerFio::getGrenadeDamage() { return 20; }
float PlayerFio::getMeleeRange() { return 50.0f; }
