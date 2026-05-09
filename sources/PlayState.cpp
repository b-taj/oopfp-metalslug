#include "../headers/PlayState.h"
#include "../headers/GameStateManager.h"
#include <SFML/Window/Keyboard.hpp>
#include <cstring>

extern GameStateManager gStateManager;

PlayState::PlayState(Level* lvl, PlayerSoldier** heroes, Camera* cam, HUD* h, ScoreManager* s, SoundManager* snd)
	: level(lvl), characters(heroes), camera(cam), hud(h), scoreManager(s), soundManager(snd),
	  activeCharIdx(0), zPressTimer(0.0f), zWasPressed(false), elapsedTime(0.0f)
{
}

void PlayState::onEnter() {}

void PlayState::handleEvents(sf::RenderWindow& window, sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::P) {
			gStateManager.changeState(GameStateID::PAUSE);
		}
		if (event.key.code == sf::Keyboard::R) {
			timeline.rewind();
		}
	}
}

void PlayState::update(float dt)
{
	elapsedTime += dt;
	
	// Character Swapping Edge Detection (FIX 1)
	bool zDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
	if (zDown && !zWasPressed) {
		float oldX = characters[activeCharIdx]->getX();
		float oldY = characters[activeCharIdx]->getY();
		activeCharIdx = (activeCharIdx + 1) % 4;
		characters[activeCharIdx]->setPosition(oldX, oldY);
	}
	zWasPressed = zDown;

	// Update active character and subsystems
	characters[activeCharIdx]->update(dt);
	if (level) {
		level->update(dt, *scoreManager);
		camera->update(characters[activeCharIdx]->getX(), characters[activeCharIdx]->getY(), dt);
		
		// UNDERWATER INTEGRATION (Prompt 12.2)
		if (soundManager) {
			if (characters[activeCharIdx]->getY() > 800.0f) { 
				soundManager->setUnderwater(true);
			} else {
				soundManager->setUnderwater(false);
			}
		}
	}
	
	// FIX 6: HUD draws current active character stats every frame
	hud->update(characters[activeCharIdx]->getHp(), characters[activeCharIdx]->getMaxHp(), 
				scoreManager->getScore(), characters[activeCharIdx]->getName(), 
				0, characters[activeCharIdx]->getGrenadeCount(), dt);

	// Timeline Recording
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
}

void PlayState::onExit() {}
