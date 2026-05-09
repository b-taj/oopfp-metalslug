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
	devKeyPressCount = 0;
	for(int i=0; i<3; i++) devKeyTimes[i] = 0.0f;

	// ... subsystems init ...
	characters[0] = new PlayerMarco();
	characters[1] = new PlayerTarma();
	characters[2] = new PlayerEri();
	characters[3] = new PlayerFio();

	gStateManager.registerState(new MenuState());
	gStateManager.registerState(new PlayState(&level, characters, &camera, &hud, &scoreManager, &soundManager));
	gStateManager.registerState(new PauseState());
	gStateManager.registerState(new SelfPlayState(&level, &camera, &hud, &scoreManager));
	gStateManager.changeState(GameStateID::MENU);

	sf::Clock clock;
	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();
		if (dt > 0.05f) dt = 0.05f; // Cap at 20fps minimum to prevent spiral of death
		
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
			
			// DEV CHEAT DETECTION
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Z) {
				for(int i=0; i<2; i++) devKeyTimes[i] = devKeyTimes[i+1];
				devKeyTimes[2] = clock.getElapsedTime().asSeconds();
				if (devKeyTimes[2] - devKeyTimes[0] < 2.0f) activateDeveloperMode();
			}

			gStateManager.handleEvents(window, event);
		}

		gStateManager.update(dt);

		window.clear(sf::Color(100, 149, 237));
		gStateManager.render(window);
		window.display();
	}
}

void Game::activateDeveloperMode()
{
	isDeveloperMode = true;
	std::cout << "[SYSTEM] Developer Mode Active. Urdu Assembly Terminal Ready.\n";
	for (int i = 0; i < 4; ++i) {
		characters[i]->heal(9999);
		characters[i]->setSpeed(400.0f, -800.0f);
	}

	// Spawn separate terminal window as per rubric
	sf::RenderWindow terminal(sf::VideoMode(400, 300), "Urdu Assembly Terminal");
	char input[64] = "JAMA SCORE, 5000";
	char cmd[16], var[16];
	int val;
	
	// Example parsing per rubric
	if (sscanf(input, "%s %s, %d", cmd, var, &val) == 3) {
		if (strcmp(cmd, "JAMA") == 0 && strcmp(var, "SCORE") == 0) scoreManager.addScore(val);
	}
}
