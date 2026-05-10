#include "../headers/Biome.h"
#include "../headers/Level.h"
#include "../headers/InfantryEnemies.h"
#include <cmath>

// ==================== PLAINS ====================
PlainsBiome::PlainsBiome(int start, int end) {
	startX = start; endX = end; minAlt = 8; maxAlt = 12;
}

int PlainsBiome::getHeightAt(int x) {
	return minAlt + (int)(std::sin(x * 0.1f) * 2.0f);
}

void PlainsBiome::spawnEnemies(Level* level) {
	static int spawnCounter = 0;
	spawnCounter++;
	if (spawnCounter % 5 != 0) return;

	float spawnX = (float)(endX * CELL_SIZE - 200);
	float spawnY = (float)((LEVEL_HEIGHT - 4) * CELL_SIZE);
	RebelSoldier* r = new RebelSoldier();
	r->setPosition(spawnX, spawnY);
	level->addEnemy(r);
}

// ==================== AERIAL ====================
AerialBiome::AerialBiome(int start, int end) {
	startX = start; endX = end; minAlt = 5; maxAlt = 8;
}

int AerialBiome::getHeightAt(int x) {
	return minAlt + (int)(std::cos(x * 0.05f) * 3.0f);
}

void AerialBiome::spawnEnemies(Level* level) {
	float spawnX = (float)(endX * CELL_SIZE - 200);
	float spawnY = (float)(3 * CELL_SIZE);
	// Paratrooper placeholder (Rebel for now)
	RebelSoldier* p = new RebelSoldier();
	p->setPosition(spawnX, spawnY);
	level->addEnemy(p);
}

// ==================== AQUATIC ====================
AquaticBiome::AquaticBiome(int start, int end) {
	startX = start; endX = end; minAlt = 16; maxAlt = 18;
}

int AquaticBiome::getHeightAt(int x) {
	return minAlt;
}

void AquaticBiome::spawnEnemies(Level* level) {
	float spawnX = (float)(endX * CELL_SIZE - 200);
	float spawnY = (float)((LEVEL_HEIGHT - 2) * CELL_SIZE);
	Zombie* z = new Zombie();
	z->setPosition(spawnX, spawnY);
	level->addEnemy(z);
}
