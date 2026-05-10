#include "../headers/PlayState.h"
#include "../headers/GameStateManager.h"
#include "../headers/Bosses.h"
#include <SFML/Window/Keyboard.hpp>
#include <cstring>

PlayState::PlayState(Level* lvl, PlayerSoldier** heroes, Camera* cam, HUD* h, ScoreManager* s, SoundManager* snd, GameStateManager* gsm)
	: level(lvl), characters(heroes), camera(cam), hud(h), scoreManager(s), soundManager(snd), stateManager(gsm),
	  activeCharIdx(0), zPressTimer(0.0f), zWasPressed(false), elapsedTime(0.0f), bossTransitionFlash(0.0f)
{
}

void PlayState::onEnter() 
{
	level->loadMockLevel();
	level->setPlayerPtr(characters[activeCharIdx]);

	// FIX 7: Boss Spawning
	UltimateBoss* boss = new UltimateBoss(&level->getEntityManager());
	boss->setPosition(10000.0f, (float)((LEVEL_HEIGHT - 5) * CELL_SIZE));
	level->addEnemy(boss);
}

void PlayState::handleEvents(sf::RenderWindow& window, sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::P) {
			stateManager->changeState(GameStateID::PAUSE);
		}
		if (event.key.code == sf::Keyboard::R) {
			timeline.rewind();
		}
	}
}

void PlayState::update(float dt)
{
	elapsedTime += dt;
	
	// Character Swapping Edge Detection
	bool zDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
	if (zDown && !zWasPressed) {
		float oldX = characters[activeCharIdx]->getX();
		float oldY = characters[activeCharIdx]->getY();
		activeCharIdx = (activeCharIdx + 1) % 4;
		characters[activeCharIdx]->setPosition(oldX, oldY);
		level->setPlayerPtr(characters[activeCharIdx]); // FIX 5
	}
	zWasPressed = zDown;

	// FIX 6: Game Over Transition
	if (characters[activeCharIdx]->getLives() <= 0) {
		bool anyAlive = false;
		for (int i = 0; i < 4; i++) if (characters[i]->getLives() > 0) anyAlive = true;

		if (!anyAlive) {
			stateManager->changeState(GameStateID::MENU);
			return;
		} else {
			for (int i = 0; i < 4; i++) {
				if (characters[i]->getLives() > 0) {
					float oldX = characters[activeCharIdx]->getX();
					float oldY = characters[activeCharIdx]->getY();
					activeCharIdx = i;
					characters[activeCharIdx]->setPosition(oldX, oldY);
					level->setPlayerPtr(characters[activeCharIdx]);
					break;
				}
			}
		}
	}

	// Update active character and subsystems
	characters[activeCharIdx]->update(dt);
	if (level) {
		level->update(dt, *scoreManager);
		camera->update(characters[activeCharIdx]->getX(), characters[activeCharIdx]->getY(), dt);
	}
	
	hud->update(characters[activeCharIdx]->getHp(), 100, 
				scoreManager->getScore(), characters[activeCharIdx]->getName(), 
				0, characters[activeCharIdx]->getGrenadeCount(), dt);

	takeSnapshot(dt);
}

void PlayState::takeSnapshot(float dt)
{
	GameStateSnapshot snap;
	snap.charX = characters[activeCharIdx]->getX();
	snap.charY = characters[activeCharIdx]->getY();
	snap.charHp = characters[activeCharIdx]->getHp();
	snap.charState = CharacterState::NORMAL; 
	snap.ammo = 0; 
	snap.grenadeCount = characters[activeCharIdx]->getGrenadeCount();
	snap.timestamp = elapsedTime;
	const char* wType = "PISTOL";
	for (int i = 0; i < 31 && wType[i] != '\0'; i++) {
		snap.weaponType[i] = wType[i];
		snap.weaponType[i + 1] = '\0';
	}
	timeline.recordFrame(snap);
}

void PlayState::render(sf::RenderWindow& window)
{
	if (level) level->draw(window, *camera);
	characters[activeCharIdx]->draw(window, camera->getOffsetX(), camera->getOffsetY());
	hud->draw(window);

	if (bossTransitionFlash > 0.0f) {
		sf::RectangleShape flash(sf::Vector2f((float)SCREEN_W, (float)SCREEN_H));
		float alpha = (bossTransitionFlash / 0.3f) * 200.f;
		flash.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)alpha));
		window.draw(flash);
		bossTransitionFlash -= 0.016f; // Simplified dt
	}
}

void PlayState::onExit() {}
