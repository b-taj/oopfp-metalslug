#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"

/**
 * ScoreManager.h -- Tracks player score and handles the leaderboard.
 * Implements specific scoring rules and persistence using file I/O.
 */

class ScoreManager
{
public:
	ScoreManager();

	void	addScore(int pts);
	int		getScore() const;

	// Rubric-specific scoring logic
	void	recordKill(Enemy* e, bool isMelee, bool isAerial, int multiKillCount);
	void	recordLevelClear(bool isSurvival, bool isFlawless);

	// Leaderboard logic
	void	addToLeaderboard(const char* name, int score);
	void	saveLeaderboard(const char* path);
	void	loadLeaderboard(const char* path);
	void	drawLeaderboard(sf::RenderWindow& w, sf::Font& font);

	// Rage Bait & Death Compositor
	void	updateExpectation();
	void	recordDeath(class Soldier* s, float timeSinceCheckpoint);
	bool	matchesDeathPattern(class Soldier* s);

private:
	int		score;
	int		highScores[10];
	int		highScoreCount;
	char	highScoreNames[10][32];

	// Rage Bait
	float	expectedScore;
	float	historicMidpoints[20];
	int		midpointCount;
	float	targetGap;

	// Death Compositor
	struct DeathFingerprint {
		int hp; float posX, posY; char weapon[32]; float timeSinceCheckpoint;
	};
	DeathFingerprint deathHistory[50];
	int deathCount;
};
