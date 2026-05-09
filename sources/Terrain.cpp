#include "../headers/Terrain.h"

FractalNoise::FractalNoise()
	: octaves(1), persistence(0.5f), lacunarity(2.0f), amplitude(1.0f), frequency(1.0f)
{
}

float FractalNoise::sample(float x, float y) { (void)x; (void)y; return 0.0f; }
void FractalNoise::setParameters(float amp, float freq, int oct) { amplitude = amp; frequency = freq; octaves = oct; }

float PerlinNoise::noise(float x, float y) { (void)x; (void)y; return 0.0f; }
float PerlinNoise::noise(float x, float y, float z) { (void)x; (void)y; (void)z; return 0.0f; }
float PerlinNoise::fade(float t) { return t; }
float PerlinNoise::lerp(float t, float a, float b) { return a + t * (b - a); }
float PerlinNoise::grad(int hash, float x, float y, float z) { (void)hash; (void)x; (void)y; (void)z; return 0.0f; }
void PerlinNoise::shuffle(int seed) { (void)seed; }

void TerrainGenerator::generateChunk(int chunkX) { (void)chunkX; }
float TerrainGenerator::getHeightAt(float worldX) { (void)worldX; return 0.0f; }
Biome* TerrainGenerator::assignBiome(float y) { (void)y; return nullptr; }
void TerrainGenerator::setProfile(LevelProfileFactory* factory) { profile = factory; }
