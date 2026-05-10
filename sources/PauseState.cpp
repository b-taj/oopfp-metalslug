#include "../headers/PauseState.h"
#include "../headers/GameStateManager.h"
#include <SFML/Window/Keyboard.hpp>

extern GameStateManager gStateManager;

PauseState::PauseState()
{
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
		gStateManager.changeState(GameStateID::PLAY);
	}
}

void PauseState::update(float dt) { (void)dt; }

void PauseState::render(sf::RenderWindow& window)
{
	window.draw(text);
}

void PauseState::onExit() {}
