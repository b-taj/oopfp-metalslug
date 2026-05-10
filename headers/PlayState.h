#pragma once
#include "GameState.h"
#include "Level.h"
#include "HUD.h"
#include "ScoreManager.h"
#include "Camera.h"
#include "PlayerSoldier.h"
#include "SoundManager.h"
#include "Timeline.h"

class GameStateManager;

/**
 * PlayState.h -- The active gameplay state.
 */

class PlayState : public GameState
{
public:
	PlayState(Level* lvl, PlayerSoldier** heroes, Camera* cam, HUD* h, ScoreManager* s, SoundManager* snd, GameStateManager* gsm);

	void			onEnter() override;
	void			handleEvents(sf::RenderWindow& window, sf::Event& event) override;
	void			update(float dt) override;
	void			render(sf::RenderWindow& window) override;
	void			onExit() override;
	GameStateID		getID() const override { return GameStateID::PLAY; }

private:
	void			takeSnapshot(float dt);

	Level*			level;
	PlayerSoldier**	characters;
	Camera*			camera;
	HUD*			hud;
	ScoreManager*	scoreManager;
	SoundManager*	soundManager;
	GameStateManager* stateManager;
	Timeline		timeline;

	int				activeCharIdx;
	float			zPressTimer;
	bool			zWasPressed;
	float			elapsedTime;
	float			bossTransitionFlash;
};
