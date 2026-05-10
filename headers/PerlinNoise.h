#pragma once

/**
 * PerlinNoise.h -- Implementation of Ken Perlin's improved noise algorithm.
 * Used for procedural terrain generation.
 */

class PerlinNoise
{
public:
	PerlinNoise(int seed = 0);
	float	noise(float x, float y);
	float	noise(float x, float y, float z);

private:
	float	fade(float t);
	float	lerp(float t, float a, float b);
	float	grad(int hash, float x, float y, float z);
	void	shuffle(int seed);

	int		permutation[512];
	float	gradients[512][3];
};
