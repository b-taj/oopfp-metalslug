#include "../headers/Enemy.h"

void Enemy::activateGrudge() {}
int Enemy::getScore() { return scoreValue; }
void Enemy::draw(sf::RenderWindow& window) { if (isAlive) window.draw(sprite); }
void Enemy::setPosition(float nx, float ny) { x = nx; y = ny; sprite.setPosition(x, y); }

void Enemy::loadTexture(const char* filename)
{
	if (texture.loadFromFile(filename)) {
		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 0, 50, 50)); 
	}
}

// ==================== REBEL SOLDIER ====================

RebelSoldier::RebelSoldier(float sx, float sy)
{
	// Initialize members directly in body as they are protected in Enemy
	x = sx;
	y = sy;
	hp = 20; 
	speed = 100.0f;
	isAlive = true;
	facingRight = true;
	scoreValue = 50;
	grudgePowerMult = 1.0f;
	isGrudge = false;

	loadTexture("Sprites/Enemies/green.png"); 
	sprite.setScale(1.5f, 1.5f);
	sprite.setPosition(x, y);
}

void RebelSoldier::update(float dt)
{
	if (!isAlive) return;

	float patrolRange = 100.0f;
	// Use a fixed start point for patrolling relative to spawn
	static float startX = x; 
	
	if (facingRight) {
		x += speed * dt;
		if (x > startX + patrolRange) {
			facingRight = false;
		}
	} else {
		x -= speed * dt;
		if (x < startX - patrolRange) {
			facingRight = true;
		}
	}
	sprite.setPosition(x, y);
}

void RebelSoldier::takeDamage(int dmg)
{
	hp -= dmg;
	if (hp <= 0) die();
}

void RebelSoldier::attack(Character* target) { (void)target; }

void RebelSoldier::die()
{
	isAlive = false;
}
