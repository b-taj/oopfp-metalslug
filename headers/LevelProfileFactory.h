#pragma once
#include "FractalNoise.h"

/**
 * LevelProfileFactory.h -- Factory Method for creating noise profiles.
 */

class LevelProfileFactory
{
public:
	virtual ~LevelProfileFactory() = default;

	virtual FractalNoise createNoise() const = 0;
	virtual float getAmplitude() const = 0;
	virtual float getFrequency() const = 0;
	virtual float getOceanDepth() const = 0;

protected:
	float	amplitude;
	float	frequency;
	int		octaves;
	float	oceanDepth;
};

class AmplifiedFactory : public LevelProfileFactory {
public:
    AmplifiedFactory() { amplitude = 8.0f; frequency = 0.05f; octaves = 6; oceanDepth = 12.0f; }
    FractalNoise createNoise() const override { return FractalNoise(1234, octaves, amplitude, frequency); }
    float getAmplitude() const override { return 8.0f; }
    float getFrequency() const override { return frequency; }
    float getOceanDepth() const override { return oceanDepth; }
};

class FlatFactory : public LevelProfileFactory {
public:
    FlatFactory() { amplitude = 2.0f; frequency = 0.02f; octaves = 3; oceanDepth = 15.0f; }
    FractalNoise createNoise() const override { return FractalNoise(1234, octaves, amplitude, frequency); }
    float getAmplitude() const override { return 2.0f; }
    float getFrequency() const override { return frequency; }
    float getOceanDepth() const override { return oceanDepth; }
};

class NormalFactory : public LevelProfileFactory {
public:
    NormalFactory() { amplitude = 5.0f; frequency = 0.03f; octaves = 4; oceanDepth = 14.0f; }
    FractalNoise createNoise() const override { return FractalNoise(1234, octaves, amplitude, frequency); }
    float getAmplitude() const override { return 5.0f; }
    float getFrequency() const override { return frequency; }
    float getOceanDepth() const override { return oceanDepth; }
};
