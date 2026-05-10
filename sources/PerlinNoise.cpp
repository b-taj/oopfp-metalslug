#include "../headers/PerlinNoise.h"
#include <cmath>
#include <algorithm>

PerlinNoise::PerlinNoise(int seed)
{
	shuffle(seed);
	
	// Precompute gradients
	for (int i = 0; i < 512; ++i) {
		float z = 1.0f - 2.0f * (float)i / 512.0f;
		float r = std::sqrt(1.0f - z * z);
		float theta = 2.0f * 3.14159f * (float)i / 512.0f;
		gradients[i][0] = r * std::cos(theta);
		gradients[i][1] = r * std::sin(theta);
		gradients[i][2] = z;
	}
}

float PerlinNoise::fade(float t)
{
	// 6t^5 - 15t^4 + 10t^3
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise::lerp(float t, float a, float b)
{
	return a + t * (b - a);
}

float PerlinNoise::grad(int hash, float x, float y, float z)
{
	int h = hash & 15;
	float u = h < 8 ? x : y;
	float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

void PerlinNoise::shuffle(int seed)
{
	for (int i = 0; i < 256; ++i) permutation[i] = i;
	
	// Fisher-Yates with fixed PRNG to stay STL-free for randomness
	unsigned int next = (unsigned int)seed;
	for (int i = 255; i > 0; --i) {
		next = next * 1103515245 + 12345;
		int j = (unsigned int)(next / 65536) % (i + 1);
		int temp = permutation[i];
		permutation[i] = permutation[j];
		permutation[j] = temp;
	}

	for (int i = 0; i < 256; ++i) {
		permutation[256 + i] = permutation[i];
	}
}

float PerlinNoise::noise(float x, float y)
{
	return noise(x, y, 0.0f);
}

float PerlinNoise::noise(float x, float y, float z)
{
	int X = (int)std::floor(x) & 255;
	int Y = (int)std::floor(y) & 255;
	int Z = (int)std::floor(z) & 255;

	x -= std::floor(x);
	y -= std::floor(y);
	z -= std::floor(z);

	float u = fade(x);
	float v = fade(y);
	float w = fade(z);

	int A = permutation[X] + Y;
	int AA = permutation[A] + Z;
	int AB = permutation[A + 1] + Z;
	int B = permutation[X + 1] + Y;
	int BA = permutation[B] + Z;
	int BB = permutation[B + 1] + Z;

	return lerp(w, lerp(v, lerp(u, grad(permutation[AA], x, y, z),
		grad(permutation[BA], x - 1, y, z)),
		lerp(u, grad(permutation[AB], x, y - 1, z),
			grad(permutation[BB], x - 1, y - 1, z))),
		lerp(v, lerp(u, grad(permutation[AA + 1], x, y, z - 1),
			grad(permutation[BA + 1], x - 1, y, z - 1)),
			lerp(u, grad(permutation[AB + 1], x, y - 1, z - 1),
				grad(permutation[BB + 1], x - 1, y - 1, z - 1))));
}
