#pragma once
#include "PerlinNoise.h"

/**
 * FractalNoise.h -- Sums multiple octaves of Perlin noise.
 */

class FractalNoise
{
public:
	FractalNoise(int seed = 0, int oct = 4, float amp = 1.0f, float freq = 1.0f);
	float sample(float x, float y);
	void  setParameters(float amp, float freq, int oct);

private:
	PerlinNoise layers[8];
	int			octaves;
	float		persistence;
	float		lacunarity;
	float		amplitude;
	float		frequency;
};
