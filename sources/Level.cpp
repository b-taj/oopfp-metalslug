#include "../headers/Level.h"
#include "../headers/Camera.h"
#include "../headers/ScoreManager.h"
#include "../headers/Constants.h"

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

void Level::generateDebugLevel() { loadMockLevel(); }

void Level::update(float dt, ScoreManager& score)
{
	(void)score;
	if (playerPtr) {
		// Pass interact key state and camera offset (0 for mock)
		entityManager.update(dt, playerPtr, false, 0.0f);
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
		enemies[i]->draw(w, camOX, camOY);
	}
	
	Projectile** projs = entityManager.getProjectiles();
	for (int i = 0; i < entityManager.getProjectileCount(); ++i) {
		projs[i]->draw(w, camOX, camOY);
	}
}

void Level::addProjectile(class Projectile* p) { entityManager.addProjectile(p); }
void Level::addEnemy(class Enemy* e) { entityManager.addEnemy(e); }

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

const char** Level::getTileGrid() {
	// Re-purposing Level grid for char** representation if needed.
	// Since blocks is Block**, we can't directly return it as const char**.
	// We will use a static char grid for this specific UML request context.
	static char* rawGrid[LEVEL_HEIGHT];
	static char actualData[LEVEL_HEIGHT][LEVEL_WIDTH];
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			actualData[y][x] = (blocks[y][x].type == BlockType::AIR) ? ' ' : '#';
		}
		rawGrid[y] = actualData[y];
	}
	return (const char**)rawGrid;
}
int Level::getWidth() const { return width; }
int Level::getHeight() const { return height; }
int Level::getCellSize() const { return cellSize; }
