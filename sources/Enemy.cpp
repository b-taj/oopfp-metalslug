#include "../headers/Enemy.h"
#include "../headers/PatrolState.h"
#include "../headers/DeadAIState.h"
#include "../headers/Constants.h"

Enemy::Enemy() 
	: currentAI(nullptr), weapon(nullptr), aggroRange(400.0f), scoreValue(0),
	  isGrudge(false), grudgePowerMult(1.5f), velocityX(0.0f), velocityY(0.0f), facingRight(true)
{
	active = true;
	currentAI = new PatrolState(); // UML requirement
}

Enemy::~Enemy()
{
	if (currentAI) delete currentAI;
	if (weapon) delete weapon;
}

void Enemy::update(float dt) { (void)dt; }

void Enemy::update(float dt, class Soldier* player)
{
	if (!active) return;

	// Physics
	velocityY += GRAVITY * dt;
	x += velocityX * dt;
	y += velocityY * dt;

	if (currentAI) currentAI->update(this, dt, player);
	if (weapon) weapon->update(dt);
}

void Enemy::setAIState(EnemyAIState* next)
{
	if (!next) return;
	if (currentAI) {
		currentAI->onExit(this);
		delete currentAI;
	}
	currentAI = next;
	currentAI->onEnter(this);
}

void Enemy::takeDamage(int dmg)
{
	hp -= isGrudge ? (int)(dmg * 0.5f) : dmg; // Grudge enemies are tankier
	if (hp <= 0) die();
}

void Enemy::die()
{
	hp = 0;
	onDeath();
	setAIState(new DeadAIState());
	active = false;
}

void Enemy::activateGrudge()
{
	isGrudge = true;
	hp = maxHp * 2;
}

void Enemy::loadTexture(const char* path) { texture.loadFromFile(path); sprite.setTexture(texture); }
int Enemy::getScoreValue() const { return scoreValue; }
void Enemy::setVelocityX(float vx) { velocityX = vx; }
void Enemy::setVelocityY(float vy) { velocityY = vy; }
void Enemy::setFacingRight(bool right) { facingRight = right; }

void Enemy::shootPlayer(class Soldier* player)
{
	if (weapon && weapon->canFire()) {
		// Calculate angle to player
		// Projectile* p = weapon->fire(x, y, angle);
		// Add to Level... (implemented in EntityManager)
	}
}

void Enemy::draw(sf::RenderWindow& window, float camOX, float camOY)
{
	if (!active) return;
	sprite.setPosition(x - camOX, y - camOY);
	if (facingRight) sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
	else sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
	window.draw(sprite);
}
