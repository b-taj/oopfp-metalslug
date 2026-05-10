#include "../headers/PauseState.h"
#include "../headers/GameStateManager.h"
#include <SFML/Window/Keyboard.hpp>

PauseState::PauseState()
{
	if (bgTexture.loadFromFile("Sprites/backgrounds/pause_screen.png")) {
		bgSprite.setTexture(bgTexture);
		bgSprite.setColor(sf::Color(255, 255, 255, 180));
	}

	if (font.loadFromFile("Sprites/font.ttf")) {
		text.setFont(font);
		text.setString("PAUSED");
		text.setCharacterSize(100);
		text.setFillColor(sf::Color::Yellow);
		text.setPosition(650.0f, 350.0f);
	}
}

void PauseState::onEnter() {}

void PauseState::handleEvents(sf::RenderWindow& window, sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::P || event.key.code == sf::Keyboard::Escape)) {
		// Note: PlayState handles the local toggle, this is for manager-level transition if needed
		// But based on Task 3, we toggle locally in PlayState
	}
}

void PauseState::update(float dt) { (void)dt; }

void PauseState::render(sf::RenderWindow& window)
{
	window.draw(bgSprite);
	window.draw(text);
}

void PauseState::onExit() {}
