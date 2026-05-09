#pragma once
#include "Terrain.h"
#include "NEAT.h"

class Level;

// GameMode.h -- Abstract base for the three play modes.
// Game holds a GameMode* and calls its virtual methods each frame,
// enabling runtime mode switching without restarting the main loop.
// Removed restricted libraries.

class GameMode
{
public:
	virtual ~GameMode();

	virtual void	start() = 0;			
	virtual void	update(float dt) = 0;		
	virtual bool	checkWinCondition() = 0;	
	virtual void	onLevelComplete() = 0;		

	int			getScore();
	Level*		getLevel();

protected:
	bool		isActive;
	Level*		currentLevel;
	int			enemiesKilled;
	int			score;
};

// 3 normal levels + 1 boss level with horizontal and vertical scrolling.
// Enemy positions are pre-defined and baked into each Level.
class SurvivalMode : public GameMode
{
public:
	void	start() override;
	void	update(float dt) override;
	bool	checkWinCondition() override;	
	void	onLevelComplete() override;		

private:
	int		currentLevelNum;	
	Level*	levels[4];			
	Level*	bossLevel;			

	void	loadLevel(int n);
};

// Infinite procedurally generated world. One level; complete when kill quota is met.
// Enemies spawn dynamically at runtime. Player chooses a terrain profile at start.
class CampaignMode : public GameMode
{
public:
	void	start() override;
	void	update(float dt) override;
	bool	checkWinCondition() override;	
	void	onLevelComplete() override;

	// Generate the next off-screen terrain chunk as the player advances.
	void	generateChunk(int x);

	// Apply a terrain profile (Amplified / Flat / Normal) chosen from the menu.
	void	setProfile(LevelProfileFactory* factory);

private:
	TerrainGenerator	generator;
	int			killQuota[9];		
	LevelProfileFactory*	selectedProfile;	
};

// NEAT-driven self-play. Evolves indefinitely; each generation improves over the last.
// Key B highlights the all-time best genome running alongside the current generation.
class SelfPlayingMode : public GameMode
{
public:
	void	start() override;
	void	update(float dt) override;
	bool	checkWinCondition() override;	
	void	onLevelComplete() override;		

	void	toggleBestPlayer();		
	void	saveGlobalBest();
	void	loadGlobalBest();

private:
	NEATManager	neatManager;
	Genome*		globalBestGenome;	
	float		globalBestScore;
	bool		showBestPlayer;

	void	evolveGeneration();	
};
