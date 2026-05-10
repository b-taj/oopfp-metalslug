#include "../headers/Level.h"
#include "../headers/Camera.h"
#include "../headers/ScoreManager.h"
#include "../headers/Constants.h"
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
	grassTex.loadFromFile("Sprites/blocks/grass_block_side.png");

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
    // 1. Reset all blocks to AIR
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            blocks[r][c].init(c, r, BlockType::AIR, nullptr);
        }
    }

    // 2. SEGMENTED PLATFORM GENERATION
    for (int c = 0; c < width; c++) {
        bool isGap = (c % 5 == 0 || c % 5 == 4); 
        
        if (!isGap) {
            // Main ground: snap to exactly row 15 with slight variance
            int surfaceY = 15;
            if (c % 10 > 7) surfaceY = 14; // step up

            blocks[surfaceY][c].init(c, surfaceY, BlockType::GRASS, &grassTex);
            
            // SINGLE BLOCK SUSPENDED PLATFORMS (Floating in air)
            if (c % 12 > 4 && c % 12 < 8) {
                int highY = 8;
                blocks[highY][c].init(c, highY, BlockType::STONE, &stoneTex);
            }
        }

        // Bedrock safety line (bottom of screen)
        blocks[height-1][c].init(c, height-1, BlockType::INDESTRUCTIBLE, &stoneTex);
    }

    // Spawn platform
    blocks[15][2].init(2, 15, BlockType::GRASS, &grassTex);
}

void Level::update(float dt, ScoreManager& score)
{
	(void)score;
	if (playerPtr) {
		entityManager.update(dt, playerPtr, false, 0.0f);
		entityManager.cleanupInactive();
	}
}

void Level::draw(sf::RenderWindow& w, Camera& cam)
{
	float camOX = cam.getOffsetX();
	float camOY = cam.getOffsetY();

	int startCol = (int)(camOX / cellSize) - 1;
	int endCol   = startCol + (SCREEN_W / cellSize) + 2;

	if (startCol < 0) startCol = 0;
	if (endCol >= width) endCol = width - 1;

	for (int y = 0; y < height; ++y) {
		for (int x = startCol; x <= endCol; ++x) {
			blocks[y][x].draw(w, camOX, camOY);
		}
	}

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

const char** Level::getTileGrid() {
	static char* rawGrid[20];
	static char actualData[20][200];
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			actualData[y][x] = (blocks[y][x].type == BlockType::AIR) ? ' ' : '#';
		}
		rawGrid[y] = actualData[y];
	}
	return (const char**)rawGrid;
}

void Level::setPlayerPtr(Soldier* p) { playerPtr = p; }
EntityManager& Level::getEntityManager() { return entityManager; }
int Level::getWidth() const { return width; }
int Level::getHeight() const { return height; }
int Level::getCellSize() const { return cellSize; }
