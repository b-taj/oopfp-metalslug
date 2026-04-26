#pragma once
#include <SFML/Graphics.hpp>

#include "Block.h"

class Biome;
class Enemy;
class EnemyVehicle;
class Vehicle;
class Collectible;
class Camera;

// Level.h -- Owns the 2-D block grid and all entity lists for one stage.
// Survival levels: 150-250 blocks wide, 20-60 blocks tall, fixed enemy positions.
// Campaign level: effectively infinite (chunks generated on demand).

class Level
{
public:
	void		update(float dt);
	void		draw(sf::RenderWindow& window, Camera& cam);

	// Returns block at grid position (x,y); nullptr if out of range.
	Block*		getBlock(int x, int y);

	// Remove all destructible blocks within blast radius of grid cell (x, y).
	void		destroyBlock(int x, int y, int radius);

	// Fill any crater (width <= 3 blocks, depth >= 1 block) exposed to rain.
	void		fillCraterWithWater();

	// Return the Biome containing grid column x.
	Biome*		getBiomeAt(int x);

	// Add a new enemy to the active entity list at world position (x, y).
	void		spawnEnemy(Enemy* enemy, int x, int y);

	// Begin a randomised rain event (duration and interval are random).
	void		triggerRain();

	// True when Campaign kill quotas are met (unused in Survival; checked per level).
	bool		isComplete();

private:
	Block**					blocks;			// [height][width] heap-allocated grid
	int					width;
	int					height;
	std::vector<Biome*>		biomes;			// owned; ordered left to right
	std::vector<Enemy*>		enemies;		// active enemy list (aggregated)
	std::vector<EnemyVehicle*>	enemyVehicles;
	std::vector<Collectible*>	collectibles;
	std::vector<Vehicle*>		playerVehicles;	// parked vehicles available to board
	int					seaLevel;		// fixed grid row for AquaticBiome surface
	bool					isRaining;
	float					rainTimer;
};
