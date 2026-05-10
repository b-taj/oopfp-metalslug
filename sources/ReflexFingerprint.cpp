#include "../headers/ReflexFingerprint.h"
#include <iostream>
#include <cmath>

ReflexFingerprint::ReflexFingerprint() {
    for (int i = 0; i < 10; i++) metadata[i] = 0;
    // ... initialize others if needed
}

void ReflexFingerprint::record(int code, float dt) { (void)code; (void)dt; }
void ReflexFingerprint::saveToFile(const char* path) { (void)path; }
void ReflexFingerprint::loadFromFile(const char* path) { (void)path; }
int ReflexFingerprint::getNextAction() { return 0; }
void ReflexFingerprint::activateAutopilot(float dur) { (void)dur; }
