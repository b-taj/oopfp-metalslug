#include "../headers/Soldier.h"
#include "../headers/NormalState.h"
#include "../headers/Projectile.h"
#include "../headers/BallisticProjectile.h"
#include "../headers/Constants.h"
#include <iostream>
#include <cmath>

Soldier::Soldier()
	: speedX(200.0f), speedY(JUMP_FORCE), velocityX(0.0f), velocityY(0.0f), speedMult(1.0f),
	  accel(1800.0f), friction(2400.0f), maxSpeedX(280.0f),
	  onGround(false), facingRight(true), jumpWasPressed(false), knifeOnly(false), 
	  lives(2), grenadeCount(10), currentWeapon(nullptr), soundManager(nullptr)
{
	active = true;
	width = 50.0f;
	height = 50.0f;
	hp = 100;
	maxHp = 100;
	damageLevel = 0;
	
	currentTransform = new NormalState();
}

Soldier::~Soldier()
{
	if (currentTransform) delete currentTransform;
	if (currentWeapon) delete currentWeapon;
}

void Soldier::update(float dt)
{
	if (!active) return;

	if (currentTransform) currentTransform->update(this, dt);

	if (!onGround) {
		velocityY += GRAVITY * dt;
	}

	x += velocityX * dt;
	y += velocityY * dt;

	// ANIMATION TRANSITION RULES
	if (!animator.isFinished() && !animator.getCurrentAnimationLoops()) 
		goto apply_anim;

	if (hp <= 0) { animator.play("die"); goto apply_anim; }
	if (!onGround) { animator.play("jump"); goto apply_anim; }
	if (std::abs(velocityX) > 1.0f) { animator.play("walk"); goto apply_anim; }
	animator.play("idle");

apply_anim:
	animator.update(dt);
	animator.applyToSprite(sprite);
}

void Soldier::draw(sf::RenderWindow& window, float camOffsetX, float camOffsetY)
{
	if (!active) return;
	sprite.setPosition(x - camOffsetX, y - camOffsetY);
	
	float sx = facingRight ? animator.getScaleX() : -animator.getScaleX();
	sprite.setScale(sx, animator.getScaleY());
	animator.applyToSprite(sprite);

	window.draw(sprite);
}

void Soldier::takeDamage(int dmg)
{
	hp -= dmg;
	if (soundManager) soundManager->play("player_hit");
	if (hp <= 0) die();
}

void Soldier::die()
{
	active = false;
	if (soundManager) soundManager->play("enemy_die");
}

void Soldier::loadTexture(const char* path)
{
    if (path == nullptr || path[0] == '\0') return;
    if (!texture.loadFromFile(path))
    {
        // Fallback: leave texture blank — sprite will render as white rectangle
        // This is intentional: missing texture should not crash the build
        return;
    }
    sprite.setTexture(texture);
    // Default texture rect covers full image
    sprite.setTextureRect(sf::IntRect(
        0, 0,
        (int)texture.getSize().x,
        (int)texture.getSize().y));
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

void Soldier::moveLeft(float dt) { velocityX = -speedX * speedMult; facingRight = false; }
void Soldier::moveRight(float dt) { velocityX = speedX * speedMult; facingRight = true; }
void Soldier::jump() { if (onGround) { velocityY = speedY; onGround = false; if (soundManager) soundManager->play("jump"); } }

Projectile* Soldier::throwGrenade(float angle) {
	if (grenadeCount > 0) {
		grenadeCount--;
		float rad = angle * 3.14159f / 180.0f;
		return new BallisticProjectile(x, y, std::cos(rad)*600.0f, std::sin(rad)*600.0f, 20, true, nullptr);
	}
	return nullptr;
}

float Soldier::getVelocityX() const { return velocityX; }
float Soldier::getVelocityY() const { return velocityY; }
bool Soldier::isOnGround() const { return onGround; }
void Soldier::setSoundManager(SoundManager* sm) { soundManager = sm; }
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
			if (velocityY > 0.0f) {
				y = (float)(tileY * cell - height);
				velocityY = 0.0f;
				onGround = true;
			}
		}
	}
}

Projectile* Soldier::shoot(float angle)
{
	if (knifeOnly) return nullptr;
	if (currentWeapon && currentWeapon->canFire()) {
		animator.play("shoot"); // Trigger animation
		return currentWeapon->fire(x, y, angle, soundManager);
	}
	return nullptr;
}
