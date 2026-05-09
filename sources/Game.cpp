#include "../headers/Game.h"
#include "../headers/Projectile.h"
#include "../headers/Enemy.h"
#include "../headers/Camera.h"
#include "../headers/Level.h"
#include <iostream>

void Game::run()
{
	window.create(sf::VideoMode(1600, 900), "Metal Slug", sf::Style::Close);
	window.setFramerateLimit(60);

	// Initialize characters
	characters[0] = new PlayerMarco(100.0f, 0.0f);
	characters[1] = new PlayerTarma(100.0f, 0.0f);
	characters[2] = new PlayerEri(100.0f, 0.0f);
	characters[3] = new PlayerFio(100.0f, 0.0f);

	for (int i = 0; i < 4; ++i) {
		characters[i]->setSpeed(50.0f, 200.0f);
		characters[i]->setSpriteScale(0.2f, 0.2f);
	}

	characters[0]->loadTexture("Sprites/Character.png");
	characters[1]->loadTexture("Sprites/Tarma Roving.png");
	characters[2]->loadTexture("Sprites/Eri Kasamoto.png");
	characters[3]->loadTexture("Sprites/Fiolina Germi.png");

	activeCharIdx = 0;
	isPaused = false;

	// Load Level and HUD
	level.loadMockLevel();
	hud.init();

	if (pauseFont.loadFromFile("Sprites/font.ttf")) {
		pauseText.setFont(pauseFont);
		pauseText.setString("PAUSED");
		pauseText.setCharacterSize(100);
		pauseText.setFillColor(sf::Color::Yellow);
		pauseText.setPosition(650, 350);
	}

	sf::Clock clock;
	bool pWasPressed = false;

	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();
		sf::Event event;
		while (window.pollEvent(event)) { if (event.type == sf::Event::Closed) window.close(); }

		bool pIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::P);
		if (pIsPressed && !pWasPressed) isPaused = !isPaused;
		pWasPressed = pIsPressed;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) switchCharacter();

		if (!isPaused) {
			update(dt);
			characters[activeCharIdx]->resolveGround(level.getTileGrid(), level.getHeight(), level.getWidth(), level.getCellSize());
			camera.update(characters[activeCharIdx]->getX(), characters[activeCharIdx]->getY());

			bool fireIsPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || 
			                     sf::Keyboard::isKeyPressed(sf::Keyboard::L) ||
			                     sf::Keyboard::isKeyPressed(sf::Keyboard::X);

			if (fireIsPressed) {
				Projectile* p = characters[activeCharIdx]->shoot(0.0f);
				if (p) level.addProjectile(p); // Level now handles bullets
			}

			// Level handles enemy movement and projectile collisions
			int score = scoreManager.getScore();
			level.update(dt, score);
			
			// Sync score back to manager if it changed
			if (score != scoreManager.getScore()) {
				scoreManager.addScore(score - scoreManager.getScore());
			}
		}

		hud.update(characters[activeCharIdx]->getHp(), scoreManager.getScore(), "HERO");

		// Render
		window.clear(sf::Color(100, 149, 237));

		level.draw(window, camera);

		float cwx = characters[activeCharIdx]->getX(); float cwy = characters[activeCharIdx]->getY();
		characters[activeCharIdx]->setPosition(cwx - camera.getOffsetX(), cwy - camera.getOffsetY());
		characters[activeCharIdx]->draw(window);
		characters[activeCharIdx]->setPosition(cwx, cwy);

		hud.draw(window);
		if (isPaused) window.draw(pauseText);

		window.display();
	}

	for (int i = 0; i < 4; ++i) delete characters[i];
}

void Game::handleEvents() {}
void Game::update(float dt) { characters[activeCharIdx]->update(dt); }
void Game::switchCharacter() {
	float oldX = characters[activeCharIdx]->getX();
	float oldY = characters[activeCharIdx]->getY();
	activeCharIdx = (activeCharIdx + 1) % 4;
	characters[activeCharIdx]->setPosition(oldX, oldY);
}
void Game::render() {}
void Game::setGameMode(GameMode* mode) {}
void Game::checkDevModeCheat() {}
void Game::activateDeveloperMode() {}
