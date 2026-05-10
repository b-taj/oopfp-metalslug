#pragma once
#include "Enums.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

/**
 * GameState.h -- Abstract base for the State Pattern.
 * Replaces monolithic branches in the game loop.
 */

class GameState
{
public:
	virtual ~GameState() = default;

	virtual void		onEnter() = 0;
	virtual void		handleEvents(sf::RenderWindow& window, sf::Event& event) = 0;
	virtual void		update(float dt) = 0;
	virtual void		render(sf::RenderWindow& window) = 0;
	virtual void		onExit() = 0;
	virtual GameStateID	getID() const = 0;

	virtual const char* serialize() const { return ""; }
	virtual void deserialize(const char* data) { (void)data; }
};
