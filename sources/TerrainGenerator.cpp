#include "../headers/TerrainGenerator.h"
#include "../headers/Constants.h"
#include <cmath>

TerrainGenerator::TerrainGenerator(int s) 
	: fractalNoise(s, 4, 12.0f, 0.03f), profile(nullptr), seed(s), cachedCount(0)
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
	// Noise sample shifted and scaled to grid height
	float sample = fractalNoise.sample(worldX, 0.0f);
	float baseHeight = 15.0f; // Typical floor
	return baseHeight - (sample * (profile ? profile->getAmplitude() : 5.0f));
}

void TerrainGenerator::generateChunk(int chunkX, Block** blockGrid, int gridHeight, int gridWidth, int cellSize, sf::Texture* tex)
{
	// Check cache
	for (int i = 0; i < cachedCount; ++i) {
		if (cachedChunkX[i] == chunkX) return; 
	}

	// Simple generation for the provided grid
	for (int x = 0; x < gridWidth; ++x) {
		float worldX = (float)(chunkX * gridWidth + x);
		int surfaceY = (int)getHeightAt(worldX);

		for (int y = 0; y < gridHeight; ++y) {
			BlockType t = BlockType::AIR;
			if (y >= surfaceY) t = BlockType::STONE;
			if (y == gridHeight - 1) t = BlockType::INDESTRUCTIBLE;
			
			blockGrid[y][x].init(x, y, t, tex);
		}
	}

	// Update cache
	int idx = cachedCount % 64;
	cachedChunkX[idx] = chunkX;
	if (cachedCount < 64) cachedCount++;
}
