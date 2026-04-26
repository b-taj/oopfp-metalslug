#pragma once

class Level;

// Biome.h -- Abstract horizontal zone with its own terrain height function and
// enemy spawn rules. Three biomes stack vertically: Aerial (high), Plains (mid),
// Aquatic (low).

class Biome
{
public:
	virtual ~Biome() = default;

	// Return the terrain surface height in grid rows at horizontal position x.
	virtual int		getHeightAt(int x) = 0;

	// Populate level enemies with the correct batch counts for this biome.
	virtual void	spawnEnemies(Level* level) = 0;

	// True when grid position (x, y) falls within this biome's horizontal span.
	bool	contains(int x, int y);

protected:
	int			startX;
	int			endX;
	int			minAltitude;
	int			maxAltitude;
	std::string	name;
};

// High altitude; jagged mountainous peaks.
// Spawns aerial enemies and aliens; no ground undead.
class AerialBiome : public Biome
{
public:
	int		getHeightAt(int x) override;
	void	spawnEnemies(Level* level) override;

private:
	float	peakAmplitude;
};

// Central altitude; rolling hills.
// Spawns all infantry types and undead; primary vehicle zone.
class PlainsBiome : public Biome
{
public:
	int		getHeightAt(int x) override;
	void	spawnEnemies(Level* level) override;

private:
	float	hillFrequency;
};

// Below sea level; deep enough for submarine combat.
// Spawns zombies and enemy subs.
class AquaticBiome : public Biome
{
public:
	int		getHeightAt(int x) override;
	void	spawnEnemies(Level* level) override;

private:
	int		seaLevel;		// fixed Y row marking the water surface
	float	oceanDepth;
};
