#pragma once
#include "GameState.h"

/**
 * GameStateManager.h -- Orchestrates transitions between Menu, Play, and Pause.
 * Owns a fixed-size pool of registered state objects.
 */

class GameStateManager
{
public:
	GameStateManager();
	~GameStateManager();

	void		registerState(GameState* state);
	void		changeState(GameStateID id);

	void		handleEvents(sf::RenderWindow& window, sf::Event& event);
	void		update(float dt);
	void		render(sf::RenderWindow& window);

	GameStateID	getCurrentID() const;

private:
	GameState*	states[8];
	int			stateCount;
	GameState*	currentState;
};
