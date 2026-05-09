#include "../headers/Biome.h"
#include "../headers/Level.h"
#include "../headers/InfantryEnemies.h"
#include <cmath>

// ==================== PLAINS ====================
PlainsBiome::PlainsBiome(int start, int end) {
	startX = start; endX = end; minAlt = 12; maxAlt = 15;
}

int PlainsBiome::getHeightAt(int x) {
	return minAlt + (int)(std::sin(x * 0.1f) * 2.0f);
}

void PlainsBiome::spawnEnemies(Level* level) {
	for (int i = startX; i < endX; i += 20) {
		level->addEnemy(new RebelSoldier()); // Simplified spawn
	}
}

// ==================== AERIAL ====================
AerialBiome::AerialBiome(int start, int end) {
	startX = start; endX = end; minAlt = 5; maxAlt = 8;
}

int AerialBiome::getHeightAt(int x) {
	return minAlt + (int)(std::cos(x * 0.05f) * 3.0f);
}

void AerialBiome::spawnEnemies(Level* level) {
	// Spawn paratroopers or martians
}

// ==================== AQUATIC ====================
AquaticBiome::AquaticBiome(int start, int end) {
	startX = start; endX = end; minAlt = 16; maxAlt = 18;
}

int AquaticBiome::getHeightAt(int x) {
	return minAlt; // Flat sea floor
}

void AquaticBiome::spawnEnemies(Level* level) {
	// Spawn aquatic enemies
}
