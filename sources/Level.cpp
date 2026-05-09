#include "../headers/Level.h"
#include "../headers/Constants.h"
#include "../headers/Projectile.h"
#include <cmath>

Level::Level() 
	: width(LEVEL_WIDTH), height(LEVEL_HEIGHT), cellSize(CELL_SIZE), 
	  playerPtr(nullptr), seaLevel(16), isRaining(false), rainTimer(0.0f)
{
	blocks = new Block*[height];
	for (int i = 0; i < height; ++i) {
		blocks[i] = new Block[width];
	}

	stoneTex.loadFromFile("Sprites/blocks/stone.png");

	biomes[0] = new AerialBiome(0, 50);
	biomes[1] = new PlainsBiome(51, 150);
	biomes[2] = new AquaticBiome(151, 199);
}

Level::~Level()
{
	for (int i = 0; i < height; ++i) delete[] blocks[i];
	delete[] blocks;
	
	for (int i = 0; i < 3; ++i) delete biomes[i];
}

void Level::loadMockLevel()
{
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			BlockType t = (y >= 15) ? BlockType::STONE : BlockType::AIR;
			if (y == height - 1) t = BlockType::INDESTRUCTIBLE;
			blocks[y][x].init(x, y, t, &stoneTex);
		}
	}
}

void Level::update(float dt, ScoreManager& score)
{
	if (playerPtr) {
		entityManager.update(dt, playerPtr);
		entityManager.cleanupInactive();
	}
}

void Level::draw(sf::RenderWindow& w, Camera& cam)
{
	float camOX = cam.getOffsetX();
	float camOY = cam.getOffsetY();

	// Draw Culling logic
	int startCol = (int)(camOX / cellSize) - 1;
	int endCol   = startCol + (SCREEN_W / cellSize) + 2;

	if (startCol < 0) startCol = 0;
	if (endCol >= width) endCol = width - 1;

	for (int y = 0; y < height; ++y) {
		for (int x = startCol; x <= endCol; ++x) {
			blocks[y][x].draw(w, camOX, camOY);
		}
	}

	// Entities
	Enemy** enemies = entityManager.getEnemies();
	for (int i = 0; i < entityManager.getEnemyCount(); ++i) {
		enemies[i]->draw(window, camOX, camOY); // assuming draw takes offsets
	}
	
	Projectile** projs = entityManager.getProjectiles();
	for (int i = 0; i < entityManager.getProjectileCount(); ++i) {
		projs[i]->draw(window, camOX, camOY);
	}
}

void Level::addProjectile(Projectile* p) { entityManager.addProjectile(p); }
void Level::addEnemy(Enemy* e) { entityManager.addEnemy(e); }

Block* Level::getBlock(int x, int y) {
	if (x >= 0 && x < width && y >= 0 && y < height) return &blocks[y][x];
	return nullptr;
}

void Level::destroyBlock(int x, int y, int radius) {
	for (int i = y - radius; i <= y + radius; ++i) {
		for (int j = x - radius; j <= x + radius; ++j) {
			Block* b = getBlock(j, i);
			if (b) b->destroy();
		}
	}
}

void Level::setPlayerPtr(Soldier* p) { playerPtr = p; }
int Level::getWidth() const { return width; }
int Level::getHeight() const { return height; }
int Level::getCellSize() const { return cellSize; }
