#pragma once


class Biome;
class Block;

// Terrain.h -- Procedural terrain pipeline:
//   PerlinNoise -> FractalNoise -> TerrainGenerator
// LevelProfileFactory (Abstract Factory) lets the player choose
// Amplified / Flat / Normal terrain before entering Campaign Mode.

// Classic gradient noise (Perlin 1985). Implemented from scratch per project spec.
class PerlinNoise
{
public:
	float	noise(float x, float y);		// 2-D noise in [-1, 1]
	float	noise(float x, float y, float z);	// 3-D noise in [-1, 1]

private:
	int		permutation[512];	// shuffled index table
	float	gradients[512][3];	// pseudo-random gradient vectors

	float	fade(float t);					// smoothstep: 6t^5 - 15t^4 + 10t^3
	float	lerp(float t, float a, float b);
	float	grad(int hash, float x, float y, float z);
	void	shuffle(int seed);				// Fisher-Yates on permutation table
};

// Stacks n octaves of PerlinNoise (double frequency, half amplitude each).
// Produces the fractal "Brownian motion" noise used for biome height maps.
class FractalNoise
{
public:
	float	sample(float x, float y);	// normalised fractal sample at (x, y)
	void	setParameters(float amp, float freq, int oct);

private:
	int					octaves;
	float				persistence;	// amplitude decay per octave (typically 0.5)
	float				lacunarity;		// frequency scale per octave (typically 2.0)
	float				amplitude;
	float				frequency;
	std::vector<PerlinNoise>	layers;
};

// Abstract Factory. One concrete factory per terrain profile.
// Stored as LevelProfileFactory* so TerrainGenerator calls it polymorphically.
class LevelProfileFactory
{
public:
	virtual ~LevelProfileFactory() = default;

	// Construct and return a pre-configured FractalNoise for this profile.
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

// Extra-tall peaks and deep oceans; plains remain normal.
class AmplifiedFactory : public LevelProfileFactory
{
public:
	FractalNoise	createNoise() override;
	float	getAmplitude() override;
	float	getFrequency() override;
	float	getOceanDepth() override;
};

// Minimal elevation changes; shallow ocean.
class FlatFactory : public LevelProfileFactory
{
public:
	FractalNoise	createNoise() override;
	float	getAmplitude() override;
	float	getFrequency() override;
	float	getOceanDepth() override;
};

// Uniformly distributed peaks and ocean depth (default profile).
class NormalFactory : public LevelProfileFactory
{
public:
	FractalNoise	createNoise() override;
	float	getAmplitude() override;
	float	getFrequency() override;
	float	getOceanDepth() override;
};

// Drives the infinite Campaign Mode world. Caches generated chunks for re-entry.
class TerrainGenerator
{
public:
	// Generate Block data for horizontal chunk index chunkX; cache the result.
	void	generateChunk(int chunkX);

	// Return surface height in world-pixels at worldX.
	float	getHeightAt(float worldX);

	// Map a world-Y value to the correct Biome pointer.
	Biome*	assignBiome(float y);

	// Swap the active profile; next generateChunk() uses the new parameters.
	void	setProfile(LevelProfileFactory* factory);

private:
	FractalNoise		fractalNoise;
	LevelProfileFactory*	profile;	// aggregated -- owned by CampaignMode
	int				seed;
};
