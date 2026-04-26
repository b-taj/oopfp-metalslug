#include "../headers/Level.h"
#include "../headers/Camera.h"

Level::Level()
{
	cellSize = 64;
	height = 14;
	width = 110;
	grid = new char*[height];
	for (int i = 0; i < height; ++i) grid[i] = new char[width];

	blackTex.loadFromFile("Sprites/blocks/stone.png");
	whiteTex.loadFromFile("Sprites/blocks/grass_block_side.png");
	blackSprite.setTexture(blackTex);
	whiteSprite.setTexture(whiteTex);

	enemyCount = 0;
	for (int i = 0; i < 64; i++) enemies[i] = nullptr;
	projectileCount = 0;
	for (int i = 0; i < 512; i++) projectiles[i] = nullptr;
}

Level::~Level()
{
	for (int i = 0; i < height; ++i) delete[] grid[i];
	delete[] grid;
	for (int i = 0; i < enemyCount; i++) if(enemies[i]) delete enemies[i];
	for (int i = 0; i < projectileCount; i++) if(projectiles[i]) delete projectiles[i];
}

void Level::loadMockLevel()
{
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			if (i == 13) grid[i][j] = 'B';
			else if (i == 10 && (j % 12 < 4)) grid[i][j] = 'W'; 
			else if (i == 7 && (j % 12 > 6)) grid[i][j] = 'W';
			else if (i == 4 && (j % 15 < 3)) grid[i][j] = 'W';
			else grid[i][j] = ' ';
		}
	}
	if (enemyCount < 64) enemies[enemyCount++] = new RebelSoldier(600.0f, 770.0f);
	if (enemyCount < 64) enemies[enemyCount++] = new RebelSoldier(1500.0f, 770.0f);
	if (enemyCount < 64) enemies[enemyCount++] = new RebelSoldier(2500.0f, 770.0f);
}

void Level::addProjectile(Projectile* p)
{
	if (projectileCount < 512) projectiles[projectileCount++] = p;
	else delete p;
}

void Level::checkCollisions(int& score)
{
	for (int i = 0; i < projectileCount; i++) {
		if (!projectiles[i]->isAlive) continue;

		for (int e = 0; e < enemyCount; e++) {
			if (enemies[e] && enemies[e]->getIsAlive()) {
				float ex = enemies[e]->getX() + 37.0f; // Center
				float ey = enemies[e]->getY() + 37.0f;
				float dx = (projectiles[i]->x + 16.0f) - ex;
				float dy = (projectiles[i]->y + 16.0f) - ey;
				
				if ((dx*dx + dy*dy) < 1600.0f) { // 40px radius
					enemies[e]->takeDamage(projectiles[i]->damage);
					if (!enemies[e]->getIsAlive()) score += enemies[e]->getScore();
					projectiles[i]->onHit();
					break;
				}
			}
		}
	}
}

void Level::update(float dt, int& score)
{
	for (int i = 0; i < enemyCount; i++) if(enemies[i]) enemies[i]->update(dt);
	for (int i = 0; i < projectileCount; i++) if(projectiles[i]) projectiles[i]->update(dt);

	checkCollisions(score);

	// Cleanup dead projectiles
	for (int i = 0; i < projectileCount; ) {
		if (!projectiles[i]->isAlive) {
			delete projectiles[i];
			projectiles[i] = projectiles[projectileCount - 1];
			projectileCount--;
		} else { i++; }
	}
}

void Level::draw(sf::RenderWindow& window, Camera& camera)
{
	float camX = camera.getOffsetX();
	float camY = camera.getOffsetY();

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			float screenX = (static_cast<float>(j * cellSize)) - camX;
			if (screenX > -cellSize && screenX < 1600) {
				if (grid[i][j] == 'B') {
					blackSprite.setPosition(screenX, static_cast<float>(i * cellSize) - camY);
					window.draw(blackSprite);
				} else if (grid[i][j] == 'W') {
					whiteSprite.setPosition(screenX, static_cast<float>(i * cellSize) - camY);
					window.draw(whiteSprite);
				}
			}
		}
	}

	for (int i = 0; i < enemyCount; i++) {
		if (enemies[i] && enemies[i]->getIsAlive()) {
			float wx = enemies[i]->getX(); float wy = enemies[i]->getY();
			enemies[i]->setPosition(wx - camX, wy - camY);
			enemies[i]->draw(window);
			enemies[i]->setPosition(wx, wy);
		}
	}

	for (int i = 0; i < projectileCount; i++) {
		if (projectiles[i] && projectiles[i]->isAlive) {
			float worldX = projectiles[i]->x; float worldY = projectiles[i]->y;
			projectiles[i]->setPosition(worldX - camX, worldY - camY);
			projectiles[i]->draw(window);
			projectiles[i]->setPosition(worldX, worldY);
		}
	}
}
