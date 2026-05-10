#include "../headers/TerrainGenerator.h"
#include "../headers/Constants.h"
#include <cmath>
#include <cstdlib>

TerrainGenerator::TerrainGenerator(int s) 
	: fractalNoise(s, 4, 8.0f, 0.03f), profile(nullptr), seed(s), cachedCount(0)
{
	for (int i = 0; i < 64; ++i) cachedChunkX[i] = -1;
}

TerrainGenerator::~TerrainGenerator() {}

void TerrainGenerator::setProfile(LevelProfileFactory* f)
{
	profile = f;
	if (profile) {
		fractalNoise = profile->createNoise();
	}
}

float TerrainGenerator::getHeightAt(float worldX)
{
	// Base height for the "main" platform layer
	return (float)(LEVEL_HEIGHT / 2 + 3); 
}

void TerrainGenerator::generateChunk(int chunkX, Block** blockGrid, int gridHeight, int gridWidth, int cellSize, sf::Texture* tex)
{
	// Check cache
	for (int i = 0; i < cachedCount; ++i) {
		if (cachedChunkX[i] == chunkX) return; 
	}

	// Use seed for deterministic randomness per chunk
	std::srand(seed + chunkX);

	for (int x = 0; x < gridWidth; ++x) {
		for (int y = 0; y < gridHeight; ++y) {
			blockGrid[y][x].init(x, y, BlockType::AIR, nullptr);
		}
	}

	// CONSOLIDATED GENERATION (Aligned with Platformer Requirements)
	for (int x = 0; x < gridWidth; x++) {
		float worldX = (float)(chunkX * gridWidth + x);
		
		// Periodic gaps for platformer feel
		if (x % 5 == 0 || x % 5 == 4) continue;

		// Main ground logic
		int surfaceY = 15 + (int)(std::sin(worldX * 0.5f) * 2.0f);
		if (surfaceY < gridHeight && surfaceY >= 5) {
			blockGrid[surfaceY][x].init(x, surfaceY, BlockType::GRASS, tex);
		}
	}

	// Update cache
	int idx = cachedCount % 64;
	cachedChunkX[idx] = chunkX;
	if (cachedCount < 64) cachedCount++;
}
