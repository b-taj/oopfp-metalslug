#pragma once
#include "PlayerSoldier.h"

class PlayerMarco : public PlayerSoldier {
public:
    PlayerMarco() {
        hp = 100; speedX = 200.0f; grenadeCount = 8;
        // Placeholder animations
        SpriteAnimator::Frame f = { sf::IntRect(0,0,100,100), 0.1f };
        animator.addAnimation("idle", &f, 1, true);
        animator.addAnimation("walk", &f, 1, true);
        animator.addAnimation("jump", &f, 1, true);
        animator.addAnimation("die", &f, 1, false);
    }
    void activateSpecialPower() override {}
    float getFireRate() const override { return 1.25f; }
    const char* getName() const override { return "MARCO"; }
};

class PlayerTarma : public PlayerSoldier {
public:
    PlayerTarma() {
        hp = 80; speedX = 160.0f; // -20% HP/Speed
    }
    void activateSpecialPower() override {}
    float getFireRate() const override { return 1.0f; }
    const char* getName() const override { return "TARMA"; }
};

class PlayerEri : public PlayerSoldier {
public:
    PlayerEri() {
        grenadeCount = 20; // double
    }
    void activateSpecialPower() override {}
    float getFireRate() const override { return 0.8f; } // -20%
    const char* getName() const override { return "ERI"; }
};

class PlayerFio : public PlayerSoldier {
public:
    PlayerFio() {
        grenadeCount = 8; // -2 grenades
    }
    void activateSpecialPower() override {}
    float getFireRate() const override { return 1.1f; } // +10%
    const char* getName() const override { return "FIO"; }
};
