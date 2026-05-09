#pragma once

#include "Enums.h"

class Biome;
class Block;

// Terrain.h -- Procedural terrain pipeline:
//   PerlinNoise -> FractalNoise -> TerrainGenerator
// LevelProfileFactory (Abstract Factory) lets the player choose
// Amplified / Flat / Normal terrain before entering Campaign Mode.

class PerlinNoise
{
public:
	float	noise(float x, float y);		
	float	noise(float x, float y, float z);	

private:
	int		permutation[512];	
	float	gradients[512][3];	

	float	fade(float t);					
	float	lerp(float t, float a, float b);
	float	grad(int hash, float x, float y, float z);
	void	shuffle(int seed);				
};

class FractalNoise
{
public:
	FractalNoise();
	float	sample(float x, float y);	
	void	setParameters(float amp, float freq, int oct);

private:
	int					octaves;
	float				persistence;	
	float				lacunarity;		
	float				amplitude;
	float				frequency;
	PerlinNoise			layers[8]; // Fixed size instead of std::vector
};

class LevelProfileFactory
{
public:
	virtual ~LevelProfileFactory() = default;

	virtual FractalNoise	createNoise() = 0;
	virtual float	getAmplitude() = 0;
	virtual float	getFrequency() = 0;
	virtual float	getOceanDepth() = 0;

protected:
	float	amplitude;
	float	frequency;
	int		octaves;
	float	oceanDepth;
};

class AmplifiedFactory : public LevelProfileFactory
{
public:
	FractalNoise	createNoise() override;
	float	getAmplitude() override;
	float	getFrequency() override;
	float	getOceanDepth() override;
};

class FlatFactory : public LevelProfileFactory
{
public:
	FractalNoise	createNoise() override;
	float	getAmplitude() override;
	float	getFrequency() override;
	float	getOceanDepth() override;
};

class NormalFactory : public LevelProfileFactory
{
public:
	FractalNoise	createNoise() override;
	float	getAmplitude() override;
	float	getFrequency() override;
	float	getOceanDepth() override;
};

class TerrainGenerator
{
public:
	void	generateChunk(int chunkX);
	float	getHeightAt(float worldX);
	Biome*	assignBiome(float y);
	void	setProfile(LevelProfileFactory* factory);

private:
	FractalNoise		fractalNoise;
	LevelProfileFactory*	profile;	
	int				seed;
};
