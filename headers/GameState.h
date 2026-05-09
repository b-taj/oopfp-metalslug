#pragma once

#include "Enums.h"

// GameState.h -- Plain-data snapshot of the world at one frame.
// Used by Timeline (rewind), DeathFingerprint (death pattern matching),
// and SelfPlayingMode (NEAT fitness evaluation).

struct GameState
{
	float					charX;			// player world-X
	float					charY;			// player world-Y
	int					charHp;			// current hit-points
	CharacterState			charState;		// injury / transform state
	std::string				weaponType;		// active weapon identifier
	int					ammo;			// remaining ammo
	int					grenadeCount;
	std::vector<std::pair<float,float>>	enemyPositions; // (x,y) of all live enemies
	float					timestamp;		// seconds since level start

	std::string	serialize();
	void		deserialize(const std::string& data);
};
