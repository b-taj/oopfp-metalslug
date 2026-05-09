#include "../headers/ReflexFingerprint.h"
#include "../headers/GhostPlayer.h"
#include "../headers/BehaviorChain.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

void ReflexFingerprint::record(int code, float dt) { (void)code; (void)dt; }
void ReflexFingerprint::saveToFile(const char* path) { (void)path; }
void ReflexFingerprint::loadFromFile(const char* path) { (void)path; }
int ReflexFingerprint::getNextAction() { return 0; }
void ReflexFingerprint::activateAutopilot(float dur) { (void)dur; }

void GhostPlayer::update(float dt) { (void)dt; }
void GhostPlayer::draw(sf::RenderWindow& w) { (void)w; }
void GhostPlayer::loadProfile(const char* p) { (void)p; }

BehaviorState BehaviorChain::inferState(float* in, int count) { (void)in; (void)count; return BehaviorState::PRESSING; }
BehaviorState BehaviorChain::predictNext() { return BehaviorState::PRESSING; }
void BehaviorChain::updateTransitions(BehaviorState f, BehaviorState t) { (void)f; (void)t; }
BehaviorState BehaviorChain::getGapState() { return BehaviorState::STALLING; }
void BehaviorChain::saveHistory(const char* f) { (void)f; }
void BehaviorChain::loadHistory(const char* f) { (void)f; }
