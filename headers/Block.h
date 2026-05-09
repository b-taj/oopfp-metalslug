#pragma once
#include "Enums.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

/**
 * Block.h -- A single grid tile.
 * Renders shared textures and handles destruction/flooding logic.
 */

class Block
{
public:
	Block();
	
	void	init(int gx, int gy, BlockType t, sf::Texture* tex);
	void	draw(sf::RenderWindow& w, float camOX, float camOY);

	void	destroy();
	void	flood();

	sf::FloatRect getBounds() const;

	BlockType	type;
	int			gridX, gridY;
	bool		isDestructible;
	bool		isIndestructible;
	bool		isWater;
	bool		active;

private:
	sf::Sprite	sprite;
	sf::Texture* texture; // NOT owned
};
