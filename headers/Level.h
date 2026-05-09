#pragma once
#include <SFML/Graphics.hpp>
#include "Block.h"

#include "Enemy.h"
#include "Projectile.h"

class Camera;

class Level
{
public:
	Level();
	~Level();

	void	loadMockLevel();

	void	update(float dt, int& score); // Pass score by ref to update it on kills
	void	draw(sf::RenderWindow& window, Camera& camera);

	void	addProjectile(Projectile* p);

	// Getters for Character collision
	char**	getTileGrid() { return grid; }
	int		getHeight() { return height; }
	int		getWidth() { return width; }
	int		getCellSize() { return cellSize; }

private:
	void    checkCollisions(int& score);

	char**		grid;
	int			height;
	int			width;
	int			cellSize;

	Enemy*      enemies[64];
	int         enemyCount;

	Projectile* projectiles[512];
	int         projectileCount;

	sf::Texture	blackTex, whiteTex;
	sf::Sprite	blackSprite, whiteSprite;
};
