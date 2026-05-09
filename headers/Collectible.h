#pragma once
#include "Entity.h"
#include "Enums.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

/**
 * Collectible.h -- Abstract base for items picked up by walking over them.
 */

class Collectible : public Entity
{
public:
	virtual ~Collectible() = default;

	virtual void	onPickup(class Soldier* s) = 0;
	
	void			update(float dt) override;
	void			draw(sf::RenderWindow& w, float cx, float cy) override;
	
	void			checkPickup(class Soldier* s);
	CollectibleType getType() const;

protected:
	Collectible();

	sf::Texture		texture;
	sf::Sprite		sprite;
	CollectibleType ctype;
	bool			pickedUp;
	float			bobTimer;
};
