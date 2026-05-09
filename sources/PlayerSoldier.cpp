#include "../headers/PlayerSoldier.h"
#include "../headers/Projectile.h"

PlayerSoldier::PlayerSoldier() : Soldier(), specialTimer(0.0f), specialActive(false)
{
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
	(void)dt;
	// Movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		velocityX = -speedX * speedMult;
		facingRight = false;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		velocityX = speedX * speedMult;
		facingRight = true;
	}

	// Jump
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && onGround) {
		velocityY = speedY;
		onGround = false;
	}

	// Shooting logic handled by Game class calling shoot() usually, 
	// but can be put here if input-to-projectile needs direct control.
}
