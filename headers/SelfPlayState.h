#pragma once
#include "GameState.h"
#include "NEATManager.h"
#include "Level.h"
#include "Camera.h"
#include "HUD.h"
#include "ScoreManager.h"
#include "Players.h"

class SelfPlayState : public GameState
{
public:
	SelfPlayState(Level* lvl, Camera* cam, HUD* h, ScoreManager* s);
	~SelfPlayState();

	void		onEnter() override;
	void		handleEvents(sf::RenderWindow& window, sf::Event& event) override;
	void		update(float dt) override;
	void		render(sf::RenderWindow& window) override;
	void		onExit() override;
	GameStateID	getID() const override { return GameStateID::SELF_PLAY; }

private:
	NEATManager		neatManager;
	Level*			level;
	Camera*			camera;
	HUD*			hud;
	ScoreManager*	scoreManager;
	
	PlayerMarco*	agent;
	PlayerMarco*	bestAgent; // Ghost/Best
	Genome*			currentGenome;
	
	bool			showGlobalBest;
	float			timeSurvived;
	int				enemiesKilledThisRun;
	float			distanceTravelled;
	int				deathCount;
};
