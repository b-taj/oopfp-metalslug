#pragma once
#include "GameState.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class GameStateManager;

/**
 * MenuState.h -- Main menu for mode selection.
 */

class MenuState : public GameState
{
public:
	MenuState(GameStateManager* gsm);
	void		onEnter() override;
	void		handleEvents(sf::RenderWindow& window, sf::Event& event) override;
	void		update(float dt) override;
	void		render(sf::RenderWindow& window) override;
	void		onExit() override;
	GameStateID	getID() const override { return GameStateID::MENU; }

private:
	sf::Font			font;
	sf::Text			title;
	sf::Text			options[3];
	GameStateManager*	stateManager;
};
