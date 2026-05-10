#include "../headers/Enemy.h"
#include "../headers/Soldier.h"
#include "../headers/PatrolState.h"
#include "../headers/DeadAIState.h"
#include "../headers/Constants.h"
#include <cmath>

Enemy::Enemy() 
	: currentAI(nullptr), weapon(nullptr), aggroRange(400.0f), scoreValue(0),
	  isGrudge(false), grudgePowerMult(1.5f), velocityX(0.0f), velocityY(0.0f), facingRight(true),
	  hitFlashTimer(0.0f), soundManager(nullptr)
{
	active = true;
	currentAI = new PatrolState();
}

Enemy::~Enemy()
{
	if (currentAI) delete currentAI;
	if (weapon) delete weapon;
}

void Enemy::update(float dt) { (void)dt; }

void Enemy::update(float dt, class Soldier* player)
{
	if (!active) {
		animator.update(dt);
		animator.applyToSprite(sprite);
		return;
	}

	// Physics
	velocityY += GRAVITY * dt;
	x += velocityX * dt;
	y += velocityY * dt;

	if (currentAI) currentAI->update(this, dt, player);
	if (weapon) weapon->update(dt);

	if (hitFlashTimer > 0.0f) {
		hitFlashTimer -= dt;
		sprite.setColor(sf::Color(255, 80, 80, 255));
	} else {
		sprite.setColor(sf::Color::White);
	}

	animator.update(dt);
	animator.applyToSprite(sprite);
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
	hp -= isGrudge ? (int)(dmg * 0.5f) : dmg;
	hitFlashTimer = 0.1f;
	if (hp <= 0) die();
}

void Enemy::die()
{
	hp = 0;
	onDeath();
	if (soundManager) soundManager->play("enemy_die");
	animator.play("die");
	setAIState(new DeadAIState());
	active = false;
}

Projectile* Enemy::fireWeapon(Soldier* target)
{
	if (!weapon || !weapon->canFire()) return nullptr;
	float dx = target->getX() - x;
	float dy = target->getY() - y;
	float angle = std::atan2(dy, dx) * 180.0f / 3.14159f;
	return weapon->fire(x, y, angle, soundManager);
}

void Enemy::activateGrudge()
{
	isGrudge = true;
	hp = maxHp * 2;
}

void Enemy::loadTexture(const char* path) { texture.loadFromFile(path); sprite.setTexture(texture); }
void Enemy::setSoundManager(SoundManager* sm) { soundManager = sm; }
int Enemy::getScoreValue() const { return scoreValue; }
void Enemy::setVelocityX(float vx) { velocityX = vx; }
void Enemy::setVelocityY(float vy) { velocityY = vy; }
void Enemy::setFacingRight(bool right) { facingRight = right; }

void Enemy::shootPlayer(class Soldier* player) { (void)player; }

void Enemy::draw(sf::RenderWindow& window, float camOX, float camOY)
{
	sprite.setPosition(x - camOX, y - camOY);
	if (facingRight) sprite.setScale(std::abs(sprite.getScale().x), sprite.getScale().y);
	else sprite.setScale(-std::abs(sprite.getScale().x), sprite.getScale().y);
	window.draw(sprite);
}
