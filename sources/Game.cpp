#include "../headers/Game.h"
#include "../headers/MenuState.h"
#include "../headers/PlayState.h"
#include "../headers/PauseState.h"
#include "../headers/SelfPlayState.h"
#include "../headers/Players.h"
#include <SFML/System/Clock.hpp>

// Global pointer for state manager access (simpler for this context)
GameStateManager gStateManager;

void Game::run()
{
	window.create(sf::VideoMode(1600, 900), "Metal Slug VS - OOP Edition", sf::Style::Close);
	window.setFramerateLimit(60);

	isDeveloperMode = false;
	for(int i=0; i<3; i++) devKeyTimes[i] = 0.0f;
	devKeyPressCount = 0;

	// Subsystems init
	characters[0] = new PlayerMarco();
	characters[1] = new PlayerTarma();
	characters[2] = new PlayerEri();
	characters[3] = new PlayerFio();

	for(int i=0; i<4; i++) characters[i]->loadTexture(""); 

	hud.init();

	// Load Sounds
	soundManager.load("shoot", "Audio/shoot.wav");
	soundManager.load("explosion", "Audio/explosion.wav");
	soundManager.load("enemy_die", "Audio/enemy_die.wav");
	soundManager.load("player_hit", "Audio/player_hit.wav");
	soundManager.load("jump", "Audio/jump.wav");
	soundManager.load("pickup", "Audio/pickup.wav");

	stateManager.registerState(new MenuState());
	stateManager.registerState(new PlayState(&level, characters, &camera, &hud, &scoreManager, &soundManager));
	stateManager.registerState(new PauseState());
	stateManager.registerState(new SelfPlayState(&level, &camera, &hud, &scoreManager));

	stateManager.changeState(GameStateID::MENU);

	sf::Clock clock;
	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();
		if (dt > 0.05f) dt = 0.05f;
		
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
				for(int i=0; i<2; i++) devKeyTimes[i] = devKeyTimes[i+1];
				devKeyTimes[2] = clock.getElapsedTime().asSeconds();
				if (devKeyTimes[2] - devKeyTimes[0] < 2.0f) activateDeveloperMode();
			}

			stateManager.handleEvents(window, event);
		}

		stateManager.update(dt);

		window.clear(sf::Color(100, 149, 237));
		stateManager.render(window);
		window.display();
	}

	for (int i = 0; i < 4; ++i) delete characters[i];
}

void Game::activateDeveloperMode()
{
	isDeveloperMode = true;
	for (int i = 0; i < 4; ++i) {
		characters[i]->heal(9999);
		characters[i]->setSpeed(400.0f, -800.0f);
	}
}
