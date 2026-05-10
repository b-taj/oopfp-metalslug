#include "../headers/PlayState.h"
#include "../headers/GameStateManager.h"
#include "../headers/Bosses.h"
#include "../headers/SoundManager.h"
#include <SFML/Window/Keyboard.hpp>
#include <cstring>

PlayState::PlayState(Level* lvl, PlayerSoldier** heroes, Camera* cam, HUD* h, ScoreManager* s, SoundManager* snd, GameStateManager* gsm)
	: level(lvl), characters(heroes), camera(cam), hud(h), scoreManager(s), soundManager(snd), stateManager(gsm),
	  activeCharIdx(0), zPressTimer(0.0f), zWasPressed(false), isPaused(false), elapsedTime(0.0f), bossTransitionFlash(0.0f)
{
	if (pauseFont.loadFromFile("Sprites/font.ttf")) {
		pauseText.setFont(pauseFont);
		pauseText.setString("PAUSED");
		pauseText.setCharacterSize(100);
		pauseText.setFillColor(sf::Color::Yellow);
		pauseText.setPosition(650.0f, 350.0f);
	}

	jungleBgTex.loadFromFile("Sprites/backgrounds/jungle_biome_bg.png");
	aerialBgTex.loadFromFile("Sprites/backgrounds/aerial_biome_bg.png");
	aquaticBgTex.loadFromFile("Sprites/backgrounds/aquatic_biome_bg.png");
	bgSprite.setTexture(jungleBgTex);
}

void PlayState::onEnter() 
{
	level->loadMockLevel();

    // Spawn on ground: row 15 * 64px = 960px. Subtract character height (50px).
    // In segmented terrain, we spawn at x=128 to ensure we hit the first platform.
    float startX = 128.0f;
    float startY = 850.0f;
    for(int i=0; i<4; i++) {
        characters[i]->setPosition(startX, startY);
        characters[i]->setActive(true);
        characters[i]->heal(100);
    }

	level->setPlayerPtr(characters[activeCharIdx]);

	if (soundManager) {
		soundManager->playMusic("Audio/stage1_ost.wav");
	}

	UltimateBoss* boss = new UltimateBoss(&level->getEntityManager());
	boss->setPosition(10000.0f, (float)((LEVEL_HEIGHT - 5) * CELL_SIZE));
	level->addEnemy(boss);
}

void PlayState::handleEvents(sf::RenderWindow& window, sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::P) {
			isPaused = !isPaused;
			if (soundManager) {
				if (isPaused) soundManager->pauseMusic();
				else soundManager->resumeMusic();
			}
		}
		if (!isPaused && event.key.code == sf::Keyboard::R) {
			timeline.rewind();
		}
	}
}

void PlayState::update(float dt)
{
	if (isPaused) return;

	elapsedTime += dt;
	
	bool zDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
	if (zDown && !zWasPressed) {
		float oldX = characters[activeCharIdx]->getX();
		float oldY = characters[activeCharIdx]->getY();
		activeCharIdx = (activeCharIdx + 1) % 4;
		characters[activeCharIdx]->setPosition(oldX, oldY);
		level->setPlayerPtr(characters[activeCharIdx]);
	}
	zWasPressed = zDown;

	if (characters[activeCharIdx]->getLives() <= 0) {
		bool anyAlive = false;
		for (int i = 0; i < 4; i++) if (characters[i]->getLives() > 0) anyAlive = true;

		if (!anyAlive) {
			if (soundManager) soundManager->play("game_end");
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

	characters[activeCharIdx]->update(dt);
	if (level) {
		characters[activeCharIdx]->resolveGround(level->getTileGrid(), level->getHeight(), level->getWidth(), level->getCellSize());
		level->update(dt, *scoreManager);
		camera->update(characters[activeCharIdx]->getX(), characters[activeCharIdx]->getY(), dt);
		
		if (characters[activeCharIdx]->getX() > 9500.0f && soundManager) {
			soundManager->playMusic("Audio/bossfight.ogg");
		}
	}
	
	hud->update(characters[activeCharIdx]->getHp(), 100, 
				scoreManager->getScore(), characters[activeCharIdx]->getName(), 
				characters[activeCharIdx]->getAmmo(), characters[activeCharIdx]->getGrenadeCount(), dt);

	takeSnapshot(dt);
}

void PlayState::takeSnapshot(float dt)
{
	GameStateSnapshot snap;
	snap.charX = characters[activeCharIdx]->getX();
	snap.charY = characters[activeCharIdx]->getY();
	snap.charHp = characters[activeCharIdx]->getHp();
	snap.charState = CharacterState::NORMAL; 
	snap.ammo = characters[activeCharIdx]->getAmmo(); 
	snap.grenadeCount = characters[activeCharIdx]->getGrenadeCount();
	snap.timestamp = elapsedTime;
	
	const char* wType = "PISTOL";
	int i = 0;
	for (; i < 31 && wType[i] != '\0'; i++) {
		snap.weaponType[i] = wType[i];
	}
	snap.weaponType[i] = '\0';

	timeline.recordFrame(snap);
}

void PlayState::render(sf::RenderWindow& window)
{
	float pX = characters[activeCharIdx]->getX();
	if (pX < 3200.0f) bgSprite.setTexture(jungleBgTex);
	else if (pX < 9600.0f) bgSprite.setTexture(aerialBgTex);
	else bgSprite.setTexture(aquaticBgTex);

	window.draw(bgSprite);

	if (level) level->draw(window, *camera);
	characters[activeCharIdx]->draw(window, camera->getOffsetX(), camera->getOffsetY());
	hud->draw(window);

	if (isPaused) {
		window.draw(pauseText);
	}

	if (!isPaused && bossTransitionFlash > 0.0f) {
		sf::RectangleShape flash(sf::Vector2f((float)SCREEN_W, (float)SCREEN_H));
		float alpha = (bossTransitionFlash / 0.3f) * 200.f;
		flash.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)alpha));
		window.draw(flash);
		bossTransitionFlash -= 0.016f; 
	}
}

void PlayState::onExit() {}
