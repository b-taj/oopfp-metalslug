#include "../headers/FractalNoise.h"

FractalNoise::FractalNoise(int seed, int oct, float amp, float freq)
	: octaves(oct), persistence(0.5f), lacunarity(2.0f), amplitude(amp), frequency(freq)
{
	if (octaves > 8) octaves = 8;
	for (int i = 0; i < octaves; ++i) {
		layers[i] = PerlinNoise(seed + i * 1337);
	}
}

float FractalNoise::sample(float x, float y)
{
	float total = 0.0f;
	float currAmp = amplitude;
	float currFreq = frequency;
	float maxValue = 0.0f;

	for (int i = 0; i < octaves; ++i) {
		total += layers[i].noise(x * currFreq, y * currFreq) * currAmp;
		maxValue += currAmp;
		currAmp *= persistence;
		currFreq *= lacunarity;
	}

	return total / maxValue;
}

void FractalNoise::setParameters(float amp, float freq, int oct)
{
	amplitude = amp;
	frequency = freq;
	octaves = (oct > 8) ? 8 : oct;
}
