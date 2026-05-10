#pragma once
#include "GameState.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

/**
 * PauseState.h -- Overlay state that freezes logic.
 */

class PauseState : public GameState
{
public:
	PauseState();
	void		onEnter() override;
	void		handleEvents(sf::RenderWindow& window, sf::Event& event) override;
	void		update(float dt) override;
	void		render(sf::RenderWindow& window) override;
	void		onExit() override;
	GameStateID	getID() const override { return GameStateID::PAUSE; }

private:
	sf::Font	font;
	sf::Text	text;

	sf::Texture bgTexture;
	sf::Sprite  bgSprite;
};
