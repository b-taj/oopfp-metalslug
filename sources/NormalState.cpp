#include "../headers/NormalState.h"
#include "../headers/Soldier.h"

/**
 * Implementation of Soldier is required for these methods.
 * For the purpose of this prompt, we assume Soldier defines:
 * - void setSpeedMultiplier(float m)
 * - void setCharacterState(CharacterState st)
 */

// We will use a mock Soldier header or forward declarations in implementation
#include "../headers/Enums.h"

class Soldier; 

void NormalState::onEnter(class Soldier* s) {
    // Logic to restore full speed and state (to be implemented in Soldier)
}

void NormalState::update(class Soldier* s, float dt) {
    (void)s; (void)dt;
}

void NormalState::onExit(class Soldier* s) {
    (void)s;
}

const char* NormalState::getName() const {
    return "NORMAL";
}
