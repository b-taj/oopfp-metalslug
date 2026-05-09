#pragma once
#include "Block.h"
#include "Biome.h"
#include "EntityManager.h"
#include "Camera.h"
#include "ScoreManager.h"
#include "Soldier.h"

/**
 * Level.h -- The main world container.
 * Owns the block grid, the biome rules, and the entity manager.
 */

class Level
{
public:
	Level();
	~Level();

	void	loadMockLevel();
	void	loadSurvivalLevel(int levelNum);
	void	generateDebugLevel();

	void	update(float dt, ScoreManager& score);
	void	draw(sf::RenderWindow& w, Camera& cam);

	void	addProjectile(class Projectile* p);
	void	addEnemy(class Enemy* e);
	
	Block*	getBlock(int x, int y);
	void	destroyBlock(int x, int y, int radius);
	void	fillCraterWithWater();
	
	Biome*	getBiomeAt(int x);
	
	int		getWidth() const;
	int		getHeight() const;
	int		getCellSize() const;
	
	void	setPlayerPtr(Soldier* p);
	const char** getTileGrid();

private:
	Block**			blocks;			// [LEVEL_HEIGHT][LEVEL_WIDTH]
	int				width;
	int				height;
	int				cellSize;

	Biome*			biomes[3];		// Aerial, Plains, Aquatic
	EntityManager	entityManager;
	Soldier*		playerPtr;

	sf::Texture		stoneTex;		// Managed shared texture
	
	int				seaLevel;
	bool			isRaining;
	float			rainTimer;
};
