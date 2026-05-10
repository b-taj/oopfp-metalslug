#include "../headers/MummyState.h"
#include "../headers/NormalState.h"

MummyState::MummyState() : timer(10.0f) {}

void MummyState::onEnter(class Soldier* s) {
    // Prompt: forces Soldier to knife-only
    // s->forceKnifeOnly(true);
    // s->setState(CharacterState::MUMMY);
}

void MummyState::update(class Soldier* s, float dt) {
    timer -= dt;
    if (timer <= 0) {
        // s->setTransformationState(new NormalState());
    }
}

void MummyState::onExit(class Soldier* s) {
    // Prompt: disables knife-only
    // s->forceKnifeOnly(false);
}

const char* MummyState::getName() const {
    return "MUMMY";
}
