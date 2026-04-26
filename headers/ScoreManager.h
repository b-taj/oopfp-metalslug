#pragma once

#include "Timeline.h"			// DeathFingerprint
#include "ReflexFingerprint.h"		// BehaviorChain
#include "GameState.h"

class Enemy;

// ScoreManager.h -- Tracks score, expected score, and death patterns.
//
// Rage Bait: maintains a gap between expected and actual score to keep
//   the player motivated for one more run.
// Death Compositor: steers the environment toward historically fatal
//   enemy configurations without directly killing the player.

class ScoreManager
{
public:
	void	addScore(int pts);
	int		getScore();

	// Award points for an enemy kill. bonusType selects melee/aerial/multi-kill bonus.
	void	recordKill(Enemy* enemy, int bonusType);

	// Store a death snapshot and update the expected score model.
	void	recordDeath(const DeathFingerprint& fp);

	// Adjust expectedScore using historical data (Rage Bait loop).
	void	updateExpectation();

	// True when current game state closely matches a stored death fingerprint.
	bool	matchesDeathPattern(const GameState& state);

private:
	int					score;
	int					expectedScore;
	std::vector<int>		historicExpected;		// rolling record for Rage Bait model
	std::vector<DeathFingerprint>	deathFingerprints;
	BehaviorChain			behaviorChain;			// Chains of Death state machine
};
