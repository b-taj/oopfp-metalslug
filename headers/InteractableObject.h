#pragma once
#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

/**
 * InteractableObject.h -- Abstract base for objects requiring a key press to use.
 */

class InteractableObject : public Entity
{
public:
	virtual ~InteractableObject() = default;

	virtual void	onInteract(class Soldier* s) = 0;
	
	void			update(float dt) override;
	void			draw(sf::RenderWindow& w, float cx, float cy) override;
	
	void			checkInteraction(class Soldier* s, bool keyPressed);

protected:
	InteractableObject();

	sf::Texture		texture;
	sf::Sprite		sprite;
};
