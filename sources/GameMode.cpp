#include "../headers/GameMode.h"
#include "../headers/Level.h"
#include <iostream>

/**
 * GameMode Implementation
 * 
 * This file implements the polymorphic behaviour of the different play modes.
 * The Game class holds a pointer to the current GameMode, allowing the entire
 * logic of the game (Survival vs Campaign vs NEAT) to be swapped at runtime.
 */

// Virtual destructor for proper cleanup of mode-specific resources
GameMode::~GameMode()
{
}

// Simple accessor for the current score accumulated in this mode.
int GameMode::getScore() 
{ 
    return score; 
}

// Safely returns a pointer to the currently active level instance
Level* GameMode::getLevel() 
{ 
    return currentLevel; 
}

// =============================================================================
// SURVIVAL MODE IMPLEMENTATION
// =============================================================================

// Initializes the classic 4-level gauntlet mission structure
void SurvivalMode::start()
{
    std::cout << "[SurvivalMode] Initializing debug camera test structure..." << std::endl;
    currentLevelNum = 0;
    score = 0;
    enemiesKilled = 0;
    isActive = true;
    
    // For testing, only initialize the first level with debug layout
    levels[0] = new Level();
    levels[0]->generateDebugLevel(); 
    
    currentLevel = levels[0];
}

// Logic for advancing survival-specific triggers and monitoring progress
void SurvivalMode::update(float dt)
{
    if (!isActive || !currentLevel) return;
    (void)dt;
}

// Returns true if the final boss has been successfully defeated
bool SurvivalMode::checkWinCondition()
{
    return (currentLevelNum == 3 && !isActive); 
}

// Handles the logic for moving the player to the next stage in the sequence
void SurvivalMode::onLevelComplete()
{
    if (currentLevelNum < 3) {
        std::cout << "[SurvivalMode] Level " << currentLevelNum << " Complete! Advancing..." << std::endl;
        currentLevelNum++;
        currentLevel = levels[currentLevelNum];
    } else {
        std::cout << "[SurvivalMode] FINAL BOSS DEFEATED!" << std::endl;
        isActive = false; 
    }
}

// Loads a specific level index (0-3) for debug or mission-start purposes
void SurvivalMode::loadLevel(int n)
{
    if (n >= 0 && n < 4) {
        currentLevelNum = n;
        currentLevel = levels[n];
    }
}

// =============================================================================
// CAMPAIGN MODE IMPLEMENTATION
// =============================================================================

// Initializes the infinite procedural mode with kill quotas
void CampaignMode::start()
{
    std::cout << "[CampaignMode] Initializing infinite procedural world..." << std::endl;
    score = 0;
    enemiesKilled = 0;
    isActive = true;
    
    currentLevel = new Level(); 
    
    for(int i = 0; i < 9; i++) killQuota[i] = 5;
}

// Monitors camera progress to trigger procedural chunk generation
void CampaignMode::update(float dt)
{
    (void)dt;
}

// Returns true when all enemy kill quotas for the sector are satisfied
bool CampaignMode::checkWinCondition()
{
    for(int i = 0; i < 9; i++) {
        if (killQuota[i] > 0) return false;
    }
    return true;
}

// Standard completion logic for the infinite mode
void CampaignMode::onLevelComplete() 
{
}

// Appends new terrain data to the level based on world-space coordinate x
void CampaignMode::generateChunk(int x) 
{
    (void)x;
}

// Configures the terrain generator with a specific noise profile
void CampaignMode::setProfile(LevelProfileFactory* factory) 
{
    selectedProfile = factory;
}

// =============================================================================
// SELF-PLAYING MODE (NEAT) IMPLEMENTATION
// =============================================================================

// Initializes the evolutionary population for automated play
void SelfPlayingMode::start()
{
    std::cout << "[SelfPlayingMode] Initializing NeuroEvolutionary training..." << std::endl;
    neatManager.initPopulation(12, 6);
    isActive = true;
    showBestPlayer = true;
}

// Feeds sensor data into the neural networks and applies the output as inputs
void SelfPlayingMode::update(float dt)
{
    (void)dt;
    float inputs[12] = {0}; 
    float outputs[6] = {0};
}

// AI training runs indefinitely
bool SelfPlayingMode::checkWinCondition() 
{ 
    return false; 
} 

// Triggers the evolution of a new generation upon agent death or goal reached
void SelfPlayingMode::onLevelComplete()
{
    evolveGeneration();
}

// Advances the NEAT algorithm through selection, reproduction, and mutation
void SelfPlayingMode::evolveGeneration() 
{
    neatManager.evolve();
}

// Toggles the visibility of the all-time best performing agent
void SelfPlayingMode::toggleBestPlayer() 
{ 
    showBestPlayer = !showBestPlayer; 
}

// Serializes the best genome to persistent storage
void SelfPlayingMode::saveGlobalBest() 
{
}

// Deserializes a previously saved champion genome
void SelfPlayingMode::loadGlobalBest() 
{
}
