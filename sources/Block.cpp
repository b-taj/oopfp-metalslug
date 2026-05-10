#include "../headers/Block.h"
#include "../headers/Constants.h"

Block::Block() 
	: type(BlockType::AIR), gridX(0), gridY(0), 
	  isDestructible(true), isIndestructible(false), isWater(false), active(false),
	  texture(nullptr)
{}

void Block::init(int gx, int gy, BlockType t, sf::Texture* tex)
{
	gridX = gx;
	gridY = gy;
	type = t;
	texture = tex;
	active = (type != BlockType::AIR);
	isWater = (type == BlockType::WATER);
	isIndestructible = (type == BlockType::INDESTRUCTIBLE);
	isDestructible = !isIndestructible && active && !isWater;

	if (active && texture) {
		sprite.setTexture(*texture);
		sprite.setPosition((float)(gridX * CELL_SIZE), (float)(gridY * CELL_SIZE));
	}
}

void Block::draw(sf::RenderWindow& w, float camOX, float camOY)
{
	if (!active || type == BlockType::AIR) return;
	sprite.setPosition((float)(gridX * CELL_SIZE) - camOX, (float)(gridY * CELL_SIZE) - camOY);
	w.draw(sprite);
}

void Block::destroy()
{
	if (isDestructible) {
		type = BlockType::AIR;
		active = false;
	}
}

void Block::flood()
{
	type = BlockType::WATER;
	isWater = true;
	active = true;
	// Texture should ideally change to a water texture atlas here
}

sf::FloatRect Block::getBounds() const
{
	return sf::FloatRect((float)(gridX * CELL_SIZE), (float)(gridY * CELL_SIZE), (float)CELL_SIZE, (float)CELL_SIZE);
}
