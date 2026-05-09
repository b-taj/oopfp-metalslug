#include "../headers/MenuState.h"
#include "../headers/GameStateManager.h"
#include <SFML/Window/Keyboard.hpp>

extern GameStateManager gStateManager;

MenuState::MenuState()
{
	if (font.loadFromFile("Sprites/font.ttf")) {
		title.setFont(font);
		title.setString("METAL SLUG VS");
		title.setCharacterSize(80);
		title.setPosition(500.0f, 100.0f);

		const char* labels[] = { "1. SURVIVAL", "2. CAMPAIGN", "3. SELF-PLAY" };
		for (int i = 0; i < 3; ++i) {
			options[i].setFont(font);
			options[i].setString(labels[i]);
			options[i].setCharacterSize(40);
			options[i].setPosition(650.0f, 300.0f + i * 100.0f);
		}
	}
}

void MenuState::onEnter() {}

void MenuState::handleEvents(sf::RenderWindow& window, sf::Event& event)
{
	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Num2 || event.key.code == sf::Keyboard::Num3) {
			gStateManager.changeState(GameStateID::PLAY);
		}
	}
}

void MenuState::update(float dt) { (void)dt; }

void MenuState::render(sf::RenderWindow& window)
{
	window.draw(title);
	for (int i = 0; i < 3; ++i) window.draw(options[i]);
}

void MenuState::onExit() {}
