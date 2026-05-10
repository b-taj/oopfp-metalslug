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

void Level::loadMockLevel()
{
    // Allocate block grid if not already done
    if (blocks == nullptr)
    {
        blocks = new Block*[height];
        for (int r = 0; r < height; r++)
        {
            blocks[r] = new Block[width];
            for (int c = 0; c < width; c++)
            {
                blocks[r][c].gridX         = c;
                blocks[r][c].gridY         = r;
                blocks[r][c].type          = BlockType::AIR;
                blocks[r][c].isDestructible   = false;
                blocks[r][c].isIndestructible = false;
                blocks[r][c].isWater          = false;
            }
        }
    }

    // Ground row at row 15 (out of 20)
    for (int c = 0; c < width; c++)
    {
        blocks[15][c].type = BlockType::GRASS;
        blocks[15][c].isDestructible = true;

        // Stone below ground
        for (int r = 16; r < height - 1; r++)
        {
            blocks[r][c].type = BlockType::STONE;
            blocks[r][c].isDestructible = true;
        }
        // Indestructible bedrock at bottom row
        blocks[height-1][c].type = BlockType::INDESTRUCTIBLE;
        blocks[height-1][c].isIndestructible = true;
    }

    // A few platforms at different heights
    for (int c = 20; c < 30; c++) blocks[11][c].type = BlockType::STONE;
    for (int c = 40; c < 50; c++) blocks[9][c].type  = BlockType::STONE;
    for (int c = 60; c < 70; c++) blocks[12][c].type = BlockType::STONE;

    // Load tile textures
    grassTex.loadFromFile("Sprites/blocks/grass_block_side.png");
    stoneTex.loadFromFile("Sprites/blocks/stone.png");

    // Assign textures to blocks based on type
    for (int r = 0; r < height; r++)
        for (int c = 0; c < width; c++)
        {
            switch (blocks[r][c].type)
            {
                case BlockType::GRASS:
                    blocks[r][c].init(c, r, BlockType::GRASS, &grassTex);
                    break;
                case BlockType::STONE:
                    blocks[r][c].init(c, r, BlockType::STONE, &stoneTex);
                    break;
                case BlockType::INDESTRUCTIBLE:
                    blocks[r][c].init(c, r, BlockType::INDESTRUCTIBLE, &stoneTex);
                    break;
                default: 
                    blocks[r][c].init(c, r, BlockType::AIR, nullptr);
                    break;
            }
            // Note: init() already sets the sprite texture. 
            // We ensure scaling matches CELL_SIZE if needed.
        }
}

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

EntityManager& Level::getEntityManager() { return entityManager; }

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
