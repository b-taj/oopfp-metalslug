#include "../headers/GameStateManager.h"

GameStateManager::GameStateManager() : stateCount(0), currentState(nullptr)
{
	for (int i = 0; i < 8; ++i) states[i] = nullptr;
}

GameStateManager::~GameStateManager()
{
	for (int i = 0; i < stateCount; ++i) {
		if (states[i]) delete states[i];
	}
}

void GameStateManager::registerState(GameState* state)
{
	if (stateCount < 8 && state) {
		states[stateCount++] = state;
	}
}

void GameStateManager::changeState(GameStateID id)
{
	for (int i = 0; i < stateCount; ++i) {
		if (states[i]->getID() == id) {
			if (currentState) currentState->onExit();
			currentState = states[i];
			currentState->onEnter();
			return;
		}
	}
}

void GameStateManager::handleEvents(sf::RenderWindow& window, sf::Event& event)
{
	if (currentState) currentState->handleEvents(window, event);
}

void GameStateManager::update(float dt)
{
	if (currentState) currentState->update(dt);
}

void GameStateManager::render(sf::RenderWindow& window)
{
	if (currentState) currentState->render(window);
}

GameStateID GameStateManager::getCurrentID() const
{
	return currentState ? currentState->getID() : GameStateID::MENU;
}
