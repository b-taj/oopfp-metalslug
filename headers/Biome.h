#pragma once

/**
 * Biome.h -- Abstract base for terrain generation and enemy spawning rules.
 */

class Biome
{
public:
	virtual ~Biome() = default;
	virtual int		getHeightAt(int x) = 0;
	virtual void	spawnEnemies(class Level* level) = 0;
	
	bool contains(int x) const { return x >= startX && x <= endX; }

protected:
	int startX, endX;
	int minAlt, maxAlt;
	char name[32];
};

class PlainsBiome : public Biome
{
public:
	PlainsBiome(int start, int end);
	int		getHeightAt(int x) override;
	void	spawnEnemies(class Level* level) override;
};

class AerialBiome : public Biome
{
public:
	AerialBiome(int start, int end);
	int		getHeightAt(int x) override;
	void	spawnEnemies(class Level* level) override;
};

class AquaticBiome : public Biome
{
public:
	AquaticBiome(int start, int end);
	int		getHeightAt(int x) override;
	void	spawnEnemies(class Level* level) override;
};
