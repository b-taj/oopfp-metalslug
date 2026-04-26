#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include "GameMode.h"
#include "Character.h"
#include "Camera.h"
#include "HUD.h"
#include "ScoreManager.h"
#include "SoundManager.h"

// Game.h -- Root class. Owns every top-level subsystem and drives the main loop.
// Constructed in main(); call run() to enter the 1600x900 60-fps game loop.

class Game
{
public:
	// Initialise SFML window, load assets, then enter the main loop.
	void	run();

private:
	// ── main loop phases ─────────────────────────────────────────────────
	void	handleEvents();			// SFML event polling (close, keypress, etc.)
	void	update(float dt);			// advance all subsystems one frame
	void	render();				// clear -> draw world -> draw HUD -> display

	// Cycle activeCharIdx to the next living character (key Z).
	void	switchCharacter();

	// Swap currentMode to mode and call mode->start().
	void	setGameMode(GameMode* mode);

	// Detect the developer-mode cheat key sequence (triple-press within 2 s).
	void	checkDevModeCheat();

	// Grant immortality and all weapons to every character (for demo testing).
	void	activateDeveloperMode();

	// ── subsystems ────────────────────────────────────────────────────────
	sf::RenderWindow	window;			// 1600x900, Style::Close, 60 fps
	GameMode*		currentMode;		// active play mode (owned)
	Character*		characters[4];		// Marco, Tarma, Eri, Fio (owned)
	int			activeCharIdx;		// index into characters[]
	Camera			camera;
	HUD			hud;
	ScoreManager		scoreManager;
	SoundManager		soundManager;
	bool			isDeveloperMode;
};
