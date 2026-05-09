#include "../headers/PlayerSoldier.h"
#include "../headers/Projectile.h"
#include "../headers/SoundManager.h"
#include "../headers/Constants.h"

PlayerSoldier::PlayerSoldier() : Soldier(), specialTimer(0.0f), specialActive(false)
{
	// Movement stats per Priority 2
	accel      = 1800.0f;
	friction   = 2400.0f;
	maxSpeedX  = 280.0f;
}

void PlayerSoldier::update(float dt)
{
	handleInput(dt);
	Soldier::update(dt);
	
	if (specialActive) {
		specialTimer -= dt;
		if (specialTimer <= 0) specialActive = false;
	}
}

void PlayerSoldier::handleInput(float dt)
{
	bool movingLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	bool movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	bool jumpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

	if (movingRight) {
		velocityX += accel * dt;
		if (velocityX > maxSpeedX) velocityX = maxSpeedX;
		facingRight = true;
	}
	else if (movingLeft) {
		velocityX -= accel * dt;
		if (velocityX < -maxSpeedX) velocityX = -maxSpeedX;
		facingRight = false;
	}
	else {
		// Deceleration / Friction
		if (velocityX > 0) {
			velocityX -= friction * dt;
			if (velocityX < 0) velocityX = 0;
		}
		else if (velocityX < 0) {
			velocityX += friction * dt;
			if (velocityX > 0) velocityX = 0;
		}
	}

	// Jump with edge detection
	if (onGround && jumpPressed && !jumpWasPressed) {
		velocityY = JUMP_FORCE;
		onGround = false;
		if (soundManager) soundManager->play("jump");
	}
	jumpWasPressed = jumpPressed;
}
