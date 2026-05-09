#pragma once
#include "Terrain.h"
#include "NEAT.h"

class Level;

// GameMode.h -- Abstract base for the three play modes.
// Game holds a GameMode* and calls its virtual methods each frame,
// enabling runtime mode switching without restarting the main loop.

class GameMode
{
public:
	virtual ~GameMode() = default;

	virtual void	start() = 0;			// initialise levels, spawn entities
	virtual void	update(float dt) = 0;		// advance all game logic one frame
	virtual bool	checkWinCondition() = 0;	// true when mode is complete
	virtual void	onLevelComplete() = 0;		// transition to next level or end screen

	int			getScore();

protected:
	bool		isActive;
	Level*		currentLevel;
	int			enemiesKilled;
};

// 3 normal levels + 1 boss level with horizontal and vertical scrolling.
// Enemy positions are pre-defined and baked into each Level.
class SurvivalMode : public GameMode
{
public:
	void	start() override;
	void	update(float dt) override;
	bool	checkWinCondition() override;	// true when boss level is cleared
	void	onLevelComplete() override;		// advance currentLevelNum; load next level

private:
	int		currentLevelNum;	// 0-2 = normal levels; 3 = boss level
	Level*	levels[4];			// owned
	Level*	bossLevel;			// alias for levels[3]

	void	loadLevel(int n);
};

// Infinite procedurally generated world. One level; complete when kill quota is met.
// Enemies spawn dynamically at runtime. Player chooses a terrain profile at start.
class CampaignMode : public GameMode
{
public:
	void	start() override;
	void	update(float dt) override;
	bool	checkWinCondition() override;	// true when all 9 kill quotas are satisfied
	void	onLevelComplete() override;

	// Generate the next off-screen terrain chunk as the player advances.
	void	generateChunk(int x);

	// Apply a terrain profile (Amplified / Flat / Normal) chosen from the menu.
	void	setProfile(LevelProfileFactory* factory);

private:
	TerrainGenerator	generator;
	int			killQuota[9];		// 5 kills required per enemy type
	LevelProfileFactory*	selectedProfile;	// aggregated -- owned by menu
};

// NEAT-driven self-play. Evolves indefinitely; each generation improves over the last.
// Key B highlights the all-time best genome running alongside the current generation.
class SelfPlayingMode : public GameMode
{
public:
	void	start() override;
	void	update(float dt) override;
	bool	checkWinCondition() override;	// always false; exits only on user input
	void	onLevelComplete() override;		// restart level; advance generation

	void	toggleBestPlayer();		// key B: show / hide globalBestGenome agent
	void	saveGlobalBest();
	void	loadGlobalBest();

private:
	NEATManager	neatManager;
	Genome*		globalBestGenome;	// best agent across all sessions (owned)
	float		globalBestScore;
	bool		showBestPlayer;

	void	evolveGeneration();	// one complete NEAT generation tick
};
