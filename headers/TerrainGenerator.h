#pragma once
#include "FractalNoise.h"
#include "LevelProfileFactory.h"
#include "Block.h"

/**
 * TerrainGenerator.h -- Orchestrates procedural terrain using fractal noise.
 * Includes a linear chunk cache for performance.
 */

class TerrainGenerator
{
public:
	TerrainGenerator(int seed = 1234);
	~TerrainGenerator();

	void	generateChunk(int chunkX, Block** blockGrid, int gridHeight, int gridWidth, int cellSize, sf::Texture* tex);
	float	getHeightAt(float worldX);
	void	setProfile(LevelProfileFactory* f);

private:
	FractalNoise		fractalNoise;
	LevelProfileFactory*	profile;
	int					seed;

	int					cachedChunkX[64];
	int					cachedCount;
};
