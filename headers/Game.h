#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "GameStateManager.h"
#include "PlayerSoldier.h"
#include "Camera.h"
#include "Level.h"
#include "HUD.h"
#include "ScoreManager.h"
#include "SoundManager.h"

// Game.h -- Central engine controller. Owns all major subsystems and manages
// the main loop (handleEvents -> update -> render).

class Game
{
public:
	void	run();

private:
	sf::RenderWindow	window;
	GameStateManager	stateManager;

	Level				level;
	PlayerSoldier*		characters[4];
	Camera				camera;
	HUD					hud;
	ScoreManager		scoreManager;
	SoundManager		soundManager;

	bool				isDeveloperMode;
	float				devKeyTimes[3];
	int					devKeyPressCount;
