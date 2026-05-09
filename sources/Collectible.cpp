#include "../headers/Collectible.h"
#include "../headers/Soldier.h"
#include <cmath>

Collectible::Collectible() : ctype(CollectibleType::FOOD_FRUIT), pickedUp(false), bobTimer(0.0f)
{
	active = true;
	width = 32.0f;
	height = 32.0f;
}

void Collectible::update(float dt)
{
	if (!active) return;
	bobTimer += dt * 5.0f;
	// Entity y is the base position, we apply bobbing to the sprite in draw or here
}

void Collectible::draw(sf::RenderWindow& w, float cx, float cy)
{
	if (!active) return;
	float bobOffset = std::sin(bobTimer) * 10.0f;
	sprite.setPosition(x - cx, y - cy + bobOffset);
	w.draw(sprite);
}

void Collectible::checkPickup(class Soldier* s)
{
	if (!active || !s) return;
	if (getBounds().intersects(s->getBounds())) {
		onPickup(s);
		pickedUp = true;
		active = false;
	}
}

CollectibleType Collectible::getType() const { return ctype; }
