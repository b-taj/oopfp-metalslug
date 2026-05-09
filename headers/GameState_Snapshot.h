#pragma once
#include "Enums.h"

/**
 * GameStateSnapshot.h -- Lightweight container for the state of a single frame.
 * Used by the Timeline system for rewinding and branching.
 */

struct GameStateSnapshot
{
	float			charX;
	float			charY;
	int				charHp;
	CharacterState	charState;
	int				ammo;
	int				grenadeCount;
	float			timestamp;
	char			weaponType[32];
};
