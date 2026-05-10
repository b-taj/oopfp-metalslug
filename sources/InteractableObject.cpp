#include "../headers/InteractableObject.h"
#include "../headers/Soldier.h"
#include <cmath>

InteractableObject::InteractableObject()
{
	active = true;
	width = 64.0f;
	height = 64.0f;
}

void InteractableObject::update(float dt) { (void)dt; }

void InteractableObject::draw(sf::RenderWindow& w, float cx, float cy)
{
	if (!active) return;
	sprite.setPosition(x - cx, y - cy);
	w.draw(sprite);
}

void InteractableObject::checkInteraction(class Soldier* s, bool keyPressed)
{
	if (!active || !s || !keyPressed) return;
	
	// Distance check instead of strict bounds if needed, but UML suggests checkInteraction
	if (getBounds().intersects(s->getBounds())) {
		onInteract(s);
	}
}
