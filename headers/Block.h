#pragma once
#include <SFML/Graphics.hpp>
#include "Enums.h"

// Block.h -- A single 64x64 pixel voxel tile in the level grid.
// Destructible blocks are removed by blast radius; the indestructible bottom
// row survives all explosions. Water blocks fill craters exposed to rain.

class Block
{
public:
	void			draw(sf::RenderWindow& window);

	// Remove from grid if isDestructible is true.
	void			destroy();

	// Convert to WATER tile; called by Level::fillCraterWithWater().
	void			flood();

	sf::FloatRect	getBounds();

	// ── data ─────────────────────────────────────────────────────────────
	int			x;					// grid column
	int			y;					// grid row
	BlockType	type;
	bool		isDestructible;
	bool		isIndestructible;		// true for bedrock bottom row
	bool		isSolid;			// false for AIR and WATER
	bool		isWater;

private:
	sf::Sprite	sprite;
	sf::Texture	texture;
};
