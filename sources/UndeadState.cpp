#include "../headers/UndeadState.h"
#include "../headers/NormalState.h"
// Implementation depends on Soldier.h which we will define next if needed.
// For now, using forward declarations and hypothetical Soldier methods from prompt.

UndeadState::UndeadState() : timer(10.0f) {}

void UndeadState::onEnter(class Soldier* s) {
    // Prompt: reduces Soldier speed by 50%, sets CharacterState::UNDEAD
    // s->setSpeedMultiplier(0.5f);
    // s->setState(CharacterState::UNDEAD);
}

void UndeadState::update(class Soldier* s, float dt) {
    timer -= dt;
    if (timer <= 0) {
        // s->setTransformationState(new NormalState());
    }
}

void UndeadState::onExit(class Soldier* s) {
    // Prompt: restores speed
    // s->setSpeedMultiplier(1.0f);
}

const char* UndeadState::getName() const {
    return "UNDEAD";
}
